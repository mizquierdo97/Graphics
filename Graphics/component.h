#ifndef COMPONENT_H
#define COMPONENT_H

#include <QWidget>
#include "object.h"
class Component : public QWidget
{
    Q_OBJECT
public:
    explicit Component(Object* _parentObject, QWidget *parent = nullptr);
signals:

public slots:
public:
    int componentType = -1;
    Object* parentObject;
};

#endif // COMPONENT_H
