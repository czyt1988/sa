#include "SAQwtPlotItemAxisBindSetWidget.h"
#include "ui_SAQwtPlotItemAxisBindSetWidget.h"
#include <QButtonGroup>
#include "qwt_plot.h"
SAQwtPlotItemAxisBindSetWidget::SAQwtPlotItemAxisBindSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAQwtPlotItemAxisBindSetWidget)
{
    ui->setupUi(this);
    m_xBtnGroup = new QButtonGroup(this);
    m_xBtnGroup->addButton(ui->toolButtonXTop,QwtPlot::xTop);
    m_xBtnGroup->addButton(ui->toolButtonXBottom,QwtPlot::xBottom);
    m_xBtnGroup->setExclusive(true);
    m_yBtnGroup = new QButtonGroup(this);
    m_yBtnGroup->addButton(ui->toolButtonYLeft,QwtPlot::yLeft);
    m_yBtnGroup->addButton(ui->toolButtonYRight,QwtPlot::yRight);
    m_yBtnGroup->setExclusive(true);

    connect(m_xBtnGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            ,this,&SAQwtPlotItemAxisBindSetWidget::onBtnChanged);
    connect(m_yBtnGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            ,this,&SAQwtPlotItemAxisBindSetWidget::onBtnChanged);
}

SAQwtPlotItemAxisBindSetWidget::~SAQwtPlotItemAxisBindSetWidget()
{
    delete ui;
}

int SAQwtPlotItemAxisBindSetWidget::getXAxisBind() const
{
    return m_xBtnGroup->checkedId();
}

void SAQwtPlotItemAxisBindSetWidget::setXAxisBind(int axisID)
{
    QAbstractButton * btn = m_xBtnGroup->button(axisID);
    if(btn)
        btn->setChecked(true);
}

int SAQwtPlotItemAxisBindSetWidget::getYAxisBind() const
{
    return m_yBtnGroup->checkedId();
}

void SAQwtPlotItemAxisBindSetWidget::setYAxisBind(int axisID)
{
    QAbstractButton * btn = m_yBtnGroup->button(axisID);
    if(btn)
        btn->setChecked(true);
}

void SAQwtPlotItemAxisBindSetWidget::onBtnChanged(int id)
{
    Q_UNUSED(id);
    emit axisBindChanged(getXAxisBind(),m_yBtnGroup->checkedId());
}
