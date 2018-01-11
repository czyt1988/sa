#include "SAChartAxesSetWidget.h"
#include "ui_SAChartAxesSetWidget.h"
#include "SAQwtAxisSetWidget.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include <QVBoxLayout>
#include <QDebug>
SAChartAxesSetWidget::SAChartAxesSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAChartAxesSetWidget)
    ,m_chart(nullptr)
{
    ui->setupUi(this);
    m_layoutXBottom = new QVBoxLayout();
    m_layoutXTop = new QVBoxLayout();
    m_layoutYLeft = new QVBoxLayout();
    m_layoutYRight = new QVBoxLayout();

    m_xBottomAxisWidget = new SAQwtAxisSetWidget();
    m_xTopAxisWidget = new SAQwtAxisSetWidget();
    m_yLeftAxisWidget = new SAQwtAxisSetWidget();
    m_yRightAxisWidget = new SAQwtAxisSetWidget();

    m_layoutXBottom->addWidget(m_xBottomAxisWidget);
    m_layoutXTop->addWidget(m_xTopAxisWidget);
    m_layoutYLeft->addWidget(m_yLeftAxisWidget);
    m_layoutYRight->addWidget(m_yRightAxisWidget);

    ui->groupBoxXBottom->setLayout(m_layoutXBottom);
    ui->groupBoxXTop->setLayout(m_layoutXTop);
    ui->groupBoxYLeft->setLayout(m_layoutYLeft);
    ui->groupBoxYRight->setLayout(m_layoutYRight);

    connect(m_xBottomAxisWidget,&SAQwtAxisSetWidget::enableAxis
            ,this,&SAChartAxesSetWidget::onEnableAxis);
    connect(m_xTopAxisWidget,&SAQwtAxisSetWidget::enableAxis
            ,this,&SAChartAxesSetWidget::onEnableAxis);
    connect(m_yLeftAxisWidget,&SAQwtAxisSetWidget::enableAxis
            ,this,&SAChartAxesSetWidget::onEnableAxis);
    connect(m_yRightAxisWidget,&SAQwtAxisSetWidget::enableAxis
            ,this,&SAChartAxesSetWidget::onEnableAxis);
}

SAChartAxesSetWidget::~SAChartAxesSetWidget()
{
    delete ui;
}

void SAChartAxesSetWidget::setChart(SAChart2D *chart)
{
    m_chart = chart;
    m_xBottomAxisWidget->setChart(chart,QwtPlot::xBottom);
    m_xTopAxisWidget->setChart(chart,QwtPlot::xTop);
    m_yLeftAxisWidget->setChart(chart,QwtPlot::yLeft);
    m_yRightAxisWidget->setChart(chart,QwtPlot::yRight);
}
///
/// \brief 坐标轴激活
///
/// 如果激活了坐标轴，而这个坐标轴并没有关联任何Item，
/// 则把这个坐标轴设置为另外一个坐标轴一样的范围
/// \param enable
/// \param axis
///
void SAChartAxesSetWidget::onEnableAxis(bool enable, int axis)
{
    if(QwtPlot::axisCnt == axis)
    {
        return;
    }
    if(enable && m_chart)
    {
        const QwtPlotItemList& items = m_chart->itemList();
        const int size = items.size();
        bool noItemLink = true;
        for(int i=0;i<size;++i)
        {
            const QwtPlotItem* item = items[i];
            if(item)
            {
                if(item->xAxis() == axis
                   ||
                   item->yAxis() == axis
                   )
                {
                    noItemLink = false;
                }
            }
        }
        if(noItemLink)
        {
            int otherAxis = SAChart::otherAxis(axis);
            QwtInterval inv = m_chart->axisInterval(otherAxis);
            m_chart->setAxisScale(axis,inv.minValue(),inv.maxValue());
        }
    }
}
