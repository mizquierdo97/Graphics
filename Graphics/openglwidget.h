#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>

class OpenGlWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit OpenGlWidget(QWidget * parent = nullptr);

    //From these methods we can call opengl functions
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

signals:
public slots:
};



#endif // OPENGLWIDGET_H
