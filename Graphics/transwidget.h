#ifndef TRANSWIDGET_H
#define TRANSWIDGET_H

#include <QWidget>

namespace Ui {
class TransWidget;
}
class Object;
class TransWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransWidget(QWidget *parent = nullptr);
    ~TransWidget();

    public slots:
    void Update(Object* selected);
    void UpdateX(double newX);
    void UpdateY(double newY);
    void UpdateZ(double newZ);
    void UpdateRotX(double newX);
    void UpdateRotY(double newY);
    void UpdateRotZ(double newZ);
    void UpdateScaleX(double newX);
    void UpdateScaleY(double newY);
    void UpdateScaleZ(double newZ);

private:
    Ui::TransWidget *ui;
};

#endif // TRANSWIDGET_H
