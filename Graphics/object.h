#ifndef OBJECT_H
#define OBJECT_H

#include <QWidget>

class Object : public QWidget
{
    Q_OBJECT
public:
    explicit Object(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // OBJECT_H