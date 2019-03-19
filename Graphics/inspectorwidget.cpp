#include "inspectorwidget.h"
#include "transformwidget.h"
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLayout>

InspectorWidget::InspectorWidget(QWidget *parent) : QWidget(parent)
{
   /* //setStyleSheet(QString("background-color: rgb(0,0,0);"));
    Inspector *transformWidget = new Inspector;
    QSpacerItem* spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(transformWidget);    
    //layout->addItem(spacer);

    setLayout(layout);*/
}

InspectorWidget::~InspectorWidget()
{

}

void InspectorWidget::UpdateInspector(Object *selectedObject)
{
    //setStyleSheet(QString("background-color: rgb(0,0,0);"));
       TransformWidget *transformWidget = new TransformWidget;
       QSpacerItem* spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);

       QVBoxLayout* layout = new QVBoxLayout;

       layout->addWidget(transformWidget);
       setLayout(layout);
}
