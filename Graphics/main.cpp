#include "mainwindow.h"
#include "scenewidget.h"
#include "hierarchy.h"
#include <QApplication>

//Global pointers
SceneWidget* sceneWidget = nullptr;
Hierarchy* hierarchy = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
