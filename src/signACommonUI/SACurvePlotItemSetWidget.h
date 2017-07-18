#ifndef SACURVEPLOTITEMSETWIDGET_H
#define SACURVEPLOTITEMSETWIDGET_H
#include "qwt_plot.h"
//#define _USE_PROPERTY_BROWSER
#ifdef _USE_PROPERTY_BROWSER
    #include "SAPropertySetWidget.h"
#else
    #include <QWidget>
#endif
class SACurvePlotItemSetWidget :
        #ifdef _USE_PROPERTY_BROWSER
        public SAPropertySetWidget
        #else
        public QWidget
        #endif
{
public:
#ifdef _USE_PROPERTY_BROWSER
    SACurvePlotItemSetWidget(QWidget* par = nullptr,BrowserType type = TreeType);
#else
    SACurvePlotItemSetWidget(QWidget* par = nullptr);
#endif
    ~SACurvePlotItemSetWidget();
    void setPlotItems(const QwtPlotItemList& items);
#ifdef _USE_PROPERTY_BROWSER
private slots:
    void onPropertyValueChanged(QtProperty* prop,const QVariant& value);
#endif
public slots:
    void plotItemAttached(QwtPlotItem* item, bool on);
private:
    class UI;
    SACurvePlotItemSetWidget::UI* ui;
};

#endif // SACURVEPLOTITEMSETWIDGET_H
