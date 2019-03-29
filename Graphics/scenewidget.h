#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QObject>
#include <QWidget>

class Hierarchy;

class SceneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;


signals:

public slots:

private:
    void paintEvent(QPaintEvent* event) override;
public:
    Hierarchy* hierarchyRef = nullptr;
};

extern SceneWidget* sceneWidget;
#endif // CUSTOMWIDGET_H
