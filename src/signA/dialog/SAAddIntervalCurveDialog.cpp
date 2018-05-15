#include "SAAddIntervalCurveDialog.h"
#include "ui_SAAddIntervalCurveDialog.h"
#include "SAValueManager.h"

SAAddIntervalCurveDialog::SAAddIntervalCurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAAddIntervalCurveDialog)
{
    ui->setupUi(this);
    ui->valueComboBoxValue->setFilter(&filterSAValueSelectComboBox1Dim);
    ui->valueComboBoxMin->setFilter(&filterSAValueSelectComboBox1Dim);
    ui->valueComboBoxMax->setFilter(&filterSAValueSelectComboBox1Dim);

}

SAAddIntervalCurveDialog::~SAAddIntervalCurveDialog()
{
    delete ui;
}
///
/// \brief 获取x值
/// \return
///
SAAbstractDatas *SAAddIntervalCurveDialog::getValueDatas() const
{
    return saValueManager->findData(ui->valueComboBoxValue->comboBoxText());
}
///
/// \brief 获取ymin值
/// \return
///
SAAbstractDatas *SAAddIntervalCurveDialog::getMinDatas() const
{
    return saValueManager->findData(ui->valueComboBoxMin->comboBoxText());
}
///
/// \brief 获取ymax值
/// \return
///
SAAbstractDatas *SAAddIntervalCurveDialog::getMaxDatas() const
{
    return saValueManager->findData(ui->valueComboBoxMax->comboBoxText());
}

void SAAddIntervalCurveDialog::setValueDatas(SAAbstractDatas *d)
{
    ui->valueComboBoxValue->setComboBoxText(d->getName());
}

void SAAddIntervalCurveDialog::setMinDatas(SAAbstractDatas *d)
{
    ui->valueComboBoxMin->setComboBoxText(d->getName());
}

void SAAddIntervalCurveDialog::setMaxDatas(SAAbstractDatas *d)
{
    ui->valueComboBoxMax->setComboBoxText(d->getName());
}
///
/// \brief 曲线的名
/// \return
///
QString SAAddIntervalCurveDialog::getChartTitle() const
{
    return ui->lineEditTitle->text();
}
///
/// \brief 设置曲线名
/// \param title 曲线名
///
void SAAddIntervalCurveDialog::setChartTitle(const QString &title)
{
    ui->lineEditTitle->setText(title);
}
///
/// \brief 是否添加到当前的chart
/// \return
///
bool SAAddIntervalCurveDialog::isAddInCurrentChart() const
{
    return ui->checkBoxAddInCur->isChecked();
}
