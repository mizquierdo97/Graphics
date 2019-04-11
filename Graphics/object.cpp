#include "object.h"
#include "componentrender.h"
#include "componenttransform.h"
#include "componentmesh.h"
#include "scenewidget.h"
Object::Object(QWidget *parent) : QWidget(parent)
{
    ComponentTransform* compTransform = new ComponentTransform(this);
    components.push_back(compTransform);

    ComponentMesh* compMesh = new ComponentMesh(this);
    components.push_back(compMesh);
    compMesh->mesh->loadModel("./models/Patrick.obj");
}

ComponentTransform *Object::GetComponentTransform()
{
    for(int i =0; i< components.size(); i++)
    {
        if(components[i]->componentType == 0)
        {
            return dynamic_cast<ComponentTransform*>(components[i]);
        }
    }
}

ComponentMesh *Object::GetComponentMesh()
{
    for(int i =0; i< components.size(); i++)
    {
        if(components[i]->componentType == 1)
        {
            return dynamic_cast<ComponentMesh*>(components[i]);
        }
    }
}

void Object::Render()
{
    ComponentMesh* compMesh = GetComponentMesh();
    if(compMesh != nullptr)
        compMesh->Render();
}
