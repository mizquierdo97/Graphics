#ifndef OBJECT_H
#define OBJECT_H

#include <QWidget>
#include <QList>
class Component;
class ComponentTransform;
class ComponentRender;
class CustomWidget;
class Object : public QWidget
{
    Q_OBJECT
public:
    explicit Object(QWidget *parent = nullptr);
    ComponentTransform* GetComponentTransform();
    ComponentRender* GetComponentRender();

    void Render(QPainter* painter);
signals:

public slots:
public:
     QList<Component*> components;
     QString name;
};

#endif // OBJECT_H
