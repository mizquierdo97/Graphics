#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>

namespace Ui {
class RenderWidget;
}

class RenderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    void Update();
private slots:
    void on_FillButton_clicked();

    void on_StrokeButton_clicked();

    void on_Shape_currentIndexChanged(int index);

    void on_LineStyle_currentIndexChanged(int index);

    void on_StrokeThickness_valueChanged(double arg1);

private:
    Ui::RenderWidget *ui;
};

#endif // RENDERWIDGET_H
