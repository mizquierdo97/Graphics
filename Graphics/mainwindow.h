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
class ShaderOptions;
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

    void ActivateFinalRender();
    void ActivateDeferredRender();
    void ActivateColorRender();
    void ActivateNormalRender();
    void ActivateDepthRender();
    void ActivateSSAORender();
    void ActivateOnlySSAORender();
    void ActivateBlurRender();

public:
    InspectorWidget *inspectorWidget;
private slots:

private:
    Ui::MainWindow *ui;
    Ui::Rendering *uiRendering;
    //Inspector *inspector;

    ShaderOptions* shaderOptions = nullptr;
    Hierarchy *hierarchyWidget = nullptr;
    Object* selectedObject = nullptr;
};
extern MainWindow* mainWindow;

#endif // MAINWINDOW_H
