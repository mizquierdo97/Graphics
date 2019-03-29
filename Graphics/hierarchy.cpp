#include "hierarchy.h"
#include "ui_hierarchy.h"

#include "inspectorwidget.h"
#include "transformwidget.h"
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLayout>
#include "object.h"
#include <QtGui>

Hierarchy::Hierarchy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hierarchy)
{
    ui->setupUi(this);
    connect(ui->AddObject, SIGNAL(clicked()), this, SLOT(addObject()));
    connect(ui->DeleteObject, SIGNAL(clicked()), this, SLOT(deleteObject()));
    connect(ui->HierarchyList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectedItem(QListWidgetItem*)));
}

Hierarchy::~Hierarchy()
{
    delete ui;
}

void Hierarchy::RenderObjects(QPainter* painter)
{
    for(int i = 0; i < objectList.size(); i++)
    {
        objectList[i]->Render(painter);
    }
}

void Hierarchy::addObject()
{
    Object* newObject = new Object();
    newObject->name = "NewObject";
    objectList.push_back(newObject);
    ui->HierarchyList->addItem("VIVA WILLYREX");
    if(objectList.size() == 1)
        selectedObject = newObject;
}

void Hierarchy::deleteObject()
{

}

void Hierarchy::selectedItem(QListWidgetItem *item)
{
    QModelIndexList indexes = ui->HierarchyList->selectionModel()->selectedIndexes();
    if(indexes.size() <= 0) return;

    QModelIndex modelIndex = indexes[0];
    int index = modelIndex.row();
    selectedObject = objectList[index];
    parentWidget->inspectorWidget->UpdateInspector(selectedObject);
}
