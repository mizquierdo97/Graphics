#ifndef RESOURCES_H
#define RESOURCES_H

#include <QMap>

class Resources
{
public:
    Resources();

    void LoadResources(QString path);
public:
    QMap<QString, QString>  meshResources;
    QMap<QString, QString>  textureResources;
};
extern Resources* resources;
#endif // RESOURCES_H
