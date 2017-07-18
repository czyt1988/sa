#include "SAPlotItemSetWidget.h"
#include <QApplication>
#include "qwt_plot_item.h"
#include "qwt_plot.h"
#include <QDebug>
#ifdef SAPlotItemSetWidget_USE_QtPropertyLib_
    #include <functional>
    class GroupPropertyItems
    {
    public:
        GroupPropertyItems()
        {
            isValid = false;
            topGroupItem = visibleItem = titleItem = nullptr;
        }

        bool isValid;
        QtVariantProperty* topGroupItem;
        QtVariantProperty* visibleItem;
        QtVariantProperty* titleItem;
        QList<QtVariantProperty*> otherItem;
    };

#else
    #include <QMap>
    #include "SAQwtPlotItemVGroupBoxPropertyItem.h"
    #include <QVBoxLayout>
    #define T1
#endif

#define TR(str) QApplication::translate("SAPlotItemSetWidget", str, 0)

class SAPlotItemSetWidget::UI
{
public:
    SAPlotItemSetWidget* parentClass;
    QwtPlotItemList plotItemList;
#ifdef SAPlotItemSetWidget_USE_QtPropertyLib_
    typedef std::function<void(QwtPlotItem*,QtProperty*,const QVariant&)> FunPtr;
    QHash<QtProperty*,QwtPlotItem*> property2plotItem;
    QHash<QtProperty*,FunPtr> property2FunPtr;
    QHash<QwtPlotItem*,GroupPropertyItems> plotItem2Propertys;
#else
    QVBoxLayout* vlayout;
    QMap<QwtPlotItem*,SAQwtPlotItemVGroupBoxPropertyItem*> itemToUI;
#endif
    void setupUI(SAPlotItemSetWidget* par)
    {
        this->parentClass = par;
#ifdef SAPlotItemSetWidget_USE_QtPropertyLib_
       par->connect(par,&SAPropertySetWidget::valueChanged
                    ,par,&SAPlotItemSetWidget::onPropertyValueChanged);
#else
        par->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        par->setMinimumSize(150,500);
        par->setObjectName(QStringLiteral("SAPlotItemSetWidget"));
#ifdef T1
        this->vlayout = nullptr;
#else
        this->vlayout = new QVBoxLayout;
#endif
#endif

    }


