#ifndef SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#define SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#include "SAVCollapsibleGroupBox.h"
class QwtPlotItem;
///
/// \brief 适用于QwtPlotItem的VGroupBox
///
class SAQwtPlotItemVGroupBox : public SAVCollapsibleGroupBox
{
    Q_OBJECT
public:
    enum ShowMode
    {
        ShowSimple///< 显示最简单的设置
        ,ShowAll///<显示所有设置
    };

    SAQwtPlotItemVGroupBox(QwtPlotItem *plotItem, QWidget* par = nullptr, ShowMode mode=ShowAll);
    ~SAQwtPlotItemVGroupBox();

private:
    class UI;
    SAQwtPlotItemVGroupBox::UI* ui;
};

class SAQwtPlotCurveItemVGroupBox : public SAQwtPlotItemVGroupBox
{
    Q_OBJECT
public:
};

#endif // SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
