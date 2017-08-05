#include "SAChartAxisSetSelect.h"
#include "ui_SAChartAxisSetSelect.h"
#include <QButtonGroup>
#include "qwt_plot.h"
SAChartAxisSetSelect::SAChartAxisSetSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAChartAxisSetSelect)
{
    ui->setupUi(this);
    mXAxisButtonGroup = new QButtonGroup(this);
    mXAxisButtonGroup->addButton(ui->pushButtonXTop,QwtPlot::xTop);
    mXAxisButtonGroup->addButton(ui->pushButtonXBottom,QwtPlot::xBottom);

    mYAxisButtonGroup = new QButtonGroup(this);
    mYAxisButtonGroup->addButton(ui->pushButtonYLeft,QwtPlot::yLeft);
    mYAxisButtonGroup->addButton(ui->pushButtonYRight,QwtPlot::yRight);
    mXAxisButtonGroup->setExclusive(true);
    mYAxisButtonGroup->setExclusive(true);
    connect(mXAxisButtonGroup,static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked)
            ,this,&SAChartAxisSetSelect::onButtonClicked);
    connect(mYAxisButtonGroup,static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked)
            ,this,&SAChartAxisSetSelect::onButtonClicked);
}

SAChartAxisSetSelect::~SAChartAxisSetSelect()
{
    delete ui;
}

void SAChartAxisSetSelect::setCurrentSelectAxis(int xAxis, int yAxis)
{
    QAbstractButton* btn= nullptr;
    btn = mXAxisButtonGroup->button(xAxis);
    if(btn)
    {
        btn->setChecked(true);
    }
    btn = mYAxisButtonGroup->button(yAxis);
    if(btn)
    {
        btn->setChecked(true);
    }
}

int SAChartAxisSetSelect::getCurrentSelectAxisX() const
{
    return mXAxisButtonGroup->checkedId();
}

int SAChartAxisSetSelect::getCurrentSelectAxisY() const
{
    return mYAxisButtonGroup->checkedId();
}

void SAChartAxisSetSelect::onButtonClicked(QAbstractButton *btn)
{
    Q_UNUSED(btn);
    emit axisChanged(getCurrentSelectAxisX(),getCurrentSelectAxisY());
}


