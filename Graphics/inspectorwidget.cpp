#include "inspectorwidget.h"
#include "transformwidget.h"
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLayout>

InspectorWidget::InspectorWidget(QWidget *parent) : QWidget(parent)
{

    transformWidget = new TransformWidget(this);
    QSpacerItem* spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(transformWidget);
    setLayout(layout);
}

InspectorWidget::~InspectorWidget()
{

}

void InspectorWidget::UpdateInspector(Object *selected)
{
    selectedObject = selected;
    transformWidget->Update(selectedObject);

}
