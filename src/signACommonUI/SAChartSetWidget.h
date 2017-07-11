#ifndef SACHARTSETWIDGET_H
#define SACHARTSETWIDGET_H

#include <QWidget>
#include <QScopedPointer>
class QwtPlot;
///
/// \brief
///
class SAChartSetWidget : public QWidget
{
    Q_OBJECT
public:
    SAChartSetWidget(QWidget* parent = nullptr);
    ~SAChartSetWidget();
    void setChart(QwtPlot *chart);
private:
    class UI;
    SAChartSetWidget::UI* ui;
};

#endif // SAFIGURECANVASSETWIDGET_H
