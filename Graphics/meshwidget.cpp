#include "meshwidget.h"
#include "ui_meshwidget.h"
#include "componentmesh.h"
#include "hierarchy.h"
#include "resources.h"
MeshWidget::MeshWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshWidget)
{
    ui->setupUi(this);
}

MeshWidget::~MeshWidget()
{
    delete ui;
}

void MeshWidget::Update(Object *selected)
{
    int selectedIndex = selected->GetComponentMesh()->index;
    ui->comboBox->setCurrentIndex(selectedIndex);
}

void MeshWidget::on_comboBox_currentIndexChanged(int _index)
{
    if(hierarchy->selectedObject != nullptr)
    {
        ComponentMesh* selectedMesh = hierarchy->selectedObject->GetComponentMesh();
        selectedMesh->index = _index;
        selectedMesh->resourceMesh = resources->meshResources[_index];
    }
}
