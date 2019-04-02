#include "mainwindow.h"
#include "scenewidget.h"
#include "hierarchy.h"
#include <QApplication>
#include <QSurfaceFormat>

//Global pointers
SceneWidget* sceneWidget = nullptr;
Hierarchy* hierarchy = nullptr;

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
