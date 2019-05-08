#include "componentmesh.h"
#include "mesh.h"
#include "ui_componentmesh.h"

ComponentMesh::ComponentMesh(Object* _parentObject, QWidget *parent)  :
    Component(_parentObject, parent),
    ui(new Ui::ComponentMesh)
{
    componentType = 1;
    parentObject = _parentObject;
    ui->setupUi(this);
}

void ComponentMesh::UpdateWidget(Object* selectedObject)
{
    int objectIndex = selectedObject->GetComponentMesh()->index;
    ui->comboBox->setCurrentIndex(objectIndex);
}

void ComponentMesh::Render()
{
    if(resourceMesh != nullptr && resourceMesh->mesh != nullptr)
    {
        resourceMesh->mesh->Render();
    }
}

