#include "SAChartAxesSetWidget.h"
#include "ui_SAChartAxesSetWidget.h"
#include "SAQwtAxisSetWidget.h"
SAChartAxesSetWidget::SAChartAxesSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAChartAxesSetWidget)
{
    ui->setupUi(this);
    m_xBottomAxisWidget = new SAQwtAxisSetWidget(ui->groupBoxXBottom);
    m_xTopAxisWidget = new SAQwtAxisSetWidget(ui->groupBoxXTop);
    m_yLeftAxisWidget = new SAQwtAxisSetWidget(ui->groupBoxYLeft);
    m_yRightAxisWidget = new SAQwtAxisSetWidget(ui->groupBoxYRight);
}

SAChartAxesSetWidget::~SAChartAxesSetWidget()
{
    delete ui;
}
