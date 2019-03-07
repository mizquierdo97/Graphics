#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <QWidget>

class Object;
class QListWidgetItem;
namespace Ui {
class Hierarchy;
}

class Hierarchy : public QWidget
{
    Q_OBJECT

public:
    explicit Hierarchy(QWidget *parent = 0);
    ~Hierarchy();

public slots:
    void addObject();
    void deleteObject();
    void selectedItem(QListWidgetItem* item);
private:
    Ui::Hierarchy *ui;

    QList<Object*> objectList;
    Object* selectedObject = nullptr;
};

#endif // HIERARCHY_H
