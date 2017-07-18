#include "SAQwtPlotItemVGroupBoxPropertyItem.h"
#include <QApplication>
#include <QVBoxLayout>
#include "qwt_plot_item.h"
#include "SALineEditPropertyItem.h"
#include "SADoubleSpinBoxPropertyItem.h"
#include "SACheckBoxPropertyItem.h"
#include "SAAxisSelectComboBoxPropertyItem.h"
#define TR(str) QApplication::translate("SAQwtPlotItemVGroupBoxPropertyItem", str, 0)

class SAQwtPlotItemVGroupBoxPropertyItem::UI
{
public:
    QwtPlotItem* itemPtr;
    bool isShowMoreSet;
    SACheckBoxPropertyItem* visibleItem;
    SALineEditPropertyItem* titleItem;
    SADoubleSpinBoxPropertyItem* zValueItem;
    SAAxisSelectComboBoxPropertyItem* xAxisItem;
    SAAxisSelectComboBoxPropertyItem* yAxisItem;
    void setupUI(const QwtPlotItem* plotItem,SAQwtPlotItemVGroupBoxPropertyItem* par)
    {
        par->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        par->setMinimumSize(100,100);
        QLayout* vlayout = par->layout();
        //
        visibleItem = new SACheckBoxPropertyItem(par);
        visibleItem->setObjectName("visible");
        vlayout->addWidget(visibleItem);
        //
        titleItem = new SALineEditPropertyItem(par);
        titleItem->setObjectName("title");
        vlayout->addWidget(titleItem);
        //
        zValueItem = new SADoubleSpinBoxPropertyItem(par);
        zValueItem->setObjectName("z");
        vlayout->addWidget(zValueItem);
        //
        xAxisItem = new SAAxisSelectComboBoxPropertyItem(SAAxisSelectComboBoxPropertyItem::XAxisOnly,par);
        xAxisItem->setObjectName("X Axis");
        vlayout->addWidget(xAxisItem);
        //
        yAxisItem = new SAAxisSelectComboBoxPropertyItem(SAAxisSelectComboBoxPropertyItem::YAxisOnly,par);
        yAxisItem->setObjectName("Y Axis");
        vlayout->addWidget(yAxisItem);
        //
        setItem(plotItem);
        retranslateUi(par);
    }
    void retranslateUi(SAQwtPlotItemVGroupBoxPropertyItem *w)
    {
        w->setTitle(titleItem->getEditText());
        visibleItem->setText(TR("Visible"));
        titleItem->setText(TR("Title"));
        zValueItem->setText(TR("Z Value"));
        xAxisItem->setText(TR("X Axis"));
        yAxisItem->setText(TR("Y Axis"));
    }

    void setItem(const QwtPlotItem* plotItem)
    {
        this->itemPtr = const_cast<QwtPlotItem*>(plotItem);
        titleItem->setEditText(plotItem->title().text());
    }
};



SAQwtPlotItemVGroupBoxPropertyItem::SAQwtPlotItemVGroupBoxPropertyItem(const QwtPlotItem *plotItem, QWidget* par)
    :SAVGroupBoxPropertyItem(par)
    ,ui(new SAQwtPlotItemVGroupBoxPropertyItem::UI)
{
    ui->setupUI(plotItem,this);
}

SAQwtPlotItemVGroupBoxPropertyItem::~SAQwtPlotItemVGroupBoxPropertyItem()
{
    delete ui;
}
