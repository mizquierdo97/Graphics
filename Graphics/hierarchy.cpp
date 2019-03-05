#include "hierarchy.h"
#include "ui_hierarchy.h"

#include "inspectorwidget.h"
#include "inspector.h"
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLayout>

Hierarchy::Hierarchy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hierarchy)
{
    ui->setupUi(this);
}

Hierarchy::~Hierarchy()
{
    delete ui;
}
