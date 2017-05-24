#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}


class SAChart2D;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_SAChart2DWidget_clicked();

private:
    Ui::Widget *ui;
    SAChart2D* m_chart2D;
};

#endif // WIDGET_H
