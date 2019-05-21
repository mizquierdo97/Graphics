#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLTexture>

class Material
{
public:
    Material();

public:
    QOpenGLTexture* texture = nullptr;
};

#endif // MATERIAL_H
