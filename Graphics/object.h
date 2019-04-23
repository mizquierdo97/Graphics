#ifndef OBJECT_H
#define OBJECT_H

#include <QWidget>
#include <QList>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugMessage>


class Component;
class ComponentTransform;
class ComponentRender;
class ComponentMesh;
class SceneWidget;

class Object : public QWidget,protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Object(QWidget *parent = nullptr);
    ComponentTransform* GetComponentTransform();
    ComponentMesh* GetComponentMesh();

    void Render(int program);
signals:

public slots:
public:
     QList<Component*> components;
     QString name;
};

#endif // OBJECT_H
