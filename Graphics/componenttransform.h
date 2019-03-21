#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "component.h"
#include <QVector3D>
#include <QWidget>

namespace Ui {
class Inspector;
}

class ComponentTransform : public Component
{
public:
    explicit ComponentTransform(QWidget *parent = nullptr);
    QVector3D pos;
    QVector3D rot;
    QVector3D scale;

};

#endif // COMPONENTTRANSFORM_H
