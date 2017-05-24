#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->textBrowser->setText (tr("version：v0.1\n Author Email：czy.t@163.com"));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
