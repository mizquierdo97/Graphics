#include "openglwidget.h"
#include "input.h"
#include "camera.h"
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <iostream>
#include "qopenglextrafunctions.h"
#include "resources.h"

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

    //Program
    program.create();
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/shader1_vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/shader1_frag");
    program.link();
    program.bind();

    program.release();


    deferredProgram.create();
    deferredProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/deferred_vert");
    deferredProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/deferred_frag");
    deferredProgram.link();

    blurProgram.create();
    blurProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/deferred_vert");
    blurProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/blur_frag");
    blurProgram.link();

    depthFieldProgram.create();
    depthFieldProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/deferred_vert");
    depthFieldProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/blur_frag");
    depthFieldProgram.link();

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
        vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
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
     glClearColor(0.0f,0.0f,0.0f,1.0f);
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

         int colorIndex = glGetUniformLocation(deferredProgram.programId(), "colorTex");
         int normalIndex  = glGetUniformLocation(deferredProgram.programId(), "normalMap");
    if(deferredProgram.bind())
    {
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
     gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, HBlurFbo);
     glClearDepth(1.0f);
     glClearColor(0.0f,0.0f,0.0f,1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     glBindTexture(GL_TEXTURE_2D, 0);
     glActiveTexture(GL_TEXTURE0);
    if(blurProgram.bind())
    {
        colorIndex = glGetUniformLocation(blurProgram.programId(), "colorTex");
        int typeIndex  = glGetUniformLocation(blurProgram.programId(), "type");
        int texCoordIncIndex = glGetUniformLocation(blurProgram.programId(), "texCoordInc");
        glUniform1i(colorIndex, 0);
        glUniform1i(typeIndex, 0);
        float vec[2] = {(1.0f/width) * 5, (1.0f/height) * 5};
        glUniform2fv(texCoordIncIndex, 1, &vec[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,deferredTexture);

        if(normalIndex != -1){
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D,normalTexture);
          }


        vao.bind();

        glDrawArrays(GL_TRIANGLES, 0,6);
    }
    gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearDepth(1.0f);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
   if(blurProgram.bind())
   {
       colorIndex = glGetUniformLocation(blurProgram.programId(), "colorTex");
       int typeIndex  = glGetUniformLocation(blurProgram.programId(), "type");
       int texCoordIncIndex = glGetUniformLocation(blurProgram.programId(), "texCoordInc");
       glUniform1i(colorIndex, 0);
       glUniform1i(typeIndex, 1);
       float vec[2] = {(1.0f/width) * 5, (1.0f/height) * 5};
       glUniform2fv(texCoordIncIndex, 1, &vec[0]);

       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D,HBlur);

       vao.bind();

       glDrawArrays(GL_TRIANGLES, 0,6);
   }

//BLUR----------

   //DEPTH OF FIELD----------
   if(depthFieldProgram.bind())
   {
       colorIndex = glGetUniformLocation(blurProgram.programId(), "colorTex");
       int typeIndex  = glGetUniformLocation(blurProgram.programId(), "type");
       int texCoordIncIndex = glGetUniformLocation(blurProgram.programId(), "texCoordInc");
       glUniform1i(colorIndex, 0);
       glUniform1i(typeIndex, 0);
       float vec[2] = {(1.0f/width) * 5, (1.0f/height) * 5};
       glUniform2fv(texCoordIncIndex, 1, &vec[0]);

       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D,deferredTexture);

       if(normalIndex != -1){
       glActiveTexture(GL_TEXTURE0 + 1);
       glBindTexture(GL_TEXTURE_2D,normalTexture);
         }


       vao.bind();

       glDrawArrays(GL_TRIANGLES, 0,6);
   }

   //DEPTH OF FIELD-------

       vao.release();
       glBindTexture(GL_TEXTURE_2D, 0);
       glActiveTexture(GL_TEXTURE0);
}

void OpenGlWidget::InitializeBuffers()
{
    QOpenGLExtraFunctions* gl_functions = QOpenGLContext::currentContext()->extraFunctions();

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

       glGenTextures(1, &VBlur);
       glBindTexture(GL_TEXTURE_2D, VBlur);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

       glGenTextures(1, &HBlur);
       glBindTexture(GL_TEXTURE_2D, HBlur);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);


       glGenTextures(1, &VDepthField);
       glBindTexture(GL_TEXTURE_2D, VDepthField);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

       glGenTextures(1, &HDepthField);
       glBindTexture(GL_TEXTURE_2D, HDepthField);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

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

       gl_functions->glGenFramebuffers(1, &HBlurFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, HBlurFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, HBlur, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum HBlurBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, HBlurBuffers);

       gl_functions->glGenFramebuffers(1, &VBlurFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, VBlurFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, VBlur, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum VBlurBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, VBlurBuffers);

       gl_functions->glGenFramebuffers(1, &HDepthFieldFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, HDepthFieldFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, HDepthField, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum HDepthFieldsBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, HDepthFieldsBuffers);

       gl_functions->glGenFramebuffers(1, &VDepthFieldFbo);
       gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, VDepthFieldFbo);
       gl_functions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, VDepthField, 0);

       glDrawBuffer(GL_COLOR_ATTACHMENT0);
       GLenum VDepthFieldsBuffers[] = {GL_COLOR_ATTACHMENT0};
       gl_functions->glDrawBuffers(1, VDepthFieldsBuffers);


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
