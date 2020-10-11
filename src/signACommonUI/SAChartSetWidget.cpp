#include "SAChartSetWidget.h"
#include "SAColorSetPropertyItem.h"
#include "SASpinBoxPropertyItem.h"
#include <QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QTabWidget>
#include <QTreeWidget>
#include "SAChart.h"
#include "SAChart2D.h"
#include "SAChartNormalSetWidget.h"
#include "SAColorSetPropertyItem.h"
#include <QScrollArea>
#include "SACurvePlotItemSetWidget.h"
#include "SAChartAxesSetWidget.h"
#define TR(str)    QApplication::translate("SAChartSetWidget", str, 0)
class SAChartSetWidget::UI
{
public:

    SAChart2D *chartCtrl;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    SAChartNormalSetWidget *chartNormalSetWidget;
    QScrollArea *tabScrollArea1;
    QScrollArea *tabScrollArea2;
    SACurvePlotItemSetWidget *plotItemsSetWidget;
    SAChartAxesSetWidget *plotAxesSetWidget;
    UI() : chartCtrl(nullptr)
        , verticalLayout(nullptr)
        , tabWidget(nullptr)
        , chartNormalSetWidget(nullptr)
        , tabScrollArea1(nullptr)
        , tabScrollArea2(nullptr)
        , plotItemsSetWidget(nullptr)
        , plotAxesSetWidget(nullptr)
    {
    }


    void setupUI(SAChartSetWidget *par)
    {
        par->setObjectName(QStringLiteral("SAChartSetWidget"));
        par->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        tabWidget = new QTabWidget(par);
        tabWidget->setObjectName(QStringLiteral("SAChartSetWidgetTabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->addWidget(tabWidget);
        par->setLayout(verticalLayout);

        //Tab 1 -- SAChartNormalSetWidget
        tabScrollArea1 = new QScrollArea();
        tabWidget->addTab(tabScrollArea1, "");
        tabWidget->setTabIcon(0, QIcon(":/icon/icons/normalSet.png"));
        tabScrollArea1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tabScrollArea1->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tabScrollArea1->setWidgetResizable(true);
        tabScrollArea1->setBackgroundRole(QPalette::NoRole);
        chartNormalSetWidget = new SAChartNormalSetWidget();
        chartNormalSetWidget->setObjectName(QStringLiteral("ChartNormalSetWidget"));
        tabScrollArea1->setWidget(chartNormalSetWidget);
        par->connect(chartNormalSetWidget, &SAChartNormalSetWidget::chartTitleChanged
            , par, &SAChartSetWidget::chartTitleChanged);
        //Tab 2 -- SAChartAxesSetWidget
        tabScrollArea2 = new QScrollArea();
        tabWidget->addTab(tabScrollArea2, "");
        tabWidget->setTabIcon(1, QIcon(":/icon/icons/axesSet.png"));
        tabScrollArea2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tabScrollArea2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tabScrollArea2->setWidgetResizable(true);
        tabScrollArea2->setBackgroundRole(QPalette::NoRole);
        plotAxesSetWidget = new SAChartAxesSetWidget();
        plotAxesSetWidget->setObjectName(QStringLiteral("PlotAxesSetWidget"));
        tabScrollArea2->setWidget(plotAxesSetWidget);
        //Tab 3 -- SAPlotItemSetWidget
        plotItemsSetWidget = new SACurvePlotItemSetWidget();
        plotItemsSetWidget->setObjectName(QStringLiteral("PlotItemsSetWidget"));
        tabWidget->addTab(plotItemsSetWidget, "");
        tabWidget->setTabIcon(2, QIcon(":/icon/icons/itemSet.png"));
        retranslateUi(par);
    }


    void retranslateUi(QWidget *w)
    {
        w->setWindowTitle(TR("Chart Set Widget"));
        tabWidget->setTabText(0, TR("Normal"));
        tabWidget->setTabText(1, TR("Axes"));
        tabWidget->setTabText(2, TR("Item"));
    } // retranslateUi
};

SAChartSetWidget::SAChartSetWidget(QWidget *parent) : QWidget(parent)
    , ui(new SAChartSetWidget::UI)

{
    ui->setupUI(this);
}


SAChartSetWidget::~SAChartSetWidget()
{
    delete ui;
}


void SAChartSetWidget::setChart(SAChart2D *chart)
{
    if (ui->chartCtrl == chart) {
        return;
    }
    if (ui->chartCtrl && (ui->chartCtrl != chart)) {
        disconnect(ui->chartCtrl, &QObject::destroyed, this, &SAChartSetWidget::onChartDelete);
        disconnect(ui->chartCtrl, &QwtPlot::itemAttached, this, &SAChartSetWidget::onPlotItemAttached);
    }
    ui->chartCtrl = chart;
    if (chart) {
        ui->chartNormalSetWidget->setChart(chart);
        ui->plotAxesSetWidget->setChart(chart);
        ui->plotItemsSetWidget->setPlotItems(chart->itemList());

        connect(chart, &QObject::destroyed, this, &SAChartSetWidget::onChartDelete);
        connect(chart, &QwtPlot::itemAttached, this, &SAChartSetWidget::onPlotItemAttached);
    }
}


SAChart2D *SAChartSetWidget::chart() const
{
    return (ui->chartCtrl);
}


void SAChartSetWidget::updateAll()
{
    updateNormalSet();
    updateAxesSet();
    updatePlotItemsSet();
}


void SAChartSetWidget::updatePlotItemsSet()
{
    ui->plotItemsSetWidget->updateData();
}


void SAChartSetWidget::updateAxesSet()
{
    ui->plotAxesSetWidget->updateData();
}


void SAChartSetWidget::updateNormalSet()
{
    ui->chartNormalSetWidget->updateData();
}


void SAChartSetWidget::onChartDelete(QObject *obj)
{
    Q_UNUSED(obj);
    ui->chartCtrl = nullptr;
    ui->chartNormalSetWidget->setChart(nullptr);
    ui->plotAxesSetWidget->setChart(nullptr);
    ui->plotItemsSetWidget->setPlotItems(QwtPlotItemList());
}


void SAChartSetWidget::onPlotItemAttached(QwtPlotItem *item, bool on)
{
    if (SAChart::checkIsPlotChartItem(item)) {
        ui->plotItemsSetWidget->plotItemAttached(item, on);
    }
}
