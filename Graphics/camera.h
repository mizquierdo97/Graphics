#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Camera
{
public:
    Camera();
    void Move();
    void Rotate();
    void Update();

public:
    QVector3D pos;
    QVector3D rot;
    QVector3D scale;
    QVector3D forward;
    QVector3D up;
    QVector3D left;
    QQuaternion quat;
    QMatrix4x4 transform;
    float speed = 5.0f;
};
extern Camera* camera;
#endif // CAMERA_H
