#include "SAQwtPlotItemVGroupBox.h"
#include <QApplication>
#include <QVBoxLayout>
#include "SAQwtPlotItemSetWidget.h"
#include "qwt_plot_item.h"
#define TR(str) QApplication::translate("SAQwtPlotItemVGroupBoxPropertyItem", str, 0)
class QwtPlotCurve;
class SAQwtPlotItemVGroupBox::UI
{
public:
    QwtPlotItem* itemPtr;
    SAQwtPlotItemSetWidget* innerWidget;
    void setupUI(QwtPlotItem* plotItem,SAQwtPlotItemVGroupBox* par)
    {
        itemPtr = plotItem;
        QLayout* vlayout = par->layout();
        innerWidget = SAQwtPlotItemSetWidget::createQwtPlotItemSetWidget(plotItem,nullptr);
        par->setTitle(plotItem->title().text());
        par->connect(innerWidget,&SAQwtPlotItemSetWidget::plotItemTitleChanged
                     ,par,[par](const QString& title){
            par->setTitle(title);
        });
        vlayout->addWidget(innerWidget);
    }
};



SAQwtPlotItemVGroupBox::SAQwtPlotItemVGroupBox(QwtPlotItem *plotItem, QWidget* par)
    :SAVCollapsibleGroupBox(par)
    ,ui(new SAQwtPlotItemVGroupBox::UI)
{
    ui->setupUI(plotItem,this);
}

SAQwtPlotItemVGroupBox::~SAQwtPlotItemVGroupBox()
{
    delete ui;
}

QwtPlotItem *SAQwtPlotItemVGroupBox::getPlotItem() const
{
    return ui->itemPtr;
}

SAQwtPlotItemSetWidget *SAQwtPlotItemVGroupBox::getQwtPlotItemSetWidget() const
{
    return ui->innerWidget;
}





