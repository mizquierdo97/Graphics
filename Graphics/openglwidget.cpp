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


        gl_functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearDepth(1.0f);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
         glBindTexture(GL_TEXTURE_2D, 0);
    if(deferredProgram.bind())
    {

           //deferredProgram.setUniformValue(deferredProgram.uniformLocation("tex"), 0);
           //program.setUniformValue(program.uniformLocation("normalMap"), 1);
           //program.setUniformValue(program.uniformLocation("depthMap"), 2);

        vao.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,colorTexture);

        glDrawArrays(GL_TRIANGLES, 0,6);
    }
       vao.release();
       glBindTexture(GL_TEXTURE_2D, 0);
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
