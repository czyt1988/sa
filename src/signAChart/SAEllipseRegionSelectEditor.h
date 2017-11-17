#ifndef SAELLIPSEREGIONSELECTEDITOR_H
#define SAELLIPSEREGIONSELECTEDITOR_H
#include "SAChartGlobals.h"
#include "SAAbstractRegionSelectEditor.h"
#include "SASelectRegionShapeItem.h"
class SAEllipseRegionSelectEditorPrivate;
class SA_CHART_EXPORT SAEllipseRegionSelectEditor : public SAAbstractRegionSelectEditor
{
    Q_OBJECT
    SA_IMPL(SAEllipseRegionSelectEditor)
public:
    SAEllipseRegionSelectEditor(QwtPlot* parent);
    virtual ~SAEllipseRegionSelectEditor();
    //获取选择的数据区域
    virtual QPainterPath getSelectRegion() const;
    //设置选区
    virtual void setSelectRegion(const QPainterPath& shape);
    //设置选择模式
    virtual void setSelectionMode(const SelectionMode &selectionMode);
    //rtti
    virtual int rtti() const;
    //清理数据
    void clear();
private slots:
    void onItemAttached(QwtPlotItem* item,bool on);
protected:
    bool mousePressEvent(const QMouseEvent *e);
    bool mouseMovedEvent( const QMouseEvent *e);
    bool mouseReleasedEvent( const QMouseEvent *e);
};

#endif // SAELLIPSEREGIONSELECTEDITOR_H
