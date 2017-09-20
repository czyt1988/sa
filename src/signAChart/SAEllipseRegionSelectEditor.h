#ifndef SAELLIPSEREGIONSELECTEDITOR_H
#define SAELLIPSEREGIONSELECTEDITOR_H
#include "SAChartGlobals.h"
#include "SAAbstractRegionSelectEditor.h"
#include "qwt_plot.h"
#include "SASelectRegionShapeItem.h"
class SA_CHART_EXPORT SAEllipseRegionSelectEditor : public SAAbstractRegionSelectEditor
{
    Q_OBJECT
public:
    SAEllipseRegionSelectEditor(QwtPlot* parent);
    virtual ~SAEllipseRegionSelectEditor();
    //判断是否显示选区
    virtual bool isRegionVisible() const;
    //获取选择的数据区域
    virtual QPainterPath getSelectRegion() const;
    //设置选区
    virtual void setSelectRegion(const QPainterPath& shape);
    //设置选择模式
    virtual void setSelectionMode(const SelectionMode &selectionMode);
    //获取选框区域的item
    const QwtPlotShapeItem* getShapeItem() const;
    QwtPlotShapeItem* getShapeItem();
    //清理数据
    void clear();
private slots:
    void onItemAttached(QwtPlotItem* item,bool on);
protected:
    bool mousePressEvent(const QMouseEvent *e);
    bool mouseMovedEvent( const QMouseEvent *e);
    bool mouseReleasedEvent( const QMouseEvent *e);
private:
    bool m_isEnable;///< 是否生效
    bool m_isStartDrawRegion;
    SASelectRegionShapeItem* m_shapeItem;
    SASelectRegionShapeItem* m_tmpItem;
    QPointF m_pressedPoint;
    QRectF m_selectedRect;
};

#endif // SAELLIPSEREGIONSELECTEDITOR_H
