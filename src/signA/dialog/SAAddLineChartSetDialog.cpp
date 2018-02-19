#include "SAAddLineChartSetDialog.h"
#include "ui_SAAddLineChartSetDialog.h"
#include "SATimeFormatSetWidget.h"
#include "SAUserDefineAxisDataWidget.h"
#include "SAValueSelectComboBox.h"
#include <QButtonGroup>
#include "SAValueManager.h"
SAAddLineChartSetDialog::SAAddLineChartSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAAddLineChartSetDialog)
{
    ui->setupUi(this);
    m_xMoreSetButtonGroup = new QButtonGroup(this);
    m_xMoreSetButtonGroup->addButton(ui->radioButtonXNormal,NormalSet);
    m_xMoreSetButtonGroup->addButton(ui->radioButtonXUseDef,UserDefineSet);
    m_xMoreSetButtonGroup->setExclusive(true);

    m_yMoreSetButtonGroup = new QButtonGroup(this);
    m_yMoreSetButtonGroup->addButton(ui->radioButtonYNormal,NormalSet);
    m_yMoreSetButtonGroup->addButton(ui->radioButtonYUseDef,UserDefineSet);
    m_yMoreSetButtonGroup->setExclusive(true);


    ui->valueComboBoxX->setFilter(&filterSAValueSelectComboBox1Dim);
    ui->valueComboBoxY->setFilter(&filterSAValueSelectComboBox1Dim);

    connect(m_xMoreSetButtonGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            ,this,&SAAddLineChartSetDialog::onXRadioButtonClicked);
    connect(m_yMoreSetButtonGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            ,this,&SAAddLineChartSetDialog::onYRadioButtonClicked);

    setXAsixSet(NormalSet);
    setYAsixSet(NormalSet);
}

SAAddLineChartSetDialog::~SAAddLineChartSetDialog()
{
    delete ui;
}

void SAAddLineChartSetDialog::setXAsixSet(SAAddLineChartSetDialog::AsixSet set)
{
    int id = static_cast<int>(set);
    QAbstractButton *btn = m_xMoreSetButtonGroup->button(id);
    if(btn)
    {
        btn->setChecked(true);
    }
    onXRadioButtonClicked(id);
}

void SAAddLineChartSetDialog::setYAsixSet(SAAddLineChartSetDialog::AsixSet set)
{
    int id = static_cast<int>(set);
    QAbstractButton *btn = m_yMoreSetButtonGroup->button(id);
    if(btn)
    {
        btn->setChecked(true);
    }
    onYRadioButtonClicked(id);
}

void SAAddLineChartSetDialog::setXSelectName(const QString &name)
{
    ui->valueComboBoxX->setComboBoxText(name);
}

void SAAddLineChartSetDialog::setYSelectName(const QString &name)
{
    ui->valueComboBoxY->setComboBoxText(name);
}

void SAAddLineChartSetDialog::setXUserDefineValues(double start, double increase)
{
    ui->userDefineXWidget->setStartValue(start);
    ui->userDefineXWidget->setIncreaseValue(increase);
}

void SAAddLineChartSetDialog::setYUserDefineValues(double start, double increase)
{
    ui->userDefineYWidget->setStartValue(start);
    ui->userDefineYWidget->setIncreaseValue(increase);
}

SAAddLineChartSetDialog::AsixSet SAAddLineChartSetDialog::getXAxisSet() const
{
    return static_cast<AsixSet>(m_xMoreSetButtonGroup->checkedId());
}

SAAddLineChartSetDialog::AsixSet SAAddLineChartSetDialog::getYAxisSet() const
{
    return static_cast<AsixSet>(m_yMoreSetButtonGroup->checkedId());
}


void SAAddLineChartSetDialog::getXUserDefineValues(double &start, double &increase) const
{
    start = ui->userDefineXWidget->getStartValue();
    increase = ui->userDefineXWidget->getIncreaseValue();
}

void SAAddLineChartSetDialog::getYUserDefineValues(double &start, double &increase) const
{
    start = ui->userDefineYWidget->getStartValue();
    increase = ui->userDefineYWidget->getIncreaseValue();
}
///
/// \brief 是否添加到当前的chart
/// \return
///
bool SAAddLineChartSetDialog::isAddInCurrentChart() const
{
    return ui->checkBoxAddInCur->isChecked();
}
///
/// \brief 曲线的名
/// \return
///
QString SAAddLineChartSetDialog::getChartTitle() const
{
    return ui->lineEditTitle->text();
}
///
/// \brief 设置曲线名
/// \param title 曲线名
///
void SAAddLineChartSetDialog::setChartTitle(const QString &title)
{
    ui->lineEditTitle->setText(title);
}

///
/// \brief 获取x值
/// \return
///
SAAbstractDatas *SAAddLineChartSetDialog::getXDatas() const
{
    return saValueManager->findData(ui->valueComboBoxX->comboBoxText());
}

///
/// \brief 获取y值
/// \return
///
SAAbstractDatas *SAAddLineChartSetDialog::getYDatas() const
{
    return saValueManager->findData(ui->valueComboBoxY->comboBoxText());
}

void SAAddLineChartSetDialog::onXRadioButtonClicked(int id)
{
    showValueComboBoxX(NormalSet == id);
    showUserDefX(UserDefineSet == id);
}

void SAAddLineChartSetDialog::onYRadioButtonClicked(int id)
{
    showValueComboBoxY(NormalSet == id);
    showUserDefY(UserDefineSet == id);
}

void SAAddLineChartSetDialog::showValueComboBoxX(bool on)
{
    ui->valueComboBoxX->setVisible(on);
}

void SAAddLineChartSetDialog::showValueComboBoxY(bool on)
{
    ui->valueComboBoxY->setVisible(on);
}

void SAAddLineChartSetDialog::showUserDefX(bool on)
{
    ui->userDefineXWidget->setVisible(on);
}

void SAAddLineChartSetDialog::showUserDefY(bool on)
{
    ui->userDefineYWidget->setVisible(on);
}



