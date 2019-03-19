#include "transformwidget.h"
#include "ui_inspector.h"

TransformWidget::TransformWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(UpdateX(double)));
    //valuechanged();
}

TransformWidget::~TransformWidget()
{
    delete ui;
}

void TransformWidget::UpdateX(double newX)
{
    int a = 0;
    a = 2;
}
