#ifndef OBJECT_H
#define OBJECT_H

#include <QWidget>

class Component;
class Object : public QWidget
{
    Q_OBJECT
public:
    explicit Object(QWidget *parent = nullptr);

signals:

public slots:
public:
     QList<Component*> components;
};

#endif // OBJECT_H
