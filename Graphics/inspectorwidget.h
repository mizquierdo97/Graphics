#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <QWidget>
#include "object.h"
class InspectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InspectorWidget(QWidget *parent = nullptr);
    ~InspectorWidget();
    void UpdateInspector(Object* selectedObject);
signals:

public slots:

};

#endif // INSPECTORWIDGET_H
