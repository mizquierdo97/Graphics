#include "transformwidget.h"
#include "ui_inspector.h"
#include "inspectorwidget.h"
#include "componenttransform.h"
#include "object.h"

TransformWidget::TransformWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);
    connect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(UpdateX(double)));
    connect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(UpdateY(double)));
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(UpdateZ(double)));
    //valuechanged();
}

TransformWidget::~TransformWidget()
{
    delete ui;
}

void TransformWidget::Update(Object *selected)
{
    ComponentTransform* trans = selected->GetComponentTransform();
    QVector3D position = trans->pos;
    ui->doubleSpinBox_2->setValue(position.x());
    ui->doubleSpinBox_3->setValue(position.y());
    ui->doubleSpinBox->setValue(position.z());
    //UpdateX(position.x());
    //UpdateY(position.y());
    //UpdateZ(position.z());
}

void TransformWidget::UpdateX(double newX)
{   

    InspectorWidget* temp = dynamic_cast<InspectorWidget*>(parent());
    temp->selectedObject->GetComponentTransform()->pos.setX(newX);

}

void TransformWidget::UpdateY(double newY)
{
    InspectorWidget* temp = dynamic_cast<InspectorWidget*>(parent());
    temp->selectedObject->GetComponentTransform()->pos.setY(newY);
}

void TransformWidget::UpdateZ(double newZ)
{
    InspectorWidget* temp = dynamic_cast<InspectorWidget*>(parent());
    temp->selectedObject->GetComponentTransform()->pos.setZ(newZ);
}
