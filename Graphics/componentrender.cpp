#include "componentrender.h"
#include "componenttransform.h"
#include <QWidget>
#include "scenewidget.h"
ComponentRender::ComponentRender(Object* _parentObject, QWidget *parent) : Component(_parentObject, parent)
{
    componentType = 1;
    parentObject = _parentObject;
    strokeThickness = 1.0f;
}

void ComponentRender::Render(QPainter * painter)
{
    ComponentTransform* trans = parentObject->GetComponentTransform();

    QBrush brush;
    QPen pen;

    //Change Line Thickness
    if(strokeThickness > 0.0f)
    {
        pen.setWidth(strokeThickness);
        pen.setStyle(lineStyle);
    }
    else {
        pen.setStyle(Qt::PenStyle::NoPen);
    }

    //Change Colors
    brush.setStyle((Qt::BrushStyle::SolidPattern));
    brush.setColor(fillColor);
    pen.setColor(strokeColor);

    painter->setBrush(brush);
    painter->setPen(pen);

    //Transform
    int w = trans->scale.x();
    int h = trans->scale.y();
    int x = trans->pos.x();
    int y = trans->pos.y();
    painter->translate(x, y);
    painter->rotate(trans->rot.x());
    QRect circleRect(-w * 0.5,- h * 0.5 ,w,h);

    //Draw Shape
    switch(shape)
    {
    case 0:

        painter->drawEllipse(circleRect);
        break;
    case 1:
        painter->drawRect(circleRect);
        break;
    }

    painter->rotate(-trans->rot.x());
    painter->translate(-x, -y);
}
