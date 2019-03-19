#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "component.h"
#include <QVector3D>
#include <QWidget>

class componentTransform : public Component
{
public:
    explicit componentTransform(QWidget *parent = nullptr);
    QVector3D pos;
    QVector3D rot;
    QVector3D scale;
};

#endif // COMPONENTTRANSFORM_H
