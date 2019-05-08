#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include <QWidget>
#include "object.h"
namespace Ui {
class MeshWidget;
}

class MeshWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MeshWidget(QWidget *parent = nullptr);
    ~MeshWidget();
    void Update(Object* selected);
public:
    Ui::MeshWidget *ui;
private slots:
    void on_comboBox_currentIndexChanged(int index);
};

#endif // MESHWIDGET_H
