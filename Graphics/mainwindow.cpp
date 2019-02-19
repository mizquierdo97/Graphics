#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_rendering.h"
#include "inspector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uiRendering(new Ui::Rendering)
{
    ui->setupUi(this);
    tabifyDockWidget(ui->Hierarchy, ui->Project);
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);

    QWidget *renderingWidget = new QWidget();
    uiRendering->setupUi(renderingWidget);
    renderingWidget->show();

    ui->Rendering->setWidget(renderingWidget);

    inspector = new Inspector();
    ui->Inspector->setWidget(inspector);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete uiRendering;
}
