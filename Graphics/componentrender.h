#ifndef COMPONENTRENDER_H
#define COMPONENTRENDER_H

#include "component.h"
#include <QObject>
#include <QPainter>

class ComponentRender : public Component
{
public:
    ComponentRender();
public:
    int shape = 0;
    QColor fillColor;
    QColor strokeObject;
    float strokeThickness;
    Qt::PenStyle lineStyle = Qt::PenStyle::SolidLine;
};

#endif // COMPONENTRENDER_H
