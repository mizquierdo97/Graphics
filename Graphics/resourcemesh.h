#ifndef RESOURCEMESH_H
#define RESOURCEMESH_H

#include "resource.h"
#include "mesh.h"
class ResourceMesh : public Resource
{
public:
    ResourceMesh();

    void Load();
    Mesh* mesh = nullptr;
};

#endif // RESOURCEMESH_H
