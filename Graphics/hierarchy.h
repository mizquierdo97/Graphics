#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <QWidget>

class Object;
namespace Ui {
class Hierarchy;
}

class Hierarchy : public QWidget
{
    Q_OBJECT

public:
    explicit Hierarchy(QWidget *parent = 0);
    ~Hierarchy();

    void addObject();
    void deleteObject();
private:
    Ui::Hierarchy *ui;

    QList<Object> objectList;
};

#endif // HIERARCHY_H
