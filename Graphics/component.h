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
};

#endif // COMPONENT_H