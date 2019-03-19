#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QWidget>

namespace Ui {
class Inspector;
}

class TransformWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransformWidget(QWidget *parent = 0);
    ~TransformWidget();

private:
    Ui::Inspector *ui;
public slots:
    void UpdateX(double newX);
    //void UpdateY(double newY);
    //void UpdateZ(double newZ);
};

#endif // INSPECTOR_H
