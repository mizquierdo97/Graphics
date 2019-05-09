#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include <QWidget>
#include "object.h"
#include "resourcetexture.h"
#include <QVBoxLayout>
#include <QComboBox>

namespace Ui {
class MeshWidget;
}

class MeshWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MeshWidget(QWidget *parent = nullptr);
    ~MeshWidget();
    void Update(Object* selected);
public:
    Ui::MeshWidget *ui;
    QVector<ResourceTexture*> meshTextures;
    QVector<QComboBox*> textureComboBox;
    QVBoxLayout* widgetLayout = nullptr;
private slots:
    void on_comboBox_currentIndexChanged(int index);
};

#endif // MESHWIDGET_H
