#ifndef SACHARTNORMALSETWIDGET_H
#define SACHARTNORMALSETWIDGET_H
#include <QWidget>

class SAChart2D;
class QwtPlot;
///
/// \ingroup CommonUI
/// \brief qwtPlot的常规设置窗口,包括qwtplot的坐标系设置，以及图标题脚注的设置
///
/// SAChartSetWidget包含SAChartNormalSetWidget窗体。
/// \sa SAChartSetWidget SAPlotItemSetWidget
///
class SAChartNormalSetWidget : public QWidget
{
    Q_OBJECT
public:
    SAChartNormalSetWidget(QWidget* par=nullptr);
    ~SAChartNormalSetWidget();
    void setChart(SAChart2D *chart);
    virtual void retranslateUi();
protected:
    void paintEvent(QPaintEvent *e);
private slots:
    Q_SLOT void onTitleTextChanged(const QString& text);
    Q_SLOT void onFooterTextChanged(const QString& text);
    Q_SLOT void onCanvasBackgroundColorChanged(const QColor& clr);
    Q_SLOT void onBorderRadiusChanged(double v);
signals:
    void chartTitleChanged(const QString& text);
private:
    class UI;
    SAChartNormalSetWidget::UI* ui;
};

#endif // SACHARTNORMALSETWIDGET_H
