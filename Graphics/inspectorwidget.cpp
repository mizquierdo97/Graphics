#include "inspectorwidget.h"
#include "transformwidget.h"
#include "meshwidget.h"
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLayout>

InspectorWidget::InspectorWidget(QWidget *parent) : QWidget(parent)
{

    transformWidget = new TransformWidget(this);
    QSpacerItem* spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);
    meshWidget = new MeshWidget(this);
    QVBoxLayout* layout = new QVBoxLayout;


    layout->addWidget(transformWidget);
    layout->addWidget(meshWidget);
    setLayout(layout);
}

InspectorWidget::~InspectorWidget()
{

}

void InspectorWidget::UpdateInspector(Object *selected)
{
    selectedObject = selected;
    transformWidget->Update(selectedObject);
    meshWidget->Update(selectedObject);

}
