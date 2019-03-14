#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "component.h"
#include <QVector3D>

class componentTransform : public Component
{
public:
    componentTransform();
    QVector3D pos;
    QVector3D rot;
    QVector3D scale;
};

#endif // COMPONENTTRANSFORM_H
