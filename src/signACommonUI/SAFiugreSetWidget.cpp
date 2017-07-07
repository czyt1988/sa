#include "SAFiugreSetWidget.h"
#include "ui_SAFiugreSetWidget.h"

SAFiugreSetWidget::SAFiugreSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAFiugreSetWidget)
{
    ui->setupUi(this);
}

SAFiugreSetWidget::~SAFiugreSetWidget()
{
    delete ui;
}
