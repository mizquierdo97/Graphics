#ifndef OBJECT_H
#define OBJECT_H

#include <QWidget>
#include <QList>
class Component;
class ComponentTransform;
class ComponentRender;

class Object : public QWidget
{
    Q_OBJECT
public:
    explicit Object(QWidget *parent = nullptr);
    ComponentTransform* GetComponentTransform();
    ComponentRender* GetComponentRender();
signals:

public slots:
public:
     QList<Component*> components;
     QString name;
};

#endif // OBJECT_H
