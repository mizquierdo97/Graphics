#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scenewidget.h"
#include "object.h"
namespace Ui {
class MainWindow;
class Rendering;
}
class Inspector;
class InspectorWidget;
class SceneWidget;
class Hierarchy;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void openProject();
    void saveProject();

    void CreateHierarchyWidget();
    Hierarchy* getHierarchyWidget();
public:
    InspectorWidget *inspectorWidget;
private:
    Ui::MainWindow *ui;
    Ui::Rendering *uiRendering;
    //Inspector *inspector;

    Hierarchy *hierarchyWidget;
    Object* selectedObject = nullptr;
};

#endif // MAINWINDOW_H
