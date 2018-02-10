#include "SAAddLineChartSetDialog.h"
#include "ui_SAAddLineChartSetDialog.h"
#include "SATimeFormatSetWidget.h"
#include "SAUserDefineAxisDataWidget.h"
#include "SAValueSelectComboBox.h"
#include <QButtonGroup>

SAAddLineChartSetDialog::SAAddLineChartSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAAddLineChartSetDialog)
{
    ui->setupUi(this);
    m_xMoreSetButtonGroup = new QButtonGroup(this);
    m_xMoreSetButtonGroup->addButton(ui->radioButtonXNormal,NormalSet);
    m_xMoreSetButtonGroup->addButton(ui->radioButtonXTime,TimeSet);
    m_xMoreSetButtonGroup->addButton(ui->radioButtonXInv,InvSet);
    m_xMoreSetButtonGroup->addButton(ui->radioButtonXUseDef,UserDefineSet);
    m_xMoreSetButtonGroup->setExclusive(true);

    m_yMoreSetButtonGroup = new QButtonGroup(this);
    m_yMoreSetButtonGroup->addButton(ui->radioButtonYNormal,NormalSet);
    m_yMoreSetButtonGroup->addButton(ui->radioButtonYTime,TimeSet);
    m_yMoreSetButtonGroup->addButton(ui->radioButtonYInv,InvSet);
    m_yMoreSetButtonGroup->addButton(ui->radioButtonYUseDef,UserDefineSet);
    m_yMoreSetButtonGroup->setExclusive(true);


    m_xValueCombox = new SAValueSelectComboBox(this);
    m_yValueCombox = new SAValueSelectComboBox(this);
    m_xValueCombox->setFilter(&filterSAValueSelectComboBox1Dim);
    m_yValueCombox->setFilter(&filterSAValueSelectComboBox1Dim);

    m_xTimeSet = new SATimeFormatSetWidget(this);
    m_yTimeSet = new SATimeFormatSetWidget(this);
    m_xUsrDef = new SAUserDefineAxisDataWidget(this);
    m_yUsrDef = new SAUserDefineAxisDataWidget(this);
    ui->stackedWidgetX->addWidget(m_xValueCombox);
    ui->stackedWidgetX->addWidget(m_xTimeSet);
    ui->stackedWidgetX->addWidget(m_xUsrDef);
    ui->stackedWidgetY->addWidget(m_yValueCombox);
    ui->stackedWidgetY->addWidget(m_yTimeSet);
    ui->stackedWidgetY->addWidget(m_yUsrDef);
    connect(m_xMoreSetButtonGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            ,this,&SAAddLineChartSetDialog::onXRadioButtonClicked);
    connect(m_yMoreSetButtonGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            ,this,&SAAddLineChartSetDialog::onYRadioButtonClicked);

    ui->radioButtonXNormal->setChecked(true);
    ui->radioButtonYNormal->setChecked(true);
}

SAAddLineChartSetDialog::~SAAddLineChartSetDialog()
{
    delete ui;
}

void SAAddLineChartSetDialog::setXAsixSet(SAAddLineChartSetDialog::AsixSet set)
{
    QAbstractButton *btn = m_xMoreSetButtonGroup->button(static_cast<int>(set));
    if(btn)
    {
        btn->setChecked(true);
    }
    onXRadioButtonClicked(set);
}

void SAAddLineChartSetDialog::setYAsixSet(SAAddLineChartSetDialog::AsixSet set)
{
    QAbstractButton *btn = m_yMoreSetButtonGroup->button(static_cast<int>(set));
    if(btn)
    {
        btn->setChecked(true);
    }
    onYRadioButtonClicked(set);
}

void SAAddLineChartSetDialog::setXSelectName(const QString &name)
{
    m_xValueCombox->setComboBoxText(name);
}

void SAAddLineChartSetDialog::setYSelectName(const QString &name)
{
    m_yValueCombox->setComboBoxText(name);
}

void SAAddLineChartSetDialog::setXUserDefineValues(double start, double increase)
{
    m_xUsrDef->setStartValue(start);
    m_xUsrDef->setIncreaseValue(increase);
}

void SAAddLineChartSetDialog::setYUserDefineValues(double start, double increase)
{
    m_yUsrDef->setStartValue(start);
    m_yUsrDef->setIncreaseValue(increase);
}

SAAddLineChartSetDialog::AsixSet SAAddLineChartSetDialog::getXAxisSet() const
{
    return static_cast<AsixSet>(m_xMoreSetButtonGroup->checkedId());
}

SAAddLineChartSetDialog::AsixSet SAAddLineChartSetDialog::getYAxisSet() const
{
    return static_cast<AsixSet>(m_yMoreSetButtonGroup->checkedId());
}

QString SAAddLineChartSetDialog::getXTimeFormat() const
{
    return m_xTimeSet->getTimeFormat();
}

QString SAAddLineChartSetDialog::getYTimeFormat() const
{
    return m_yTimeSet->getTimeFormat();
}

void SAAddLineChartSetDialog::getXUserDefineValues(double &start, double &increase) const
{
    start = m_xUsrDef->getStartValue();
    increase = m_xUsrDef->getIncreaseValue();
}

void SAAddLineChartSetDialog::getYUserDefineValues(double &start, double &increase) const
{
    start = m_yUsrDef->getStartValue();
    increase = m_yUsrDef->getIncreaseValue();
}

void SAAddLineChartSetDialog::onXRadioButtonClicked(int id)
{

    switch(id)
    {
    case NormalSet:
    case InvSet:
        ui->stackedWidgetX->setCurrentWidget(m_xValueCombox);
        break;
    case TimeSet:
        ui->stackedWidgetX->setCurrentWidget(m_xTimeSet);
        break;
    case UserDefineSet:
        ui->stackedWidgetX->setCurrentWidget(m_xUsrDef);
        break;
    default:
        break;
    }
}

void SAAddLineChartSetDialog::onYRadioButtonClicked(int id)
{
    switch(id)
    {
    case NormalSet:
    case InvSet:
        ui->stackedWidgetY->setCurrentWidget(m_yValueCombox);
        break;
    case TimeSet:
        ui->stackedWidgetY->setCurrentWidget(m_yTimeSet);
        break;
    case UserDefineSet:
        ui->stackedWidgetY->setCurrentWidget(m_yUsrDef);
        break;
    default:
        break;
    }
}



