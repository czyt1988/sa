#include "SAChartAxesSetWidget.h"
#include "ui_SAChartAxesSetWidget.h"
#include "SAQwtAxisSetWidget.h"
#include "SAChart2D.h"
#include <QVBoxLayout>
#include <QDebug>
SAChartAxesSetWidget::SAChartAxesSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAChartAxesSetWidget)
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

}

SAChartAxesSetWidget::~SAChartAxesSetWidget()
{
    delete ui;
}

void SAChartAxesSetWidget::setChart(SAChart2D *chart)
{
    m_xBottomAxisWidget->setChart(chart,QwtPlot::xBottom);
    m_xTopAxisWidget->setChart(chart,QwtPlot::xTop);
    m_yLeftAxisWidget->setChart(chart,QwtPlot::yLeft);
    m_yRightAxisWidget->setChart(chart,QwtPlot::yRight);
}
