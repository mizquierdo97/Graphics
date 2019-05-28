#include "transwidget.h"
#include "ui_transwidget.h"

#include "componenttransform.h"
#include "scenewidget.h"
#include "object.h"
#include "hierarchy.h"

TransWidget::TransWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransWidget)
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

TransWidget::~TransWidget()
{
    delete ui;
}

void TransWidget::Update(Object *selected)
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


}

void TransWidget::UpdateX(double newX)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->pos.setX(newX);

}

void TransWidget::UpdateY(double newY)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->pos.setY(newY);

}

void TransWidget::UpdateZ(double newZ)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->pos.setZ(newZ);

}

void TransWidget::UpdateRotX(double newX)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->rot.setX(newX);

}

void TransWidget::UpdateRotY(double newY)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->rot.setY(newY);
}

void TransWidget::UpdateRotZ(double newZ)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->rot.setZ(newZ);
}

void TransWidget::UpdateScaleX(double newX)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->scale.setX(newX);
}

void TransWidget::UpdateScaleY(double newY)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->scale.setY(newY);
}

void TransWidget::UpdateScaleZ(double newZ)
{
    if(hierarchy->selectedObject == nullptr) return;

    hierarchy->selectedObject->GetComponentTransform()->scale.setZ(newZ);
}
