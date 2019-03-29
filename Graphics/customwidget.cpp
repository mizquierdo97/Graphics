#include "customwidget.h"
#include <QPainter>
#include "hierarchy.h"

CustomWidget::CustomWidget(QWidget * _parent) : QWidget(_parent)
{
    setAutoFillBackground(true);
}

QSize CustomWidget::sizeHint() const
{
    return QSize(256,256);
}

QSize CustomWidget::minimumSizeHint() const
{
    return QSize(64,64);
}

void CustomWidget::paintEvent(QPaintEvent *event)
{
    if(hierarchyRef == nullptr)
    {
         MainWindow* mainWindowRef = dynamic_cast<MainWindow*>( parent()->parent());
        hierarchyRef = mainWindowRef->getHierarchyWidget();
    }
    QColor blueColor = QColor::fromRgb(127,190,220);
    QColor whiteColor = QColor::fromRgb(255,255,255);
    QColor blackColor = QColor::fromRgb(0,0,0);


    QPainter painter(this);
    QBrush brush;
       QPen pen;

       brush.setColor(blueColor);
       brush.setStyle(Qt::BrushStyle::SolidPattern);
       pen.setStyle(Qt::PenStyle::NoPen);
       painter.setBrush(brush);
       painter.setPen(pen);

       painter.drawRect(rect());

    hierarchyRef->RenderObjects(&painter);

}
