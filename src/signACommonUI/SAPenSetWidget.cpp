#include "SAPenSetWidget.h"
#include "ui_SAPenSetWidget.h"

SAPenSetWidget::SAPenSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAPenSetWidget)
{
    ui->setupUi(this);
}

SAPenSetWidget::~SAPenSetWidget()
{
    delete ui;
}

QPen SAPenSetWidget::getPen() const
{
    QPen pen;
    pen.setColor(ui->pushButtonColor->currentColor());
    pen.setWidth(ui->spinBoxPenWidth->value());
    pen.setStyle(ui->comboBoxPenStyle->style());
    return pen;
}

void SAPenSetWidget::setPen(const QPen &pen)
{
    ui->pushButtonColor->setCurrentColor(pen.color());
    ui->comboBoxPenStyle->setStyle(pen.style());
    ui->spinBoxPenWidth->setValue(pen.width());
}
