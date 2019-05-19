#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugMessage>
#include <QTimer>
#include <QWidget>
#include "hierarchy.h"

class OpenGlWidget :
        public QOpenGLWidget,
        public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit OpenGlWidget(QWidget * parent = nullptr);
    ~OpenGlWidget() override;

    //From these methods we can call opengl functions
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void InitializeBuffers();

    //INPUT
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;

public slots:
    void finalizeGL();
    void handleLoggedMessage(const QOpenGLDebugMessage & debugMessage);
    void Update();

public:
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;
    QOpenGLShaderProgram program;
    QOpenGLShaderProgram deferredProgram;

    Hierarchy* hierarchyRef = nullptr;
    QTimer timer;

private:
    GLuint colorTexture;
    GLuint depthTexture;
    GLuint normalTexture;
    GLuint fbo;

    int width = 1000;
    int height = 1000;
};

extern OpenGlWidget* openGLWidget;


#endif // OPENGLWIDGET_H
