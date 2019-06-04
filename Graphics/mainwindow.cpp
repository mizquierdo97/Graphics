#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_rendering.h"
#include "transformwidget.h"
#include "inspectorwidget.h"
#include "input.h"
#include "camera.h"
#include "resources.h"
#include "meshwidget.h"
#include "ui_meshwidget.h"
#include <QComboBox>

#include <iostream>
#include "hierarchy.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uiRendering(new Ui::Rendering)
{
    mainWindow = this;
    ui->setupUi(this);

    inspectorWidget = new InspectorWidget();
    ui->Inspector->setWidget(inspectorWidget);
    
    input = new Input();
    camera = new Camera();
    resources = new Resources();
    CreateHierarchyWidget();

    ResourceMesh* newMesh = new ResourceMesh();
    newMesh->path = "NONE";
    newMesh->name = "NONE";
    resources->meshResources.push_back(newMesh);
    inspectorWidget->meshWidget->ui->comboBox->addItem("NONE");
    resources->LoadResources("./assets");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiRendering;
}

void MainWindow::openProject()
{
    std::cout << "Open project" << std::endl;
}

void MainWindow::saveProject()
{
    std::cout << "Save project" << std::endl;
}

void MainWindow::CreateHierarchyWidget()
{
    hierarchyWidget = new Hierarchy();
    ui->Hierarchy->setWidget(hierarchyWidget);
    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionFINAL, SIGNAL(triggered()), this, SLOT(ActivateFinalRender()));
    connect(ui->actionDeferred_SSAO, SIGNAL(triggered()), this, SLOT(ActivateSSAORender()));
    connect(ui->actionDeferred, SIGNAL(triggered()), this, SLOT(ActivateDeferredRender()));
    connect(ui->actionColor, SIGNAL(triggered()), this, SLOT(ActivateColorRender()));
    connect(ui->actionNormal, SIGNAL(triggered()), this, SLOT(ActivateNormalRender()));
    connect(ui->actionDepth, SIGNAL(triggered()), this, SLOT(ActivateDepthRender()));
    connect(ui->actionSSAO_only, SIGNAL(triggered()), this, SLOT(ActivateOnlySSAORender()));
    connect(ui->actionBlur, SIGNAL(triggered()), this, SLOT(ActivateBlurRender()));
    hierarchyWidget->parentWidget = this;
}

Hierarchy *MainWindow::getHierarchyWidget()
{
    return hierarchyWidget;
}

void MainWindow::ActivateFinalRender()
{
    openGLWidget->renderType = 0;
}

void MainWindow::ActivateDeferredRender()
{
    openGLWidget->renderType = 1;
}

void MainWindow::ActivateColorRender()
{
    openGLWidget->renderType = 2;
}

void MainWindow::ActivateNormalRender()
{
    openGLWidget->renderType = 3;
}

void MainWindow::ActivateDepthRender()
{
    openGLWidget->renderType = 4;
}

void MainWindow::ActivateSSAORender()
{
    openGLWidget->renderType = 5;
}

void MainWindow::ActivateOnlySSAORender()
{
    openGLWidget->renderType = 6;
}

void MainWindow::ActivateBlurRender()
{
    openGLWidget->renderType = 7;
}
