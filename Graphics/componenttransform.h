#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "component.h"
#include <QVector3D>
#include <QWidget>
#include <QMatrix4x4>


class ComponentTransform : public Component
{
public:
    explicit ComponentTransform(Object* _parentObject, QWidget *parent = nullptr);
    QVector3D pos;
    QVector3D rot;
    QVector3D scale;

    QMatrix4x4 transform;
};

#endif // COMPONENTTRANSFORM_H
