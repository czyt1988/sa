#ifndef SARECTSELECTEDITOR_H
#define SARECTSELECTEDITOR_H
#include "SAChartGlobals.h"
#include "SAAbstractRegionSelectEditor.h"
#include "qwt_plot.h"
#include "SASelectRegionShapeItem.h"
class QKeyEvent;
///
/// \brief 用于给图标添加矩形选框的事件过滤器
///
class SA_CHART_EXPORT SARectRegionSelectEditor : public SAAbstractRegionSelectEditor
{
    Q_OBJECT
    Q_ENUMS(SelectionMode)
public:
    SARectRegionSelectEditor(QwtPlot* parent = 0);
    virtual ~SARectRegionSelectEditor();


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
    QPointF invTransform( const QPointF &pos ) const;
    //数据坐标转换为屏幕坐标
    QPointF transform( const QPointF &pos ) const;
    //设置关联的坐标轴
    void setAxis(int xAxis,int yAxis);
    //获取选框区域的item
    const QwtPlotShapeItem* getShapeItem() const;
    QwtPlotShapeItem* getShapeItem();
    //清理数据
    void clear();
    //获取绑定的x轴
    int getXAxis() const;
    //获取绑定的y轴
    int getYAxis() const;
    //把当前区域转换为其它轴系
    QPainterPath transformToOtherAxis(int axisX,int axisY);
private slots:
    void onItemAttached(QwtPlotItem* item,bool on);
private:
    bool pressed( const QPoint & p);
    bool moved( const QPoint & p);
    void released( const QPoint & p);
    void keyPressed(const QKeyEvent *e);
private:
    bool m_isEnable;///< 是否生效
    bool m_isStartDrawRegion;
    SASelectRegionShapeItem* m_shapeItem;
    SASelectRegionShapeItem* m_tmpItem;
    QPointF m_pressedPoint;
    QRectF m_selectedRect;
    int m_xAxis;
    int m_yAxis;
};

#endif // SARECTSELECTEDITOR_H
