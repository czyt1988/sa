#ifndef SARECTSELECTEDITOR_H
#define SARECTSELECTEDITOR_H
#include "SAChartGlobals.h"
#include "SAAbstractRegionSelectEditor.h"
#include "SASelectRegionShapeItem.h"
class SARectRegionSelectEditorPrivate;
class QKeyEvent;
///
/// \brief 用于给图标添加矩形选框的事件过滤器
///
class SA_CHART_EXPORT SARectRegionSelectEditor : public SAAbstractRegionSelectEditor
{
    Q_OBJECT
    SA_IMPL(SARectRegionSelectEditor)
public:
    SARectRegionSelectEditor(QwtPlot* parent);
    virtual ~SARectRegionSelectEditor();
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
    bool keyPressEvent(const QKeyEvent *e);
    bool keyReleaseEvent(const QKeyEvent *e);
};

#endif // SARECTSELECTEDITOR_H
