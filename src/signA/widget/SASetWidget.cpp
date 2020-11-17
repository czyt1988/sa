#include "SASetWidget.h"
#include "ui_SASetWidget.h"
#include "qwt_plot_item.h"
#include "SAChart2D.h"
#define ICON_FIGSET    QIcon(":/icons/icons/figureSet.svg")

SASetWidgetTabBar::SASetWidgetTabBar(QWidget *p) : QTabBar(p)
{
}


SASetWidgetTabBar::~SASetWidgetTabBar()
{
}


SASetWidgetTab::SASetWidgetTab(QWidget *p) : QTabWidget(p)
{
    SASetWidgetTabBar *tabbar = new SASetWidgetTabBar(this);

    tabbar->setObjectName(QStringLiteral("SASetWidgetTabBar"));
    setTabBar(tabbar);
}


SASetWidgetTab::~SASetWidgetTab()
{
}


SASetWidget::SASetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SASetWidget)
{
    ui->setupUi(this);
    QScrollArea *s1 = new QScrollArea();

    m_figureSetWidget = new SAFigureSetWidget(ui->tabWidget);
    s1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    s1->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    s1->setWidgetResizable(true);
    s1->setBackgroundRole(QPalette::NoRole);
    s1->setWidget(m_figureSetWidget);
    ui->tabWidget->addTab(s1, QIcon(), tr("Figure"));


    QScrollArea *s2 = new QScrollArea();

    m_chartSetWidget = new SAChartSetWidget(ui->tabWidget);
    s2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    s2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    s2->setWidgetResizable(true);
    s2->setBackgroundRole(QPalette::NoRole);
    s2->setWidget(m_chartSetWidget);
    ui->tabWidget->addTab(s2, QIcon(), tr("Chart"));

    connect(m_chartSetWidget, &SAChartSetWidget::chartTitleChanged,
        this, &SASetWidget::chartTitleChanged);
}


SASetWidget::~SASetWidget()
{
    delete ui;
}


/**
 * @brief 获取当前设置的figure
 * @return
 */
SAFigureWindow *SASetWidget::currentSettingFigure() const
{
    return (m_figureSetWidget->figure());
}


/**
 * @brief 设置需要设置的fig
 * @param fig
 */
void SASetWidget::setFigure(SAFigureWindow *fig)
{
    m_figureSetWidget->setFigureWidget(fig);
}


/**
 * @brief 更新某个plotitem的内容
 * @param it
 */
void SASetWidget::setPlotItem(QwtPlotItem *it)
{
    m_chartSetWidget->setChart(qobject_cast<SAChart2D *>(it->plot()));
}


/**
 * @brief 设置图表
 * @param chart
 */
void SASetWidget::setChart(SAChart2D *chart)
{
    m_chartSetWidget->setChart(chart);
}


/**
 * @brief 获取当前设置的chart
 * @return
 */
SAChart2D *SASetWidget::currentSettingChart() const
{
    return (m_chartSetWidget->chart());
}


/**
 * @brief 获取figure设置窗口
 * @return
 */
SAFigureSetWidget *SASetWidget::getFigureSetWidget()
{
    return (m_figureSetWidget);
}
