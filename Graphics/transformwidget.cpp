#include "transformwidget.h"
#include "ui_transformwidget.h"
#include "inspectorwidget.h"
#include "componenttransform.h"
#include "object.h"

TransformWidget::TransformWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);
    connect(ui->PosX, SIGNAL(valueChanged(double)), this, SLOT(UpdateX(double)));
    connect(ui->PosY, SIGNAL(valueChanged(double)), this, SLOT(UpdateY(double)));
    connect(ui->PosZ, SIGNAL(valueChanged(double)), this, SLOT(UpdateZ(double)));
}

TransformWidget::~TransformWidget()
{
    delete ui;
}

void TransformWidget::Update(Object *selected)
{
    ComponentTransform* trans = selected->GetComponentTransform();
    QVector3D position = trans->pos;
    ui->PosX->setValue(position.x());
    ui->PosY->setValue(position.y());
    ui->PosZ->setValue(position.z());
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
