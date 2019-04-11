#include "componentmesh.h"

ComponentMesh::ComponentMesh(Object* _parentObject, QWidget *parent)  : Component(_parentObject, parent)
{

}

void ComponentMesh::Render()
{
    mesh->Render();
}
