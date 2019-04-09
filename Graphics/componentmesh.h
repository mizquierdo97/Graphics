#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "component.h"
#include "mesh.h"
class ComponentMesh : public Component
{
    Q_OBJECT
public:
    explicit ComponentMesh(Object* _parentObject, QWidget *parent = nullptr);

    void Render();
signals:

public slots:

public:
    Mesh* mesh = nullptr;
};

#endif // COMPONENTMESH_H
