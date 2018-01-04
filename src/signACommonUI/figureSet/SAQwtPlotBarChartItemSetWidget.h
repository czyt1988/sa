#ifndef SAQWTPLOTBARCHARTITEMSETWIDGET_H
#define SAQWTPLOTBARCHARTITEMSETWIDGET_H
#include "SAQwtPlotItemSetWidget.h"
#include "qwt_plot_barchart.h"
#include "SACommonUIGlobal.h"
class SAQwtPlotBarChartItemSetWidgetPrivate;
///
/// \brief The SAQwtPlotBarChartItemSetWidget class
///
class SAQwtPlotBarChartItemSetWidget : public SAQwtPlotItemSetWidget
{
    Q_OBJECT
    SA_IMPL(SAQwtPlotBarChartItemSetWidget)
public:
    SAQwtPlotBarChartItemSetWidget(QwtPlotBarChart* bar,QWidget* par = nullptr);
    ~SAQwtPlotBarChartItemSetWidget();
private slots:
    void onSpacingValueChanged(int v);
    void onMarginValueChanged(int v);
    void onFrameStyleComboBoxIndexChanged(int v);
    void onSymbolStyleComboBoxIndexChanged(int v);
    void onLineWidthValueChanged(int v);
};

#endif // SAQWTPLOTBARCHARTITEMSETWIDGET_H
