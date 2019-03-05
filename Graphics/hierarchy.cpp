#include "hierarchy.h"
#include "ui_hierarchy.h"

#include "inspectorwidget.h"
#include "inspector.h"
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLayout>

Hierarchy::Hierarchy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hierarchy)
{
    ui->setupUi(this);
    connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(ui->actionSave_Prohect, SIGNAL(triggered()), this, SLOT(saveProject()));
}

Hierarchy::~Hierarchy()
{
    delete ui;
}

void Hierarchy::addObject()
{
    objectList.push_back();
}

void Hierarchy::deleteObject()
{

}
