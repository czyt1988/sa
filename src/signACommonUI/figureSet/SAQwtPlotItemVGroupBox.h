#ifndef SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#define SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#include "SAVCollapsibleGroupBox.h"
#include "qwt_symbol.h"
class QwtPlotItem;
class SAQwtPlotItemSetWidget;
///
/// \brief 适用于QwtPlotItem的VGroupBox
///
class SAQwtPlotItemVGroupBox : public SAVCollapsibleGroupBox
{
    Q_OBJECT
public:

    SAQwtPlotItemVGroupBox(QwtPlotItem *plotItem, QWidget* par = nullptr);
    ~SAQwtPlotItemVGroupBox();
    QwtPlotItem *getPlotItem() const;
    SAQwtPlotItemSetWidget* getQwtPlotItemSetWidget() const;
    void updateData();
private:
    class UI;
    SAQwtPlotItemVGroupBox::UI* ui;
};




#endif // SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
