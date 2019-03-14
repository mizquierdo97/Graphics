#ifndef COMPONENT_H
#define COMPONENT_H

#include <QWidget>

class Component : public QWidget
{
    Q_OBJECT
public:
    explicit Component(QWidget *parent = nullptr);
signals:

public slots:
public:
    int componentType = -1;
};

#endif // COMPONENT_H
