#ifndef SAPOLYGONREGIONSELECTEDITOR_H
#define SAPOLYGONREGIONSELECTEDITOR_H
#include "SAChartGlobals.h"
#include "SAAbstractRegionSelectEditor.h"
#include "SASelectRegionShapeItem.h"
#include <QPolygonF>
class SAPolygonRegionSelectEditorPrivate;
class SA_CHART_EXPORT SAPolygonRegionSelectEditor : public SAAbstractRegionSelectEditor
{
    Q_OBJECT
    SA_IMPL(SAPolygonRegionSelectEditor)
public:
    SAPolygonRegionSelectEditor(QwtPlot* parent);
    virtual ~SAPolygonRegionSelectEditor();
    //获取选择的数据区域
    virtual QPainterPath getSelectRegion() const;
    //设置选区
    virtual void setSelectRegion(const QPainterPath& shape);
    //设置选择模式
    virtual void setSelectionMode(const SelectionMode &selectionMode);
    //rtti
    virtual int rtti() const;
private slots:
    void onItemAttached(QwtPlotItem* item,bool on);
protected:
    bool mousePressEvent(const QMouseEvent *e);
    bool mouseMovedEvent( const QMouseEvent *e);
    bool keyPressEvent(const QKeyEvent *e);
    bool completeRegion();
    bool backspaceRegion();
};

#endif // SAPOLYGONREGIONSELECTEDITOR_H
