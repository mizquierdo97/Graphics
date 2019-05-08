#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <QWidget>
#include "object.h"
#include "transformWidget.h"
#include "meshwidget.h"
class InspectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InspectorWidget(QWidget *parent = nullptr);
    ~InspectorWidget();
    void UpdateInspector(Object* selectedObject);
signals:

public slots:

public:
    Object* selectedObject = nullptr;
    TransformWidget *transformWidget = nullptr;
    MeshWidget* meshWidget = nullptr;
};

#endif // INSPECTORWIDGET_H
