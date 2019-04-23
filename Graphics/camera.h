#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera();
    void Move(QVector3D direction);


public:
    QVector3D pos;
    QVector3D rot;
    QVector3D scale;
    QMatrix4x4 transform;
    float speed = 5.0f;
};

#endif // CAMERA_H
