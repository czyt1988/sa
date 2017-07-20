#ifndef SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#define SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
#include "SAVCollapsibleGroupBox.h"
class QwtPlotItem;
class QwtPlotCurve;
class SAPenSetWidget;





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
    //生产QwtPlotItemVGroupBox的工厂函数
    static SAQwtPlotItemVGroupBox* createQwtPlotItemVGroupBox(QwtPlotItem *plotItem, QWidget* par=nullptr);
private:
    class UI;
    SAQwtPlotItemVGroupBox::UI* ui;
};


///
/// \brief 针对QwtCurveItem的group box
///
class SAQwtPlotCurveItemVGroupBox : public SAQwtPlotItemVGroupBox
{
    Q_OBJECT
public:
    SAQwtPlotCurveItemVGroupBox(QwtPlotCurve *plotItem, QWidget* par = nullptr);
private:
    SAPenSetWidget* m_PenStyle;
};

#endif // SAQWTPLOTITEMVGROUPBOXPROPERTYITEM_H
