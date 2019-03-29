#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QObject>
#include <QWidget>

class Hierarchy;

class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;


signals:

public slots:

private:
    void paintEvent(QPaintEvent* event) override;
public:
    Hierarchy* hierarchyRef = nullptr;
};

#endif // CUSTOMWIDGET_H
