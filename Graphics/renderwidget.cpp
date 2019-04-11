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

}


void RenderWidget::on_FillButton_clicked()
{

}

void RenderWidget::on_StrokeButton_clicked()
{

}

void RenderWidget::on_Shape_currentIndexChanged(int index)
{

}

void RenderWidget::on_LineStyle_currentIndexChanged(int index)
{

}

void RenderWidget::on_StrokeThickness_valueChanged(double arg1)
{

}
