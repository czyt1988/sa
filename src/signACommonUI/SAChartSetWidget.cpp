#include "SAChartSetWidget.h"
#include "SAColorSetPropertyItem.h"
#include "SASpinBoxPropertyItem.h"
#include <QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QTabWidget>
#include <QTreeWidget>
#include "qwt_plot.h"
#include "SAChartNormalSetWidget.h"
#include "SAColorSetPropertyItem.h"
#include <QScrollArea>
class SAChartSetWidget::UI
{
public:
    QwtPlot* chartCtrl;
    QVBoxLayout *verticalLayout;
    QTabWidget* tabWidget;
    SAChartNormalSetWidget* chartNormalSetWidget;
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

        QScrollArea* sc = new QScrollArea();
        tabWidget->addTab(sc,QStringLiteral("1"));
        sc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sc->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        sc->setWidgetResizable(true);
        sc->setBackgroundRole(QPalette::NoRole);
        chartNormalSetWidget = new SAChartNormalSetWidget();
        sc->setWidget(chartNormalSetWidget);




        par->setLayout(verticalLayout);
        retranslateUi(par);
    }
    void retranslateUi(QWidget *w)
    {
        w->setWindowTitle(QApplication::translate("SAChartSetWidget", "Figure Canvas Set Widget", 0));
        tabWidget->setTabText(0,QApplication::translate("SAChartSetWidget", "normal", 0));
    } // retranslateUi
};

SAChartSetWidget::SAChartSetWidget(QWidget *parent):QWidget(parent)
  ,ui(new UI)

{
    ui->setupUI(this);
}

SAChartSetWidget::~SAChartSetWidget()
{
    delete ui;
}

void SAChartSetWidget::setChart(QwtPlot *chart)
{
    ui->chartCtrl = chart;
    ui->chartNormalSetWidget->setChart(chart);
}
