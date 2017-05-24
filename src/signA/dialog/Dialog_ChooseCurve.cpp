#include "Dialog_ChooseCurve.h"
#include "ui_Dialog_ChooseCurve.h"

Dialog_ChooseCurve::Dialog_ChooseCurve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ChooseCurve)
{
    ui->setupUi(this);
}

Dialog_ChooseCurve::~Dialog_ChooseCurve()
{
    delete ui;
}
