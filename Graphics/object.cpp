#include "object.h"
#include "componentrender.h"
#include "componenttransform.h"

Object::Object(QWidget *parent) : QWidget(parent)
{
    componentTransform* compTransform = new componentTransform();
    components.push_back(compTransform);

    ComponentRender* compRender = new ComponentRender();
    components.push_back(compRender);
}
