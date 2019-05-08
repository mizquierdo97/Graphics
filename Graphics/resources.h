#ifndef RESOURCES_H
#define RESOURCES_H

#include <QVector>
#include "resourcemesh.h"
#include "resourcetexture.h"
class Resources
{
public:
    Resources();

    void LoadResources(QString path);
public:
    QVector<ResourceMesh*>  meshResources;
     QVector<ResourceTexture*>  textureResources;
};
extern Resources* resources;
#endif // RESOURCES_H
