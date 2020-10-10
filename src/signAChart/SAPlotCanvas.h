#ifndef SAPLOTCANVAS_H
#define SAPLOTCANVAS_H
#include "SAChartGlobals.h"
#include "qwt_plot_canvas.h"
class QPaintEvent;
SA_IMPL_FORWARD_DECL(SAPlotCanvas)

/**
 * @brief 重写了paint背景的方法
 */
class SA_CHART_EXPORT SAPlotCanvas : public QwtPlotCanvas
{
    Q_OBJECT
    SA_IMPL(SAPlotCanvas)
public:
    explicit SAPlotCanvas(QwtPlot *p = nullptr);
    virtual ~SAPlotCanvas();
    //获取背景
    QBrush getCanvaBackBrush() const;
    void setCanvaBackBrush(const QBrush& b);

    //设置边框颜色
    QColor getCanvasBorderColor() const;
    void setCanvasBorderColor(const QColor& c);

    //设置qss
    void useQss();

protected:
    void paintEvent(QPaintEvent *e) override;
};

#endif // SAPLOTCANVAS_H
