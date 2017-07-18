#include "SAChartSetWidget.h"
#include "SAColorSetPropertyItem.h"
#include "SASpinBoxPropertyItem.h"
#include <QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QTabWidget>
#include <QTreeWidget>
#include "SAChart2D.h"
#include "SAChartNormalSetWidget.h"
#include "SAColorSetPropertyItem.h"
#include <QScrollArea>
#include "SAPlotItemSetWidget.h"
#define TR(str) QApplication::translate("SAChartSetWidget",str, 0)
class SAChartSetWidget::UI
{
public:
    SAChart2D* chartCtrl;
    QVBoxLayout *verticalLayout;
    QTabWidget* tabWidget;
    SAChartNormalSetWidget* chartNormalSetWidget;
    QScrollArea* tabScrollArea1;
    SAPlotItemSetWidget* plotItemsSetWidget;
    QScrollArea* tabScrollArea2;
    void setupUI(SAChartSetWidget* par)
    {
        par->setObjectName(QStringLiteral("SAChartSetWidget"));
        par->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        tabWidget = new QTabWidget(par);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        verticalLayout = new QVBoxLayout;
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->addWidget(tabWidget);
        par->setLayout(verticalLayout);

        //Tab 1
        tabScrollArea1 = new QScrollArea();
        tabWidget->addTab(tabScrollArea1,QStringLiteral("1"));
        tabScrollArea1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tabScrollArea1->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tabScrollArea1->setWidgetResizable(true);
        tabScrollArea1->setBackgroundRole(QPalette::NoRole);
        chartNormalSetWidget = new SAChartNormalSetWidget();
        tabScrollArea1->setWidget(chartNormalSetWidget);
        par->connect(chartNormalSetWidget,&SAChartNormalSetWidget::chartTitleChanged
                     ,par,&SAChartSetWidget::chartTitleChanged);

        //Tab 2
        tabScrollArea2 = new QScrollArea();
        tabWidget->addTab(tabScrollArea2,QStringLiteral("2"));
        tabScrollArea2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tabScrollArea2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tabScrollArea2->setWidgetResizable(true);
        tabScrollArea2->setBackgroundRole(QPalette::NoRole);
        plotItemsSetWidget = new SAPlotItemSetWidget();
        tabScrollArea2->setWidget(plotItemsSetWidget);
        retranslateUi(par);
    }
    void retranslateUi(QWidget *w)
    {
        w->setWindowTitle(TR("Figure Canvas Set Widget"));
        tabWidget->setTabText(0,TR("Chart Normal Set"));
        tabWidget->setTabText(1,TR("Chart Item Set"));
    } // retranslateUi
};

SAChartSetWidget::SAChartSetWidget(QWidget *parent):QWidget(parent)
  ,ui(new SAChartSetWidget::UI)

{
    ui->setupUI(this);
}

SAChartSetWidget::~SAChartSetWidget()
{
    delete ui;
}

void SAChartSetWidget::setChart(SAChart2D *chart)
{
    ui->chartCtrl = chart;
    ui->chartNormalSetWidget->setChart(chart);
    ui->plotItemsSetWidget->setChart(chart);
}
