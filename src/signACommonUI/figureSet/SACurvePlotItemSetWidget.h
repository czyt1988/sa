#ifndef SACURVEPLOTITEMSETWIDGET_H
#define SACURVEPLOTITEMSETWIDGET_H
#include "qwt_plot.h"
#include <QWidget>
///
/// \brief 设置plotCurveItem的窗体
///
class SACurvePlotItemSetWidget : public QWidget
{
public:
    SACurvePlotItemSetWidget(QWidget* par = nullptr);
    ~SACurvePlotItemSetWidget();
    void setPlotItems(const QwtPlotItemList& items);
public slots:
    void plotItemAttached(QwtPlotItem* item, bool on);
private:
    class UI;
    SACurvePlotItemSetWidget::UI* ui;
};

#endif // SACURVEPLOTITEMSETWIDGET_H
