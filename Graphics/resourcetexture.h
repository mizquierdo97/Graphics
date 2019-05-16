#ifndef RESOURCETEXTURE_H
#define RESOURCETEXTURE_H

#include "resource.h"
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class ResourceTexture : public Resource
{
public:
    ResourceTexture();
    void Load();
public:
    QOpenGLTexture* GLTexture = nullptr;
};

#endif // RESOURCETEXTURE_H
