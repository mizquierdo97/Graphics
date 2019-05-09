#include "componentmesh.h"
#include "mesh.h"

ComponentMesh::ComponentMesh(Object* _parentObject, QWidget *parent)  :
    Component(_parentObject, parent)
{
    componentType = 1;
    parentObject = _parentObject;
}


void ComponentMesh::Render()
{
    if(resourceMesh != nullptr && resourceMesh->mesh != nullptr)
    {
        resourceMesh->mesh->Render();
    }
}

