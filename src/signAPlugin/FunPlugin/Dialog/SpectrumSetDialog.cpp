#include "SpectrumSetDialog.h"
#include "ui_SpectrumSetDialog.h"

SpectrumSetDialog::SpectrumSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpectrumSetDialog)
{
    ui->setupUi(this);
}

SpectrumSetDialog::~SpectrumSetDialog()
{
    delete ui;
}
