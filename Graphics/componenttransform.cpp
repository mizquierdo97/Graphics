#include "componenttransform.h"
#include "scenewidget.h"
ComponentTransform::ComponentTransform(Object* _parentObject, QWidget *parent) :
    Component(_parentObject, parent)
{
    componentType = 0;
    parentObject = _parentObject;

    pos.setX(sceneWidget->rect().width() * 0.5);
    pos.setY(sceneWidget->rect().height() * 0.5);
    scale.setX(100);
    scale.setY(100);
}
