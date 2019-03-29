#include "component.h"

Component::Component(Object* _parentObject, QWidget *parent) : QWidget(parent)
{    
    parentObject = _parentObject;
}
