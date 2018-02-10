#include "SAUserDefineAxisDataWidget.h"
#include "ui_SAUserDefineAxisDataWidget.h"

SAUserDefineAxisDataWidget::SAUserDefineAxisDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAUserDefineAxisDataWidget)
{
    ui->setupUi(this);
    ui->doubleSpinBoxStart->setValue(0);
    ui->doubleSpinBoxIncrease->setValue(1);
    connect(ui->doubleSpinBoxStart,static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
            ,this,&SAUserDefineAxisDataWidget::onStartValueChanged);
    connect(ui->doubleSpinBoxIncrease,static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
            ,this,&SAUserDefineAxisDataWidget::onIncreaseValueChanged);
    updateText();
}

SAUserDefineAxisDataWidget::~SAUserDefineAxisDataWidget()
{
    delete ui;
}

double SAUserDefineAxisDataWidget::getStartValue() const
{
    return ui->doubleSpinBoxStart->value();
}

double SAUserDefineAxisDataWidget::getIncreaseValue() const
{
    return ui->doubleSpinBoxIncrease->value();
}

void SAUserDefineAxisDataWidget::setStartValue(double s)
{
    ui->doubleSpinBoxStart->setValue(s);
}

void SAUserDefineAxisDataWidget::setIncreaseValue(double s)
{
    ui->doubleSpinBoxIncrease->setValue(s);
}

void SAUserDefineAxisDataWidget::onStartValueChanged(double d)
{
    Q_UNUSED(d);
    updateText();
}

void SAUserDefineAxisDataWidget::onIncreaseValueChanged(double d)
{
    Q_UNUSED(d);
    updateText();
}

void SAUserDefineAxisDataWidget::updateText()
{
    double start = getStartValue();
    double inc = getIncreaseValue();
    QString text = QString::number(start);
    for(int i=1;i<6;++i)
    {
        double d = start + i * inc;
        text += ",";
        text += QString::number(d);
    }
    text += "......";
    ui->labelExample->setText(text);
}
