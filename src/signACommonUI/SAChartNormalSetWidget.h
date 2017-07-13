#ifndef SACHARTNORMALSETWIDGET_H
#define SACHARTNORMALSETWIDGET_H
#include <QTreeWidget>
#include "SAPropertySetWidget.h"
class QwtPlot;
///
/// \brief qwtPlot的常规设置窗口
///
class SAChartNormalSetWidget : public QWidget
{
    Q_OBJECT
public:
    SAChartNormalSetWidget(QWidget* par=nullptr);
    ~SAChartNormalSetWidget();
    void setChart(QwtPlot* chart);
private slots:
    Q_SLOT void onTitleTextChanged(const QString& text);
    Q_SLOT void onFooterTextChanged(const QString& text);
    Q_SLOT void onCanvasBackgroundColorChanged(const QColor& clr);
    Q_SLOT void onBorderRadiusChanged(double v);
    //
    Q_SLOT void onChartDestroy(QObject* o);
    //
    Q_SLOT void onScaleDivChangedXBottom();
    Q_SLOT void onScaleDivChangedXTop();
    Q_SLOT void onScaleDivChangedYLeft();
    Q_SLOT void onScaleDivChangedYRight();
public:
    static void setAxisEnable(QwtPlot*chart, int axisID, bool b);
    static void setAxisTitle(QwtPlot*chart,int axisID,const QString& text);
    static void setAxisFont(QwtPlot*chart,int axisID,const QFont &font);
    static void setAxisLabelRotation(QwtPlot*chart,int axisID,double v);
    static void setAxisScaleMin(QwtPlot*chart,int axisID,double v);
    static void setAxisScaleMax(QwtPlot*chart,int axisID,double v);
    static void setAxisBorderDistStart(QwtPlot*chart,int axisID,int v);
    static void setAxisBorderDistEnd(QwtPlot*chart,int axisID,int v);
    static void setAxisMargin(QwtPlot*chart,int axisID,int v);
    static void setAxisSpacing(QwtPlot*chart,int axisID,int v);
    static void setAxisLabelAlignment(QwtPlot*chart,int axisID,Qt::Alignment v);
signals:
    void chartTitleChanged(const QString& text);
private:
    class UI;
    SAChartNormalSetWidget::UI* ui;
};

#endif // SACHARTNORMALSETWIDGET_H
