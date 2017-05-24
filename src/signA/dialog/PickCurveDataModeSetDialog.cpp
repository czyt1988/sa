#include "PickCurveDataModeSetDialog.h"
#include "ui_PickCurveDataModeSetDialog.h"

PickCurveDataModeSetDialog::PickCurveDataModeSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PickCurveDataModeSetDialog)
{
    ui->setupUi(this);
    ui->radioButton_allRange->setChecked(true);
    ui->radioButton_point->setChecked(true);
}

PickCurveDataModeSetDialog::~PickCurveDataModeSetDialog()
{
    delete ui;
}

SA::ViewRange PickCurveDataModeSetDialog::getViewRange() const
{
    if(ui->radioButton_viewRange->isChecked())
        return SA::InViewRange;
    else if(ui->radioButton_allRange->isChecked())
        return SA::AllRange;
    return SA::AllRange;
}

SA::PickDataMode PickCurveDataModeSetDialog::getPickDataMode() const
{
    if(ui->radioButton_xOnly->isChecked())
        return SA::xOnly;
    else if(ui->radioButton_yOnly->isChecked())
        return SA::yOnly;
    else if(ui->radioButton_point->isChecked())
        return SA::xyPoint;
    return SA::xyPoint;
}

void PickCurveDataModeSetDialog::on_buttonBox_accepted()
{
    //something
    return QDialog::accept();
}
