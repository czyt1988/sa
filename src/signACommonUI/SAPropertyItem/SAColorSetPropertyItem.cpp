#include "SAColorSetPropertyItem.h"
#include "ui_SAColorSetPropertyItem.h"

SAColorSetPropertyItem::SAColorSetPropertyItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAColorSetPropertyItem)
{
    ui->setupUi(this);
}

SAColorSetPropertyItem::~SAColorSetPropertyItem()
{
    delete ui;
}
