#include "componenttransform.h"

ComponentTransform::ComponentTransform(Object* _parentObject, QWidget *parent) :
    Component(_parentObject, parent)
{
    componentType = 0;
    parentObject = _parentObject;
}
