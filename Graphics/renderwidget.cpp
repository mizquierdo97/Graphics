#include "renderwidget.h"
#include "hierarchy.h"
#include "ui_renderwidget.h"
#include "componentrender.h"
#include <QColorDialog>
#include <QColor>
RenderWidget::RenderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderWidget)
{
    ui->setupUi(this);
}

RenderWidget::~RenderWidget()
{
    delete ui;
}

void RenderWidget::Update()
{
    ComponentRender* rend = hierarchy->selectedObject->GetComponentRender();
    ui->Shape->setCurrentIndex(rend->shape);
    int lineIndex = 0;
    switch(rend->lineStyle)
    {
    case Qt::PenStyle::NoPen:
        lineIndex = 0;
        break;
    case Qt::PenStyle::SolidLine:
        lineIndex = 1;
        break;
    case Qt::PenStyle::DashLine:
        lineIndex = 2;
        break;
    }
    ui->LineStyle->setCurrentIndex(lineIndex);
    ui->StrokeThickness->setValue(rend->strokeThickness);
}


void RenderWidget::on_FillButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    ComponentRender* rend = hierarchy->selectedObject->GetComponentRender();
    if(color.isValid())
        rend->fillColor = color;
     sceneWidget->repaint();
}

void RenderWidget::on_StrokeButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white,this);
    ComponentRender* rend = hierarchy->selectedObject->GetComponentRender();
    if(color.isValid())
        rend->strokeColor = color;
     sceneWidget->repaint();
}

void RenderWidget::on_Shape_currentIndexChanged(int index)
{
    ComponentRender* rend = hierarchy->selectedObject->GetComponentRender();
    rend->shape = index;
    sceneWidget->repaint();
}

void RenderWidget::on_LineStyle_currentIndexChanged(int index)
{
    ComponentRender* rend = hierarchy->selectedObject->GetComponentRender();
    switch(index)
    {
    case 0:
        rend->lineStyle = Qt::PenStyle::NoPen;
        break;
    case 1:
        rend->lineStyle = Qt::PenStyle::SolidLine;
        break;
    case 2:
        rend->lineStyle = Qt::PenStyle::DashLine;
        break;
    }
     sceneWidget->repaint();
}

void RenderWidget::on_StrokeThickness_valueChanged(double arg1)
{
    ComponentRender* rend = hierarchy->selectedObject->GetComponentRender();
    rend->strokeThickness = arg1;
    sceneWidget->repaint();
}
