#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "component.h"
#include "resourcemesh.h"
#include "ui_componentmesh.h"
#include <QObject>
#include <QPainter>
#include <QWidget>

namespace Ui {
class ComponentMesh;
}

class ComponentMesh : public Component
{
    Q_OBJECT
public:
    explicit ComponentMesh(Object* _parentObject, QWidget *parent = nullptr);

    void UpdateWidget(Object* selected);
    void Render();
signals:

public slots:

public:
    ResourceMesh* resourceMesh = nullptr;    
    Ui::ComponentMesh* ui;
    int index = 0;

};

#endif // COMPONENTMESH_H
