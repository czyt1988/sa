#ifndef SACHARTAXESSETWIDGET_H
#define SACHARTAXESSETWIDGET_H

#include <QWidget>
class SAChart2D;
class QVBoxLayout;
namespace Ui {
class SAChartAxesSetWidget;
}
class SAQwtAxisSetWidget;

///
/// \brief 坐标系设置
///
class SAChartAxesSetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SAChartAxesSetWidget(QWidget *parent = 0);
    ~SAChartAxesSetWidget();
    void setChart(SAChart2D *chart);
    void updateData();
private slots:
    void onEnableAxis(bool enable,int axis);
private:
    Ui::SAChartAxesSetWidget *ui;
    SAQwtAxisSetWidget* m_xBottomAxisWidget;
    SAQwtAxisSetWidget* m_xTopAxisWidget;
    SAQwtAxisSetWidget* m_yLeftAxisWidget;
    SAQwtAxisSetWidget* m_yRightAxisWidget;
    QVBoxLayout* m_layoutXBottom;
    QVBoxLayout* m_layoutXTop;
    QVBoxLayout* m_layoutYLeft;
    QVBoxLayout* m_layoutYRight;
    SAChart2D* m_chart;
};

#endif // SACHARTAXESSETWIDGET_H
