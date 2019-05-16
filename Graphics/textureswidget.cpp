#include "textureswidget.h"
#include "ui_textureswidget.h"

TexturesWidget::TexturesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TexturesWidget)
{
    ui->setupUi(this);
}

TexturesWidget::~TexturesWidget()
{
    delete ui;
}
