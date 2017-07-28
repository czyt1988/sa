#ifndef SACHARTAXESSETWIDGET_H
#define SACHARTAXESSETWIDGET_H

#include <QWidget>

namespace Ui {
class SAChartAxesSetWidget;
}
class SAQwtAxisSetWidget;
class SAChartAxesSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAChartAxesSetWidget(QWidget *parent = 0);
    ~SAChartAxesSetWidget();

private:
    Ui::SAChartAxesSetWidget *ui;
    SAQwtAxisSetWidget* m_xBottomAxisWidget;
    SAQwtAxisSetWidget* m_xTopAxisWidget;
    SAQwtAxisSetWidget* m_yLeftAxisWidget;
    SAQwtAxisSetWidget* m_yRightAxisWidget;
};

#endif // SACHARTAXESSETWIDGET_H
