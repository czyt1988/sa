#ifndef SAPLOTITEMSETWIDGET_H
#define SAPLOTITEMSETWIDGET_H
#include "qwt_plot.h"
#include <QWidget>

///
/// \ingroup CommonUI
/// \brief
///
class SAPlotItemSetWidget : public QWidget
{
public:
    explicit SAPlotItemSetWidget(QWidget* par = nullptr);
    ~SAPlotItemSetWidget();
    void setChart(QwtPlot* chart);
private slots:
    void onPlotItemAttached(QwtPlotItem* item, bool on);
private:
    class UI;
    SAPlotItemSetWidget::UI* ui;
};

#endif // SAPLOTITEMSETWIDGET_H
