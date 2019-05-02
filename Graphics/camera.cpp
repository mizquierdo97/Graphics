#include "camera.h"
#include "input.h"

Camera::Camera()
{
    camera = this;
}

void Camera::Move()
{
    if(input->keys[Qt::Key_W] == KeyState::Down)
    {
        pos += forward * speed * 0.16666f;
    }
    if(input->keys[Qt::Key_S] == KeyState::Down)
    {
        pos -= forward * speed * 0.16666f;
    }
    if(input->keys[Qt::Key_A] == KeyState::Down)
    {
        pos += left * speed * 0.16666f;
    }
    if(input->keys[Qt::Key_D] == KeyState::Down)
    {
        pos -= left * speed * 0.16666f;
    }
}

void Camera::Rotate()
{
    quat = QQuaternion::fromEulerAngles(rot);
    float x = quat.toVector4D().x();
    float y = quat.toVector4D().y();
    float z = quat.toVector4D().z();
    float w = quat.toVector4D().w();

    forward = QVector3D( 2 * (x*z + w*y),2 * (y*z - w*x),1 - 2 * (x*x + y*y));
    up = QVector3D(2 * (x*y - w*z),1 - 2 * (x*x + z*z),2 * (y*z + w*x));
    left = QVector3D(1 - 2 * (y*y + z*z),2 * (x*y + w*z),2 * (x*z - w*y));
}

void Camera::Update()
{
    Move();
    Rotate();

    projection.setToIdentity();
    projection.perspective(60, 16/9, nearPlane, farPlane);

    transform.setToIdentity();
    transform.translate(pos);
    transform.rotate(QQuaternion::fromEulerAngles(rot));
}
