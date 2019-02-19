#include "openglwidget.h"

#pragma comment(lib, "OpenGL32.lib")

OpenGlWidget::OpenGlWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{

}

void OpenGlWidget::initializeGL()
{

}

void OpenGlWidget::resizeGL(int width, int height)
{

}

void OpenGlWidget::paintGL()
{
    glClearColor(1.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
