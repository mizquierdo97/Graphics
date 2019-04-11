#include "componentmesh.h"
#include "mesh.h"
ComponentMesh::ComponentMesh(Object* _parentObject, QWidget *parent)  : Component(_parentObject, parent)
{
    componentType = 1;
    parentObject = _parentObject;
    mesh = new Mesh();
}

void ComponentMesh::Render()
{
    if(mesh != nullptr)
    {
        mesh->Render();
    }
}
