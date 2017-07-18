#include "SACurvePlotItemSetWidget.h"
#include <QApplication>
#ifdef _USE_PROPERTY_BROWSER
#include "SAQwtPlotItemGroupPropertys.h"
#else
#include <QVBoxLayout>
#include "SAQwtPlotItemVGroupBoxPropertyItem.h"
#endif
#include <functional>
#include <QHash>
#include <QList>
#include "SAChart.h"
#define TR(str) QApplication::translate("SACurvePlotItemSetWidget", str, 0)

#ifdef _USE_PROPERTY_BROWSER
class SACurvePlotItemSetWidget::UI
{
public:

    QwtPlot *chartWidget;
    SACurvePlotItemSetWidget* parentClass;
    QwtPlotItemList plotItemList;

    typedef std::function<void(QwtPlotItem*,QtProperty*,const QVariant&)> FunPtr;
    QHash<QtProperty*,QwtPlotItem*> property2plotItem;
    QHash<QtProperty*,FunPtr> property2FunPtr;
    QHash<QwtPlotItem*,SAQwtPlotItemGroupPropertys> plotItem2Propertys;
    void setupUI(SACurvePlotItemSetWidget* par)
    {
        this->parentClass = par;
        par->connect(par,&SAPropertySetWidget::valueChanged
                     ,par,&SACurvePlotItemSetWidget::onPropertyValueChanged);
    }

    void setPlotItems(const QwtPlotItemList& itemList)
    {
        this->parentClass->deleteAll();
        property2plotItem.clear();
        property2FunPtr.clear();
        plotItem2Propertys.clear();
        for(int i=0;i<itemList.size();++i)
        {
            QwtPlotItem* item = itemList[i];
            if(!SAChart::isPlotCurveItem(item))
            {
                continue;
            }
            plotItem2Propertys[item] = SAQwtPlotItemGroupPropertys();
            plotItem2Propertys[item].isValid = true;
            QtVariantProperty* g = this->parentClass->appendGroup(item->title().text());
            property2plotItem[g] = item;
            plotItem2Propertys[item].topGroupItem = g;
            addPlotItemProperty(item,g);
        }
    }

    void addPlotItemProperty(QwtPlotItem* item,QtVariantProperty* group)
    {
        this->parentClass->setCurrentGroup(group);
        QtVariantProperty* propertyItem = nullptr;
        //Visible
        propertyItem = this->parentClass->appendBoolProperty(TR("Visible"),item->isVisible(),TR("set the plot item is visible"));
        plotItem2Propertys[item].visibleItem = propertyItem;
        property2plotItem[propertyItem] = item;
        property2FunPtr[propertyItem] = [group,this](QwtPlotItem* it,QtProperty* p,const QVariant& v){
            Q_UNUSED(p);
            if(plotItem2Propertys[it].isValid)
            {
                bool b = v.toBool();
                it->setVisible(b);
                plotItem2Propertys[it].titleItem->setEnabled(b);
                QList<QtProperty*>& otherItem = plotItem2Propertys[it].otherItem;
                const int size = otherItem.size();
                for(int i=0;i<size;++i)
                {
                    otherItem[i]->setEnabled(b);
                }
            }
        };
        //Title
        propertyItem = this->parentClass->appendStringProperty(TR("Title"),item->title().text(),TR("set the plot item title"));
        plotItem2Propertys[item].titleItem = propertyItem;
        property2plotItem[propertyItem] = item;
        property2FunPtr[propertyItem] = [group](QwtPlotItem* it,QtProperty* p,const QVariant& v){
            Q_UNUSED(p);
            it->setTitle(v.toString());
            group->setPropertyName(v.toString());
        };

    }
};
#else
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
            SAQwtPlotItemVGroupBoxPropertyItem* group = new SAQwtPlotItemVGroupBoxPropertyItem(item);
            vboxlayout->addWidget(group);
        }
        vboxlayout->addStretch();
    }

};
#endif



#ifdef _USE_PROPERTY_BROWSER
SACurvePlotItemSetWidget::SACurvePlotItemSetWidget(QWidget *par,BrowserType type)
    :SAPropertySetWidget(par,type)
    ,ui(new SACurvePlotItemSetWidget::UI)
{
    ui->setupUI(this);
}
#else
SACurvePlotItemSetWidget::SACurvePlotItemSetWidget(QWidget *par):QWidget(par)
  ,ui(new SACurvePlotItemSetWidget::UI)
{
    ui->setupUI(this);
}
#endif

SACurvePlotItemSetWidget::~SACurvePlotItemSetWidget()
{
    delete ui;
}

void SACurvePlotItemSetWidget::setPlotItems(const QwtPlotItemList &items)
{
    ui->setPlotItems(items);
}
#ifdef _USE_PROPERTY_BROWSER
void SACurvePlotItemSetWidget::onPropertyValueChanged(QtProperty *prop, const QVariant &value)
{
    SACurvePlotItemSetWidget::UI::FunPtr fun = ui->property2FunPtr.value(prop,nullptr);
    if(nullptr != fun)
    {
        QwtPlotItem* plotItem = ui->property2plotItem.value(prop,nullptr);
        fun(plotItem,prop,value);
    }
}
#endif
void SACurvePlotItemSetWidget::plotItemAttached(QwtPlotItem *item, bool on)
{

}


