#ifndef SACHARTNORMALSETWIDGET_H
#define SACHARTNORMALSETWIDGET_H
#include <QWidget>

class SAChart2D;
class SA2DGraph;
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
    void setChart(SAChart2D *chart);
    virtual void retranslateUi();
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
    //other slot in SAChartNormalSetWidget::UI::setupAxisSet function
public:

signals:
    void chartTitleChanged(const QString& text);
private:
    class UI;
    SAChartNormalSetWidget::UI* ui;
};

#endif // SACHARTNORMALSETWIDGET_H
