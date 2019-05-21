#include "object.h"
#include "componentrender.h"
#include "componenttransform.h"
#include "componentmesh.h"
#include "scenewidget.h"
#include "openglwidget.h"
#include "camera.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

Object::Object(QWidget *parent) : QWidget(parent)
{
    ComponentTransform* compTransform = new ComponentTransform(this);
    components.push_back(compTransform);

    ComponentMesh* compMesh = new ComponentMesh(this);
    components.push_back(compMesh);
    //compMesh->mesh->loadModel("./models/Patrick.obj");
}

ComponentTransform *Object::GetComponentTransform()
{
    for(int i =0; i< components.size(); i++)
    {
        if(components[i]->componentType == 0)
        {
            return dynamic_cast<ComponentTransform*>(components[i]);
        }
    }
}

ComponentMesh *Object::GetComponentMesh()
{
    for(int i =0; i< components.size(); i++)
    {
        if(components[i]->componentType == 1)
        {
            return dynamic_cast<ComponentMesh*>(components[i]);
        }
    }
}

void Object::Render(int program)
{
    ComponentTransform* compTransform = GetComponentTransform();
    compTransform->transform.setToIdentity();
    compTransform->transform.translate(compTransform->pos);
    compTransform->transform.rotate(QQuaternion::fromEulerAngles(compTransform->rot));
    compTransform->transform.scale(compTransform->scale);

    int index = openGLWidget->glGetUniformLocation(1, "_Projection");
    openGLWidget->glUniformMatrix4fv(index,1, GL_FALSE, camera->projection.data());
    QMatrix4x4 mat = camera->projection;
    
    index = openGLWidget->glGetUniformLocation(1, "_Model");
    openGLWidget->glUniformMatrix4fv(index,1, GL_FALSE, compTransform->transform.data());
    
    index = openGLWidget->glGetUniformLocation(1, "_View");
    openGLWidget->glUniformMatrix4fv(index,1, GL_FALSE, camera->transform.inverted().data());
    
    ComponentMesh* compMesh = GetComponentMesh();
    if(compMesh != nullptr)
        compMesh->Render();
}
