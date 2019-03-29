#include "object.h"
#include "componentrender.h"
#include "componenttransform.h"
#include "scenewidget.h"
Object::Object(QWidget *parent) : QWidget(parent)
{
    ComponentTransform* compTransform = new ComponentTransform(this);
    components.push_back(compTransform);

    ComponentRender* compRender = new ComponentRender(this);
    components.push_back(compRender);
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

ComponentRender *Object::GetComponentRender()
{
    for(int i =0; i< components.size(); i++)
    {
        if(components[i]->componentType == 1)
        {
            return dynamic_cast<ComponentRender*>(components[i]);
        }
    }
}

void Object::Render(QPainter* painter)
{
    ComponentRender* rend = GetComponentRender();
    rend->Render(painter);
}
