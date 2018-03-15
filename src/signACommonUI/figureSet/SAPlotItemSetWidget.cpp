#include "SAPlotItemSetWidget.h"
#include <QApplication>
#include "qwt_plot_item.h"
#include "qwt_plot.h"
#include <QDebug>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "SACurvePlotItemSetWidget.h"
#include "SAChart2D.h"
#include "SAChart.h"

#define TR(str) QApplication::translate("SAPlotItemSetWidget", str, 0)

class SAPlotItemSetWidget::UI
{
public:
    QwtPlot *chartWidget;
    SAPlotItemSetWidget* parentClass;
    QTabWidget* itemTabWidget;
    QVBoxLayout* verticalLayout;
    //
    QScrollArea* tabScrollArea1;

    SACurvePlotItemSetWidget* curveItemSetWidget;
    UI():chartWidget(nullptr)
      ,parentClass(nullptr)
      ,itemTabWidget(nullptr)
      ,verticalLayout(nullptr)
      ,tabScrollArea1(nullptr)
      ,curveItemSetWidget(nullptr)
    {

    }

    void setupUI(SAPlotItemSetWidget* par)
    {
        this->parentClass = par;
        this->chartWidget = nullptr;
        par->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        par->setMinimumSize(150,200);
        par->setObjectName(QStringLiteral("SAPlotItemSetWidget"));
        verticalLayout = new QVBoxLayout(par);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        itemTabWidget = new QTabWidget;
        itemTabWidget->setObjectName(QStringLiteral("itemTabWidget"));
        itemTabWidget->setTabPosition(QTabWidget::West);

        tabScrollArea1 = new QScrollArea();
        itemTabWidget->addTab(tabScrollArea1,QStringLiteral("1"));
        tabScrollArea1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tabScrollArea1->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tabScrollArea1->setWidgetResizable(true);
        tabScrollArea1->setBackgroundRole(QPalette::NoRole);
        //curveItemSetWidget = new SACurvePlotItemSetWidget(nullptr,SAPropertySetWidget::GroupBoxType);
        curveItemSetWidget = new SACurvePlotItemSetWidget;
        tabScrollArea1->setWidget(curveItemSetWidget);

        verticalLayout->addWidget(itemTabWidget);
        retranslateUi(par);
    }
    void retranslateUi(QWidget *w)
    {
        w->setWindowTitle(TR("Plot Item Set Widget"));
        itemTabWidget->setTabText(0,TR("Curve Set"));
    } // retranslateUi
};

SAPlotItemSetWidget::SAPlotItemSetWidget(QWidget *par)
    :QWidget(par)
    ,ui(new SAPlotItemSetWidget::UI)
{
    ui->setupUI(this);
}

SAPlotItemSetWidget::~SAPlotItemSetWidget()
{
    delete ui;
}


void SAPlotItemSetWidget::setChart(QwtPlot *chart)
{
    if(ui->chartWidget && ui->chartWidget != chart)
    {
        disconnect(ui->chartWidget,&QwtPlot::itemAttached,this,&SAPlotItemSetWidget::onPlotItemAttached);
    }
    if(nullptr == chart)
    {
        ui->curveItemSetWidget->setPlotItems(QwtPlotItemList());
    }
    else if(ui->chartWidget != chart)
    {
        QwtPlotItemList curItems = SAChart::getPlotChartItemList(chart);
        ui->curveItemSetWidget->setPlotItems(curItems);
        connect(chart,&QwtPlot::itemAttached,this,&SAPlotItemSetWidget::onPlotItemAttached);
    }
    ui->chartWidget = chart;

}

void SAPlotItemSetWidget::updateData()
{
    ui->curveItemSetWidget->updateData();
}



void SAPlotItemSetWidget::onPlotItemAttached(QwtPlotItem *item,bool on)
{
    if(nullptr == item)
    {
        return;
    }
    if(nullptr == ui->chartWidget)
    {
        return;
    }

    if(SAChart::checkIsPlotChartItem(item))
    {
        ui->curveItemSetWidget->plotItemAttached(item,on);
    }
}


