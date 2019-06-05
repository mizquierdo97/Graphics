#include "shaderoptions.h"
#include "ui_shaderoptions.h"
#include "openglwidget.h"

ShaderOptions::ShaderOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShaderOptions)
{
    ui->setupUi(this);

}

ShaderOptions::~ShaderOptions()
{
    delete ui;
}



void ShaderOptions::on_blur_valueChanged(double value)
{
    openGLWidget->blur = value;
}

void ShaderOptions::on_samples_valueChanged(int value)
{
    openGLWidget->samples = value;
}

void ShaderOptions::on_flength_valueChanged(double value)
{
    openGLWidget->fLength = value;
}

void ShaderOptions::on_fdepth_valueChanged(double value)
{
    openGLWidget->fDepth = value;
}

void ShaderOptions::on_fstop_valueChanged(double value)
{
    openGLWidget->fStop = value;
}

void ShaderOptions::on_radius_valueChanged(double value)
{
    openGLWidget->radius = value;
}
