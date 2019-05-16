#include "resourcetexture.h"
#include <QImage>
#include <QGLWidget>

ResourceTexture::ResourceTexture()
{

}

void ResourceTexture::Load()
{
    QImage img;
    QImage GLimg;
    if(!img.load(path))
    {
        img.fill(1);
    }
    GLTexture = new QOpenGLTexture(QImage(path));

}
