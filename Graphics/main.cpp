#include "mainwindow.h"
#include "scenewidget.h"
#include "hierarchy.h"
#include "input.h"
#include "camera.h"
#include "resources.h"
#include "openglwidget.h"
#include <QApplication>
#include <QSurfaceFormat>

//Global pointers
MainWindow* mainWindow = nullptr;
SceneWidget* sceneWidget = nullptr;
Hierarchy* hierarchy = nullptr;
OpenGlWidget* openGLWidget = nullptr;
Input* input = nullptr;
Camera* camera = nullptr;
Resources* resources = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Opengl Config
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setMinorVersion(3);
    format.setMajorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
    format.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();

    return a.exec();
}
