#ifndef SACHARTSETWIDGET_H
#define SACHARTSETWIDGET_H

#include "SACommonUIGlobal.h"
#include <QWidget>
#include <QScopedPointer>
class QwtPlot;
class SAChart2D;
///
/// \brief
///
class SA_COMMON_UI_EXPORT SAChartSetWidget : public QWidget
{
    Q_OBJECT
public:
    SAChartSetWidget(QWidget *parent = nullptr);
    ~SAChartSetWidget();
    void setChart(SAChart2D *chart);
    SAChart2D *chart() const;
    void updateAll();
    void updatePlotItemsSet();
    void updateAxesSet();
    void updateNormalSet();

signals:
    void chartTitleChanged(SAChart2D *chart, const QString& text);

private slots:
    void onChartDelete(QObject *obj);
    void onPlotItemAttached(QwtPlotItem *item, bool on);

private:
    class UI;
    SAChartSetWidget::UI *ui;
};

#endif // SAFIGURECANVASSETWIDGET_H
