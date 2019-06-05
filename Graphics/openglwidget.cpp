#include "openglwidget.h"
#include "input.h"
#include "camera.h"
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <iostream>
#include "qopenglextrafunctions.h"
#include "resources.h"
#include <random>
#include <QtMath>


#pragma comment(lib, "OpenGL32.lib")

OpenGlWidget::OpenGlWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(QSize(256, 256));
    openGLWidget = this;

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(Update()));
    if(format().swapInterval() == -1)
        timer.setInterval(16);
    else
        timer.setInterval(0);
    timer.start();
    setMouseTracking(true);
}

OpenGlWidget::~OpenGlWidget()
{
   makeCurrent();
   finalizeGL();
}

void OpenGlWidget::initializeGL()
{

    if(context()->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
    {
        QOpenGLDebugLogger* logger = new QOpenGLDebugLogger(this);
        logger->initialize();

        connect(logger, SIGNAL(messageLogged(const QOpenGLDebugMessage &)),
                this, SLOT(handleLoggedMessage(const QOpenGLDebugMessage &)));
        logger->startLogging();
    }
    initializeOpenGLFunctions();

    //CREATING PROGRAMS
    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/shader1_vert.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/shader1_frag.frag");
    program.link();

    deferredProgram.create();
    deferredProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/deferred_vert.vert");
    deferredProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/deferred_frag.frag");
    deferredProgram.link();

    blurProgram.create();
    blurProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/deferred_vert.vert");
    blurProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/blur_frag.frag");
    blurProgram.link();

    depthFieldProgram.create();
    depthFieldProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/deferred_vert.vert");
    depthFieldProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/depthfield_frag_copy.frag");
    depthFieldProgram.link();

    SSAOProgram.create();
    SSAOProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/deferred_vert.vert");
    SSAOProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/ssao_frag.frag");
    SSAOProgram.link();

    finalProgram.create();
    finalProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/deferred_vert.vert");
    finalProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/finalshader_frag.frag");
    finalProgram.link();


    //QUAD BUFFER
    InitializeBuffers();
    program.bind();

    float verticesQuad[] = {     /*Position*/-1.0f, -1.0f, 0.0f, /*Color*/ 1.0f, 1.0f, 1.0f,  /*TextureCoord*/ 0.0f, 0.0f,
                                                  1.0f, 1.0f, 0.0f,            1.0f, 1.0f, 1.0f,                   1.0f, 1.0f,
                                                  -1.0f, 1.0f, 0.0f,           1.0f, 1.0f, 1.0f,                   0.0f, 1.0f,
                                                  -1.0f, -1.0f, 0.0f,          1.0f, 1.0f, 1.0f,                   0.0f, 0.0f,
                                                   1.0f, -1.0f, 0.0f,          1.0f, 1.0f, 1.0f,                   1.0f, 0.0f,
                                                   1.0f, 1.0f, 0.0f,           1.0f, 1.0f, 1.0f,                   1.0f, 1.0f };
    if(!vbo.isCreated())
            vbo.create();
        vbo.bind();
        vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
        vbo.allocate(verticesQuad, 49 * sizeof(float));
        // VAO: Captures state of VBOs
        if(!vao.isCreated())
            vao.create();
        vao.bind();
        const GLint compCount = 3;
        const int strideBytes = 8 * sizeof(float);
        const int offsetBytesO = 0;
        const int offsetBytesl = sizeof(float)*3;
        const int offsetBytes2 = sizeof(float)*6;
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, compCount, GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytesO));
        glVertexAttribPointer(1, compCount, GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytesl));
        glVertexAttribPointer(2, compCount, GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes2));

}

void OpenGlWidget::resizeGL(int _width, int _height)
{

    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();

    gl_functions->glDeleteTextures(1, &colorTexture);
    gl_functions->glDeleteTextures(1, &normalTexture);
    gl_functions->glDeleteTextures(1, &depthTexture);
    gl_functions->glDeleteFramebuffers(1, &fbo);

    width = _width;
    height = _height;
    camera->ratio = (float)width / (float)height;
    InitializeBuffers();

}

