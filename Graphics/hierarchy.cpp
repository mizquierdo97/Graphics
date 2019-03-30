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
    hierarchy = this;
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
    QString name = "Object_";
    name += QString::number(objectList.size());
    newObject->name = name;
    objectList.push_back(newObject);
    ui->HierarchyList->addItem(name);
    if(objectList.size() == 1)
    {
        selectedObject = newObject;
        parentWidget->inspectorWidget->UpdateInspector(selectedObject);
    }
    sceneWidget->repaint();
}

void Hierarchy::deleteObject()
{
    QModelIndexList indexes = ui->HierarchyList->selectionModel()->selectedIndexes();
    if(indexes.size() <= 0) return;

    QModelIndex modelIndex = indexes[0];
    int index = modelIndex.row();

    hierarchy->objectList.removeAt(index);

    qDeleteAll(ui->HierarchyList->selectedItems());
    sceneWidget->repaint();
}

void Hierarchy::selectedItem(QListWidgetItem *item)
{
    //Get Selected Index
    QModelIndexList indexes = ui->HierarchyList->selectionModel()->selectedIndexes();
    if(indexes.size() <= 0) return;

    QModelIndex modelIndex = indexes[0];
    int index = modelIndex.row();
    selectedObject = objectList[index];

    //Update Inspector
    parentWidget->inspectorWidget->UpdateInspector(selectedObject);
}
