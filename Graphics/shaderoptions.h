#ifndef SHADEROPTIONS_H
#define SHADEROPTIONS_H

#include <QWidget>

namespace Ui {
class ShaderOptions;
}

class ShaderOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ShaderOptions(QWidget *parent = nullptr);
    ~ShaderOptions();

private slots:
    void on_blur_valueChanged(double arg1);

    void on_samples_valueChanged(int arg1);

    void on_flength_valueChanged(double arg1);

    void on_fdepth_valueChanged(double arg1);

    void on_fstop_valueChanged(double arg1);

    void on_radius_valueChanged(double arg1);

private:
    Ui::ShaderOptions *ui;
};

#endif // SHADEROPTIONS_H
