#ifndef COMPONENTRENDER_H
#define COMPONENTRENDER_H

#include "component.h"
#include <QObject>
#include <QPainter>
#include <QWidget>
class ComponentRender : public Component
{
public:
    explicit ComponentRender(QWidget *parent = nullptr);
public:
    int shape = 0;
    QColor fillColor;
    QColor strokeObject;
    float strokeThickness;
    Qt::PenStyle lineStyle = Qt::PenStyle::SolidLine;
};

#endif // COMPONENTRENDER_H
