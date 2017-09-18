#ifndef SAABSTRACTREGIONSELECTEDITOR_H
#define SAABSTRACTREGIONSELECTEDITOR_H
#include "SAChartGlobals.h"
#include "SAAbstractPlotEditor.h"
#include <QPainterPath>
class QwtPlotShapeItem;
class SAAbstractRegionSelectEditor : public SAAbstractPlotEditor
{
    Q_OBJECT
    Q_ENUMS(SelectionMode)
public:
    SAAbstractRegionSelectEditor(QwtPlot* parent);
    ~SAAbstractRegionSelectEditor();
    enum SelectionMode
    {
        SingleSelection ///< 单选
        ,AndSelection ///< 布尔与运算选择
        ,OrSelection ///< 布尔或运算选择
    };
    SelectionMode getSelectionMode() const;
    void setSelectionMode(const SelectionMode &selectionMode);
    //获取选择的数据区域
    virtual QPainterPath getSelectRegion() const = 0;
    //判断点是否在区域里 此算法频繁调用会耗时
    virtual bool isContains(const QPointF& p) const = 0;
    //判断是否显示选区
    virtual bool isRegionVisible() const = 0;
private:
    SelectionMode m_selectionMode; ///< 选框类型
};

#endif // SAABSTRACTREGIONSELECTEDITOR_H
