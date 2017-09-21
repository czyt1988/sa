#ifndef SAPOLYGONREGIONSELECTEDITOR_H
#define SAPOLYGONREGIONSELECTEDITOR_H
#include "SAChartGlobals.h"
#include "SAAbstractRegionSelectEditor.h"
#include "SASelectRegionShapeItem.h"
#include <QPolygonF>
class SA_CHART_EXPORT SAPolygonRegionSelectEditor : public SAAbstractRegionSelectEditor
{
    Q_OBJECT
public:
    SAPolygonRegionSelectEditor(QwtPlot* parent);
    virtual ~SAPolygonRegionSelectEditor();
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
private slots:
    void onItemAttached(QwtPlotItem* item,bool on);
protected:
    bool mousePressEvent(const QMouseEvent *e);
    bool mouseMovedEvent( const QMouseEvent *e);
    bool keyPressEvent(const QKeyEvent *e);
    bool completeRegion();
private:
    bool m_isStartDrawRegion;///< 是否生效
    SASelectRegionShapeItem* m_shapeItem;
    SASelectRegionShapeItem* m_tmpItem;
    QPolygonF m_polygon;///< 多边形
    bool m_isFinishOneRegion;///< 标定是否已经完成了一次区域，m_tmpItem还是m_shapeItem显示
};

#endif // SAPOLYGONREGIONSELECTEDITOR_H
