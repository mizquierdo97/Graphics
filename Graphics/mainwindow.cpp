#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_rendering.h"
#include "inspector.h"
#include "inspectorwidget.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uiRendering(new Ui::Rendering)
{
    ui->setupUi(this);

    inspectorWidget = new InspectorWidget();
    ui->Inspector->setWidget(inspectorWidget);
    connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(ui->actionSave_Prohect, SIGNAL(triggered()), this, SLOT(saveProject()));
    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
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
