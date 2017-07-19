#ifndef SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#define SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#include "SAVGroupBoxPropertyItem.h"
class QwtPlotItem;
///
/// \brief 适用于QwtPlotItem的VGroupBox
///
class SAQwtPlotItemVGroupBoxPropertyItem : public SAVGroupBoxPropertyItem
{
    Q_OBJECT
public:
    enum ShowMode
    {
        ShowSimple///< 显示最简单的设置
        ,ShowAll///<显示所有设置
    };

    SAQwtPlotItemVGroupBoxPropertyItem(QwtPlotItem *plotItem, QWidget* par = nullptr, ShowMode mode=ShowAll);
    ~SAQwtPlotItemVGroupBoxPropertyItem();

private:
    class UI;
    SAQwtPlotItemVGroupBoxPropertyItem::UI* ui;
};

class SAQwtPlotCurveItemVGroupBox : public SAQwtPlotItemVGroupBoxPropertyItem
{
    Q_OBJECT
public:
};

#endif // SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
