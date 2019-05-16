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
    QOpenGLTexture* GLTexture;
};

#endif // RESOURCETEXTURE_H