    void setPlotItemList(const QwtPlotItemList &itemList)
    {
#ifdef SAPlotItemSetWidget_USE_QtPropertyLib_
        this->parentClass->deleteAll();
        property2plotItem.clear();
        property2FunPtr.clear();
        plotItem2Propertys.clear();
        for(int i=0;i<itemList.size();++i)
        {
            QwtPlotItem* item = const_cast<QwtPlotItem*>(itemList[i]);
            plotItem2Propertys[item] = GroupPropertyItems();
            plotItem2Propertys[item].isValid = true;
            QtVariantProperty* g = this->parentClass->appendGroup(item->title().text());
            property2plotItem[g] = item;
            plotItem2Propertys[item].topGroupItem = g;
            addPlotItemProperty(item,g);
        }
#else
#ifdef T1
        if(vlayout)
        {
            delete vlayout;
        }
        this->vlayout = new QVBoxLayout;
        this->vlayout->setSpacing(8);
        this->vlayout->setObjectName(QStringLiteral("vlayout"));
        this->vlayout->setContentsMargins(4, 4, 4, 4);
        this->parentClass->setLayout(vlayout);
        this->plotItemList = itemList;
#else
        while(this->vlayout->count())
        {

        }
#endif
        for(auto i = this->itemToUI.begin();i!=this->itemToUI.end();++i)
        {
            if(nullptr != i.value())
            {
                delete i.value();
            }
        }
        itemToUI.clear();
        for(int i=0;i<itemList.size();++i)
        {
            SAQwtPlotItemVGroupBoxPropertyItem* itemUIPtr = this->createGroupItem(itemList[i]);
            this->itemToUI[const_cast<QwtPlotItem*>(itemList[i])] = itemUIPtr;
            this->vlayout->addWidget(itemUIPtr);
        }
        this->vlayout->addStretch();
        qDebug()<<"---------------\n setLayout before minimumSizeHint"<<this->parentClass->minimumSizeHint()
               <<",SizeHint"<<this->parentClass->sizeHint()
                  ;
        this->vlayout->update();
        this->parentClass->updateGeometry();
        qDebug()<<"setLayout after minimumSizeHint"<<this->parentClass->minimumSizeHint()
               <<",SizeHint"<<this->parentClass->sizeHint()
                  ;
        this->parentClass->setMinimumSize(this->parentClass->minimumSizeHint());
#endif
    }

#ifdef SAPlotItemSetWidget_USE_QtPropertyLib_
    void addPlotItemProperty(const QwtPlotItem* item,QtVariantProperty* group)
    {
        QwtPlotItem* ncitem = const_cast<QwtPlotItem*>(item);
        this->parentClass->setCurrentGroup(group);
        QtVariantProperty* propertyItem = nullptr;
        //Visible
        propertyItem = this->parentClass->appendBoolProperty(TR("Visible"),item->isVisible(),TR("set the plot item is visible"));
        plotItem2Propertys[ncitem].visibleItem = propertyItem;
        property2plotItem[propertyItem] = ncitem;
        property2FunPtr[propertyItem] = [group,this](QwtPlotItem* it,QtProperty* p,const QVariant& v){
            Q_UNUSED(p);
            if(plotItem2Propertys[it].isValid)
            {
                bool b = v.toBool();
                it->setVisible(b);
                plotItem2Propertys[it].titleItem->setEnabled(b);
                QList<QtVariantProperty*>& otherItem = plotItem2Propertys[it].otherItem;
                const int size = otherItem.size();
                for(int i=0;i<size;++i)
                {
                    otherItem[i]->setEnabled(b);
                }
            }
        };
        //Title
        propertyItem = this->parentClass->appendStringProperty(TR("Title"),item->title().text(),TR("set the plot item title"));
        plotItem2Propertys[ncitem].titleItem = propertyItem;
        property2plotItem[propertyItem] = ncitem;
        property2FunPtr[propertyItem] = [group](QwtPlotItem* it,QtProperty* p,const QVariant& v){
            Q_UNUSED(p);
            it->setTitle(v.toString());
            group->setPropertyName(v.toString());
        };

    }

#else
    SAQwtPlotItemVGroupBoxPropertyItem* createGroupItem(const QwtPlotItem* item)
    {
        SAQwtPlotItemVGroupBoxPropertyItem* uiItem = new SAQwtPlotItemVGroupBoxPropertyItem(item,this->parentClass);
        this->itemToUI[const_cast<QwtPlotItem*>(item)] = uiItem;
        return uiItem;
    }
#endif
};

SAPlotItemSetWidget::SAPlotItemSetWidget(QWidget *par)
#ifdef SAPlotItemSetWidget_USE_QtPropertyLib_
    :SAPropertySetWidget(par,GroupBoxType)
#else
    :QWidget(par)
#endif
    ,ui(new SAPlotItemSetWidget::UI)
{
    ui->setupUI(this);
}

SAPlotItemSetWidget::~SAPlotItemSetWidget()
{
    delete ui;
}

void SAPlotItemSetWidget::setPlotItemList(const QwtPlotItemList &itemList)
{
    ui->setPlotItemList(itemList);
}

void SAPlotItemSetWidget::setChart(QwtPlot *chart)
{
    setPlotItemList(chart->itemList());
}

void SAPlotItemSetWidget::onPropertyValueChanged(QtProperty *prop, const QVariant &value)
{
qDebug() << __FUNCTION__;
    SAPlotItemSetWidget::UI::FunPtr fun = ui->property2FunPtr.value(prop,nullptr);
    if(nullptr != fun)
    {
        QwtPlotItem* plotItem = ui->property2plotItem.value(prop,nullptr);
        fun(plotItem,prop,value);
    }
}
