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
    //设置选区
    virtual void setSelectRegion(const QPainterPath& shape);
    //判断点是否在区域里 此算法频繁调用会耗时
    virtual bool isContains(const QPointF& p) const;
    //设置选择模式
    virtual void setSelectionMode(const SelectionMode &selectionMode);
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
    void keyPressed(const QKeyEvent *e);
    void keyRelease(const QKeyEvent *e);
private:
    bool m_isEnable;///< 是否生效
    bool m_isStartDrawRegion;
    SASelectRegionShapeItem* m_shapeItem;
    SASelectRegionShapeItem* m_tmpItem;
    QPointF m_pressedPoint;
    QRectF m_selectedRect;

};

#endif // SARECTSELECTEDITOR_H
