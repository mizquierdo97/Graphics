#include "inspectorwidget.h"
#include "inspector.h"
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLayout>

InspectorWidget::InspectorWidget(QWidget *parent) : QWidget(parent)
{
    //setStyleSheet(QString("background-color: rgb(0,0,0);"));
    Inspector *transformWidget = new Inspector;
    QSpacerItem* spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(transformWidget);    
    //layout->addItem(spacer);

    setLayout(layout);
}

InspectorWidget::~InspectorWidget()
{

}
