#include "openglwidget.h"
#include <QOpenGLDebugLogger>
#include <iostream>

#pragma comment(lib, "OpenGL32.lib")

OpenGlWidget::OpenGlWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(QSize(256, 256));
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

    //VBO
    QVector3D vertices[] = {
        QVector3D(-0.5, -0.5, 0.0f), QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(0.5, -0.5, 0.0f), QVector3D(0.0f, 1.0f, 0.0f),
        QVector3D(0.0, 0.5, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)
    };
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    vbo.allocate(vertices, 6 * sizeof (QVector3D));


    vao.create();
    vao.bind();
    const GLint compCount = 3;
    const int strideBytes = 2 * sizeof(QVector3D);
    const int offsetBytes0 = 0;
    const int offsetBytes1 = sizeof(QVector3D);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, compCount, GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes0));
    glVertexAttribPointer(1, compCount, GL_FLOAT, GL_FALSE, strideBytes, (void*)(offsetBytes1));

   // vao.release();
    //vbo.release();
    //program.release();
}

void OpenGlWidget::resizeGL(int width, int height)
{

}

void OpenGlWidget::paintGL()
{
    GLenum error;

    do
    {
        error = glGetError();
        if(error != GL_NO_ERROR)
        {

        }
    }
    while (error != GL_NO_ERROR && error != GL_CONTEXT_LOST);

     glClearColor(1.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if(program.bind())
    {
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0,3);
        vao.release();
        program.release();
    }
}

void OpenGlWidget::finalizeGL()
{

}

void OpenGlWidget::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    std::cout << debugMessage.severity() << ": " << debugMessage.message().toStdString() << std::endl;
}
