#ifndef RESOURCE_H
#define RESOURCE_H

#include <QString>

enum ResourceType
{
    NONE,
    MESH,
    TEXTURE
};

class Resource
{
public:
    Resource();
    virtual void Load() = 0;
public:
    int numUses = 0;
    QString path;
    ResourceType type = NONE;
};

#endif // RESOURCE_H
