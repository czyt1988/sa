#ifndef SARECTSELECTEDITOR_H
#define SARECTSELECTEDITOR_H
#include "SAChartGlobals.h"
#include "SAAbstractRegionSelectEditor.h"
#include "qwt_plot.h"
#include "SASelectRegionShapeItem.h"
///
/// \brief 用于给图标添加矩形选框的事件过滤器
///
class SA_CHART_EXPORT SARectSelectEditor : public SAAbstractRegionSelectEditor
{
    Q_OBJECT
    Q_ENUMS(SelectionMode)
public:
    SARectSelectEditor(QwtPlot* parent = 0);
    virtual ~SARectSelectEditor();


    virtual bool eventFilter(QObject *object, QEvent *event);
    virtual void setEnabled( bool on );
    virtual bool isEnabled() const;
    //判断是否显示选区
    virtual bool isRegionVisible() const;
    //获取选择的数据区域
    virtual QPainterPath getSelectRegion() const;
    //判断点是否在区域里 此算法频繁调用会耗时
    virtual bool isContains(const QPointF& p) const;
    //屏幕坐标转换为数据坐标
    QPointF invTransform( const QPoint &pos ) const;
    //设置关联的坐标轴
    void setAxis(int xAxis,int yAxis);
    //获取选框区域的item
    const QwtPlotShapeItem* getShapeItem() const;
    QwtPlotShapeItem* getShapeItem();
    //清理数据
    void clear();
private slots:
    void onItemAttached(QwtPlotItem* item,bool on);
private:
    bool pressed( const QPoint & p);
    bool moved( const QPoint & p);
    void released( const QPoint & p);
private:
    bool m_isEnable;///< 是否生效
    bool m_isStartDrawRegion;
    SASelectRegionShapeItem* m_shapeItem;
    QPointF m_pressedPoint;
    QRectF m_selectedRect;
    int m_xAxis;
    int m_yAxis;
};

#endif // SARECTSELECTEDITOR_H