void OpenGlWidget::paintGL()
{
    QOpenGLFunctions* gl_functions = QOpenGLContext::currentContext()->functions();

    gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    camera->Update();
    if(hierarchyRef == nullptr)
    {
        MainWindow* mainWindowRef = dynamic_cast<MainWindow*>( parent()->parent());
        hierarchyRef = mainWindowRef->getHierarchyWidget();
    }

    if(true)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }
    else
    {
        glDisable(GL_FALSE);
    }


    glClearDepth(1.0f);
     glClearColor(0.3f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, 0);
    if(program.bind())
    {
        hierarchyRef->RenderObjects(program.programId());
    }


        gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, deferredFbo);
        glClearColor(0.0f,0.0f,0.0f,1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
         glBindTexture(GL_TEXTURE_2D, 0);


    if(deferredProgram.bind())
    {
        int colorIndex = glGetUniformLocation(deferredProgram.programId(), "colorTex");
        int normalIndex  = glGetUniformLocation(deferredProgram.programId(), "normalMap");

        glUniform1i(colorIndex, 0);
        if(normalIndex != -1)
        glUniform1i(normalIndex, 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,colorTexture);

        if(normalIndex != -1){
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D,normalTexture);
          }

        vao.bind();

        glDrawArrays(GL_TRIANGLES, 0,6);
    }

    //BLUR---------------------
     gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);
     glClearColor(0.0f,0.0f,0.0f,1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     glBindTexture(GL_TEXTURE_2D, 0);
     glActiveTexture(GL_TEXTURE0);
    if(blurProgram.bind())
    {
        int colorIndex = glGetUniformLocation(blurProgram.programId(), "colorTex");
        int texCoordIncIndex = glGetUniformLocation(blurProgram.programId(), "texCoordInc");
        int blurIndex = glGetUniformLocation(blurProgram.programId(), "blur");
        int samplesIndex = glGetUniformLocation(blurProgram.programId(), "samples");
        glUniform1i(colorIndex, 0);
        glUniform1f(blurIndex, blur);
        glUniform1i(samplesIndex, samples);
        float vec[2] = {(1.0f/width) * 5, (1.0f/height) * 5};
        glUniform2fv(texCoordIncIndex, 1, &vec[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,deferredTexture);


        vao.bind();

        glDrawArrays(GL_TRIANGLES, 0,6);

    }

//BLUR----------

    //DEPTH OF FIELD----------
    gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, DepthFieldFbo);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

   if(depthFieldProgram.bind())
   {
       int colorIndex = glGetUniformLocation(depthFieldProgram.programId(), "colorTex");
       int depthIndex = glGetUniformLocation(depthFieldProgram.programId(), "depthTex");
       int typeIndex  = glGetUniformLocation(depthFieldProgram.programId(), "type");
       int texCoordIncIndex = glGetUniformLocation(depthFieldProgram.programId(), "texCoordInc");
       int focalLengthIndex = glGetUniformLocation(depthFieldProgram.programId(), "focalLength");
       int focalDepthIndex = glGetUniformLocation(depthFieldProgram.programId(), "focalDepth");
       int fStopIndex = glGetUniformLocation(depthFieldProgram.programId(), "fstop");
       glUniform1i(colorIndex, 0);
       glUniform1i(depthIndex, 1);
       glUniform1i(typeIndex, 0);
       glUniform1f(focalLengthIndex, fLength);
       glUniform1f(focalDepthIndex, fDepth);
       glUniform1f(fStopIndex, fStop);
       float vec[2] = {(1.0f/width) * 5, (1.0f/height) * 5};
       glUniform2fv(texCoordIncIndex, 1, &vec[0]);

       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D,deferredTexture);
       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D,depthTexture);

       vao.bind();

       glDrawArrays(GL_TRIANGLES, 0,6);

   }
   //DEPTH OF FIELD-------

   //SSAO
   gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, SSAOFbo);
   glClearColor(0.0f,0.0f,0.0f,1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glBindTexture(GL_TEXTURE_2D, 0);
   glActiveTexture(GL_TEXTURE0);
   if(SSAOProgram.bind())
   {
       //RANDOMS
       std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
       std::default_random_engine generator;
       QVector<QVector3D> ssaoKernel;
       for (unsigned int i = 0; i < 64; ++i)
       {
        QVector3D sample(
        randomFloats(generator) * 2.0 - 1.0,
        randomFloats(generator) * 2.0 - 1.0,
        randomFloats(generator)
        );
        sample = sample.normalized();
        sample *= randomFloats(generator);
        float scale = (float)i / 64.0;
        scale = (1 - scale * scale) * 0.1f + scale * scale * 1.0f;
        sample *= scale;

        ssaoKernel.push_back(sample);
       }

       //
       int colorIndex = glGetUniformLocation(SSAOProgram.programId(), "colorTex");
       int depthIndex = glGetUniformLocation(SSAOProgram.programId(), "depthTex");
       int normalIndex = glGetUniformLocation(SSAOProgram.programId(), "normalTex");
       int samplesIndex = glGetUniformLocation(SSAOProgram.programId(), "samples");
       int projIndex = glGetUniformLocation(SSAOProgram.programId(), "proj");
       int viewIndex = glGetUniformLocation(SSAOProgram.programId(), "view");
       int viewSizeIndex = glGetUniformLocation(SSAOProgram.programId(), "viewportSize");
       int texCoordIncIndex = glGetUniformLocation(SSAOProgram.programId(), "texCoordInc");
       int rotVectorsIndex = glGetUniformLocation(SSAOProgram.programId(), "rotationVectorsTex");
       int radiusIndex = glGetUniformLocation(SSAOProgram.programId(), "radius");

       glUniform1i(colorIndex, 0);
       glUniform1i(depthIndex, 1);
       glUniform1i(normalIndex, 2);
       glUniform1i(rotVectorsIndex, 3);
       glUniform1f(radiusIndex, radius);
       SSAOProgram.setUniformValueArray(samplesIndex, &ssaoKernel[0], 64);
       SSAOProgram.setUniformValue(viewSizeIndex, QVector2D(width, height));
       glUniformMatrix4fv(projIndex,1, GL_FALSE, camera->projection.data());
       glUniformMatrix4fv(viewIndex,1, GL_FALSE, camera->transform.data());

       float vec[2] = {(1.0f/width), (1.0f/height)};
       glUniform2fv(texCoordIncIndex, 1, &vec[0]);

       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D,deferredTexture);
       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D,depthTexture);
       glActiveTexture(GL_TEXTURE2);
       glBindTexture(GL_TEXTURE_2D,normalTexture);
       glActiveTexture(GL_TEXTURE3);
       glBindTexture(GL_TEXTURE_2D,rotationVectors);


       vao.bind();

       glDrawArrays(GL_TRIANGLES, 0,6);

   }
    //BLUR---------------------
     gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, SSAOBlurFbo);
     glClearColor(0.0f,0.0f,0.0f,1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     glBindTexture(GL_TEXTURE_2D, 0);
     glActiveTexture(GL_TEXTURE0);
    if(blurProgram.bind())
    {
        int colorIndex = glGetUniformLocation(blurProgram.programId(), "colorTex");
        int typeIndex  = glGetUniformLocation(blurProgram.programId(), "type");
        int texCoordIncIndex = glGetUniformLocation(blurProgram.programId(), "texCoordInc");
        glUniform1i(colorIndex, 0);
        glUniform1i(typeIndex, 0);
        float vec[2] = {(1.0f/width) * 5, (1.0f/height) * 5};
        glUniform2fv(texCoordIncIndex, 1, &vec[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,SSAO);

        vao.bind();

        glDrawArrays(GL_TRIANGLES, 0,6);

    }
   //SSAO


    //FINAL PASS
    gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
   if(finalProgram.bind())
   {
       int deferredIndex = glGetUniformLocation(finalProgram.programId(), "deferredTex");
       int colorIndex = glGetUniformLocation(finalProgram.programId(), "colorTex");
       int normalsIndex = glGetUniformLocation(finalProgram.programId(), "normalTex");
       int depthIndex = glGetUniformLocation(finalProgram.programId(), "depthTex");
       int SSAOIndex = glGetUniformLocation(finalProgram.programId(), "SSAOTex");
       int SSAOBlurIndex = glGetUniformLocation(finalProgram.programId(), "SSAOBlurTex");
       int depthFieldIndex = glGetUniformLocation(finalProgram.programId(), "depthFieldTex");
       int blurIndex = glGetUniformLocation(finalProgram.programId(), "blurTex");


       int typeIndex  = glGetUniformLocation(finalProgram.programId(), "type");


       glUniform1i(deferredIndex, 0);
       glUniform1i(colorIndex, 1);
       glUniform1i(normalsIndex, 2);
       glUniform1i(depthIndex, 3);
       glUniform1i(SSAOIndex, 4);
       glUniform1i(SSAOBlurIndex, 5);
       glUniform1i(depthFieldIndex, 6);
       glUniform1i(blurIndex, 7);

       glUniform1i(typeIndex, renderType);


       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D,deferredTexture);
       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D,colorTexture);
       glActiveTexture(GL_TEXTURE2);
       glBindTexture(GL_TEXTURE_2D,normalTexture);
       glActiveTexture(GL_TEXTURE3);
       glBindTexture(GL_TEXTURE_2D,depthTexture);
       glActiveTexture(GL_TEXTURE4);
       glBindTexture(GL_TEXTURE_2D,SSAO);
       glActiveTexture(GL_TEXTURE5);
       glBindTexture(GL_TEXTURE_2D,SSAOBlur);
       glActiveTexture(GL_TEXTURE6);
       glBindTexture(GL_TEXTURE_2D,DepthField);
       glActiveTexture(GL_TEXTURE7);
       glBindTexture(GL_TEXTURE_2D,blurTexture);


       vao.bind();

       glDrawArrays(GL_TRIANGLES, 0,6);

   }
    //
       vao.release();
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, 0);

}

