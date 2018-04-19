#include "FitParamSetDialog.h"
#include "ui_FitParamSetDialog.h"

FitParamSetDialog::FitParamSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FitParamSetDialog)
{
    ui->setupUi(this);
}

FitParamSetDialog::~FitParamSetDialog()
{
    delete ui;
}
