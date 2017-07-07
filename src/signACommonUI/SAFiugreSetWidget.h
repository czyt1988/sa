#ifndef SAFIUGRESETWIDGET_H
#define SAFIUGRESETWIDGET_H

#include <QWidget>

namespace Ui {
class SAFiugreSetWidget;
}

class SAFiugreSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAFiugreSetWidget(QWidget *parent = 0);
    ~SAFiugreSetWidget();

private:
    Ui::SAFiugreSetWidget *ui;
};

#endif // SAFIUGRESETWIDGET_H
