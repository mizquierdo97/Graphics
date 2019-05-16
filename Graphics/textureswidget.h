#ifndef TEXTURESWIDGET_H
#define TEXTURESWIDGET_H

#include <QWidget>

namespace Ui {
class TexturesWidget;
}

class TexturesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TexturesWidget(QWidget *parent = nullptr);
    ~TexturesWidget();

private:
    Ui::TexturesWidget *ui;
};

#endif // TEXTURESWIDGET_H
