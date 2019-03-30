#include "transformwidget.h"
#include "ui_transformwidget.h"
#include "inspectorwidget.h"
#include "componenttransform.h"
#include "scenewidget.h"
#include "object.h"
#include "hierarchy.h"

TransformWidget::TransformWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);
    connect(ui->PosX, SIGNAL(valueChanged(double)), this, SLOT(UpdateX(double)));
    connect(ui->PosY, SIGNAL(valueChanged(double)), this, SLOT(UpdateY(double)));
    connect(ui->PosZ, SIGNAL(valueChanged(double)), this, SLOT(UpdateZ(double)));
    connect(ui->RotX, SIGNAL(valueChanged(double)), this, SLOT(UpdateRotX(double)));
    connect(ui->RotY, SIGNAL(valueChanged(double)), this, SLOT(UpdateRotY(double)));
    connect(ui->RotZ, SIGNAL(valueChanged(double)), this, SLOT(UpdateRotZ(double)));
    connect(ui->ScaleX, SIGNAL(valueChanged(double)), this, SLOT(UpdateScaleX(double)));
    connect(ui->ScaleY, SIGNAL(valueChanged(double)), this, SLOT(UpdateScaleY(double)));
    connect(ui->ScaleZ, SIGNAL(valueChanged(double)), this, SLOT(UpdateScaleZ(double)));
}

TransformWidget::~TransformWidget()
{
    delete ui;
}

void TransformWidget::Update(Object *selected)
{
    ComponentTransform* trans = selected->GetComponentTransform();
    QVector3D position = trans->pos;
    QVector3D rotation = trans->rot;
    QVector3D scale = trans->scale;
    ui->PosX->setValue(position.x());
    ui->PosY->setValue(position.y());
    ui->PosZ->setValue(position.z());
    ui->RotX->setValue(rotation.x());
    ui->RotY->setValue(rotation.y());
    ui->RotZ->setValue(rotation.z());
    ui->ScaleX->setValue(scale.x());
    ui->ScaleY->setValue(scale.y());
    ui->ScaleZ->setValue(scale.z());

    sceneWidget->repaint();
}

void TransformWidget::UpdateX(double newX)
{   
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->pos.setX(newX);
    sceneWidget->repaint();
}

void TransformWidget::UpdateY(double newY)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->pos.setY(newY);
    sceneWidget->repaint();
}

void TransformWidget::UpdateZ(double newZ)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->pos.setZ(newZ);
    sceneWidget->repaint();
}

void TransformWidget::UpdateRotX(double newX)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->rot.setX(newX);
    sceneWidget->repaint();
}

void TransformWidget::UpdateRotY(double newY)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->rot.setY(newY);
    sceneWidget->repaint();
}

void TransformWidget::UpdateRotZ(double newZ)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->rot.setZ(newZ);
    sceneWidget->repaint();
}

void TransformWidget::UpdateScaleX(double newX)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->scale.setX(newX);
    sceneWidget->repaint();
}

void TransformWidget::UpdateScaleY(double newY)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->scale.setY(newY);
    sceneWidget->repaint();
}

void TransformWidget::UpdateScaleZ(double newZ)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->scale.setZ(newZ);
    sceneWidget->repaint();
}
