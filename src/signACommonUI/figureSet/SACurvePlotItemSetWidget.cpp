#include "SACurvePlotItemSetWidget.h"
#include <QApplication>

#include <QVBoxLayout>
#include "SAQwtPlotItemVGroupBox.h"

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
    QMap<QwtPlotItem*,QWidget*> plotItem2Widget;///< 记录item对应的widget
    QMap<QWidget*,QwtPlotItem*> widget2PlotItem;
    void setupUI(SACurvePlotItemSetWidget* par)
    {
        parentClass = par;
        vboxlayout = new QVBoxLayout(par);

    }

    void setPlotItems(const QwtPlotItemList& itemList)
    {
        while(vboxlayout->count())
        {
            QLayoutItem * i = this->vboxlayout->itemAt(0);
            this->vboxlayout->removeItem(i);
            QWidget* w = i->widget();
            if(w)
            {
                QwtPlotItem* item = this->widget2PlotItem[w];
                this->widget2PlotItem.remove(w);
                this->plotItem2Widget.remove(item);
                delete w;
            }
            if(i)
            {
                delete i;
            }

        }
        for(int i=0;i<itemList.size();++i)
        {
            QwtPlotItem* item = itemList[i];
            this->addPlotItem(item);
        }
        this->vboxlayout->addStretch();
    }

    void removePlotItem(QwtPlotItem *item)
    {
        QWidget* itemWidget = plotItem2Widget.value(item,nullptr);
        if(itemWidget)
        {
            delete itemWidget;
        }
    }
    void addPlotItem(QwtPlotItem *item)
    {
        if(!SAChart::isPlotChartItem(item))
        {
            return;
        }
        SAQwtPlotItemVGroupBox* group = new SAQwtPlotItemVGroupBox(item);
        this->vboxlayout->addWidget(group);
        this->plotItem2Widget[item] = group;
        this->widget2PlotItem[group] = item;
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
    if(!on)
    {
        ui->removePlotItem(item);
    }
    else
    {
        ui->addPlotItem(item);
    }
}