void OpenGlWidget::InitializeBuffers()
{
    QOpenGLExtraFunctions* gl_functions = QOpenGLContext::currentContext()->extraFunctions();

    //CORE TEXTURES
       glGenTextures(1, &colorTexture);
       glBindTexture(GL_TEXTURE_2D, colorTexture);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

       glGenTextures(1, &normalTexture);
       glBindTexture(GL_TEXTURE_2D, normalTexture);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

       glGenTextures(1, &depthTexture);
       glBindTexture(GL_TEXTURE_2D, depthTexture);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

       gl_functions->glGenFramebuffers(1, &fbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
       glDrawBuffer(GL_COLOR_ATTACHMENT0);

       GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
       gl_functions->glDrawBuffers(2, buffers);

       //DEFERRED TEXTURE
       glGenTextures(1, &deferredTexture);
       glBindTexture(GL_TEXTURE_2D, deferredTexture);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

       gl_functions->glGenFramebuffers(1, &deferredFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, deferredFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, deferredTexture, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum deferredBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, deferredBuffers);

       //BLUR
       glGenTextures(1, &blurTexture);
       glBindTexture(GL_TEXTURE_2D, blurTexture);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

       gl_functions->glGenFramebuffers(1, &blurFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, blurFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexture, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum blurBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, deferredBuffers);


       //DEPTH OF FIELD
       glGenTextures(1, &DepthField);
       glBindTexture(GL_TEXTURE_2D, DepthField);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);       

       gl_functions->glGenFramebuffers(1, &DepthFieldFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, DepthFieldFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, DepthField, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum DepthFieldsBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, DepthFieldsBuffers);

       //SSAO
       glGenTextures(1, &SSAO);
       glBindTexture(GL_TEXTURE_2D, SSAO);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

       gl_functions->glGenFramebuffers(1, &SSAOFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, SSAOFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAO, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum SSAOBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, SSAOBuffers);

       //SSAO BLUR
       glGenTextures(1, &SSAOBlur);
       glBindTexture(GL_TEXTURE_2D, SSAOBlur);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

       gl_functions->glGenFramebuffers(1, &SSAOBlurFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, SSAOBlurFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAOBlur, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum SSAOBlurBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, SSAOBlurBuffers);


       std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
       std::default_random_engine generator;
       QVector<QVector3D> ssaoNoise;
       for(unsigned int i = 0; i < 16; i++)
       {
           QVector3D noise(
                       randomFloats(generator) * 2.0 - 1.0f,
                       randomFloats(generator) * 2.0 - 1.0f,
                       0.0f);
           ssaoNoise. push_back(noise);
       }
       gl_functions->glGenTextures(1, &rotationVectors);
       gl_functions->glBindTexture(GL_TEXTURE_2D, rotationVectors);
       gl_functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
       gl_functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       gl_functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       gl_functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
       gl_functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


       GLenum status = gl_functions->glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch(status)
        {
        case GL_FRAMEBUFFER_COMPLETE: // Everything's OK
        break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
        qDebug("Framebuffer ERROR: GL_FRAMEBUFFER_UNSUPPORTED"); break;
        default:
        qDebug("Framebuffer ERROR: Unknown ERROR");
        }

}

void OpenGlWidget::keyPressEvent(QKeyEvent *event)
{
    input->keyPressEvent(event);
}

void OpenGlWidget::keyReleaseEvent(QKeyEvent *event)
{
    input->keyReleaseEvent(event);
}

void OpenGlWidget::mousePressEvent(QMouseEvent *event)
{
    input->mousePressEvent(event);
}

void OpenGlWidget::mouseMoveEvent(QMouseEvent *event)
{
    input->mouseMoveEvent(event);
}

void OpenGlWidget::mouseReleaseEvent(QMouseEvent *event)
{
  input->mouseReleaseEvent(event);
}

void OpenGlWidget::enterEvent(QEvent *)
{
    grabKeyboard();
}

void OpenGlWidget::leaveEvent(QEvent *)
{
    releaseKeyboard();
}

void OpenGlWidget::finalizeGL()
{

}

void OpenGlWidget::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    std::cout << debugMessage.severity() << ": " << debugMessage.message().toStdString() << std::endl;
}

void OpenGlWidget::Update()
{
    update();
}
