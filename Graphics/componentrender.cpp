#include "componentrender.h"
#include "componenttransform.h"
#include <QWidget>
#include "customwidget.h"
ComponentRender::ComponentRender(Object* _parentObject, QWidget *parent) : Component(_parentObject, parent)
{
    componentType = 1;
    parentObject = _parentObject;
}

void ComponentRender::Render(QPainter * painter)
{
    ComponentTransform* trans = parentObject->GetComponentTransform();

    QColor blueColor = QColor::fromRgb(127,190,220);
    QColor whiteColor = QColor::fromRgb(255,255,255);
    QColor blackColor = QColor::fromRgb(0,0,0);

    QBrush brush;
    QPen pen;

    //painter->drawRect(rect());

    brush.setColor(whiteColor);
    pen.setWidth(4);
    pen.setColor(blackColor);
    pen.setStyle(Qt::PenStyle::DashLine);
    painter->setBrush(brush);
    painter->setPen(pen);

    int r = 64;
    int w = r * 2;
    int h = r * 2;
    int x = trans->pos.x();
    int y = trans->pos.y();

    QRect circleRect(x,y,w,h);
    painter->drawEllipse(circleRect);


}
