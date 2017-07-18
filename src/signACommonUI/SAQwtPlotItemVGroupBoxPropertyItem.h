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
    SAQwtPlotItemVGroupBoxPropertyItem(const QwtPlotItem* plotItem,QWidget* par = nullptr);
    ~SAQwtPlotItemVGroupBoxPropertyItem();
private:
    class UI;
    SAQwtPlotItemVGroupBoxPropertyItem::UI* ui;
};

#endif // SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
