#ifndef SAPLOTITEMSETWIDGET_H
#define SAPLOTITEMSETWIDGET_H
#include "qwt_plot.h"
#define SAPlotItemSetWidget_USE_QtPropertyLib_
#ifdef SAPlotItemSetWidget_USE_QtPropertyLib_
#include "SAPropertySetWidget.h"
#else
#include <QWidget>
#endif
///
/// \ingroup CommonUI
/// \brief
///
class SAPlotItemSetWidget
        :
#ifdef SAPlotItemSetWidget_USE_QtPropertyLib_
        public SAPropertySetWidget
#else
        public QWidget
#endif
{
public:
    explicit SAPlotItemSetWidget(QWidget* par = nullptr);
    ~SAPlotItemSetWidget();
    void setPlotItemList(const QwtPlotItemList& itemList);
    void setChart(QwtPlot* chart);
private slots:
    void onPropertyValueChanged(QtProperty* prop,const QVariant& value);
private:
    class UI;
    SAPlotItemSetWidget::UI* ui;
};

#endif // SAPLOTITEMSETWIDGET_H
