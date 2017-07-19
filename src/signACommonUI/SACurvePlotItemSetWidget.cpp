#include "SACurvePlotItemSetWidget.h"
#include <QApplication>

#include <QVBoxLayout>
#include "SAQwtPlotItemVGroupBoxPropertyItem.h"

#include <functional>
#include <QHash>
#include <QList>
#include "SAChart.h"

#define TR(str) QApplication::translate("SACurvePlotItemSetWidget", str, 0)


class SACurvePlotItemSetWidget::UI
{
public:

    QwtPlot *chartWidget;
    SACurvePlotItemSetWidget* parentClass;
    QwtPlotItemList plotItemList;
    QVBoxLayout* vboxlayout;

    void setupUI(SACurvePlotItemSetWidget* par)
    {
        vboxlayout = new QVBoxLayout(par);

    }

    void setPlotItems(const QwtPlotItemList& itemList)
    {
        while(vboxlayout->count())
        {
            QLayoutItem * i = vboxlayout->itemAt(0);
            vboxlayout->removeItem(i);
            QWidget* w = i->widget();
            if(w)
                delete w;
            if(i)
                delete i;
        }
        for(int i=0;i<itemList.size();++i)
        {
            QwtPlotItem* item = itemList[i];
            if(!SAChart::isPlotCurveItem(item))
            {
                continue;
            }
            SAQwtPlotItemVGroupBoxPropertyItem* group = new SAQwtPlotItemVGroupBoxPropertyItem(item,nullptr,SAQwtPlotItemVGroupBoxPropertyItem::ShowSimple);
            vboxlayout->addWidget(group);
        }
        vboxlayout->addStretch();
    }

};





SACurvePlotItemSetWidget::SACurvePlotItemSetWidget(QWidget *par):QWidget(par)
  ,ui(new SACurvePlotItemSetWidget::UI)
{
    ui->setupUI(this);
}

SACurvePlotItemSetWidget::~SACurvePlotItemSetWidget()
{
    delete ui;
}

void SACurvePlotItemSetWidget::setPlotItems(const QwtPlotItemList &items)
{
    ui->setPlotItems(items);
}

void SACurvePlotItemSetWidget::plotItemAttached(QwtPlotItem *item, bool on)
{

}


