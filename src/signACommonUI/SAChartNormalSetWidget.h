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
    void setChart(QwtPlot* chart);
private slots:
    Q_SLOT void onTitleTextChanged(const QString& text);
    Q_SLOT void onFooterTextChanged(const QString& text);
    Q_SLOT void onCanvasBackgroundColorChanged(const QColor& clr);
    Q_SLOT void onBorderRadiusChanged(double v);
private:
    class UI;
    SAChartNormalSetWidget::UI* ui;
};

#endif // SACHARTNORMALSETWIDGET_H
