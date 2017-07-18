#include "SAVGroupBoxPropertyItem.h"
#include <QVBoxLayout>
#include <QStyle>
#include <QStyleOption>
#include <QApplication>





class SAVGroupBoxPropertyItem::UI
{
public:
    QVBoxLayout* vlayout;
    void setupUI(SAVGroupBoxPropertyItem* par)
    {
        vlayout = new QVBoxLayout(par);
        vlayout->setContentsMargins(4, 4, 4, 4);
        vlayout->setSpacing(4);
    }
};

SAVGroupBoxPropertyItem::SAVGroupBoxPropertyItem(QWidget* par):SAGroupBoxPropertyItem(par)
  ,ui(new SAVGroupBoxPropertyItem::UI)
{
    ui->setupUI(this);
}

SAVGroupBoxPropertyItem::~SAVGroupBoxPropertyItem()
{
    delete ui;
}


void SAVGroupBoxPropertyItem::addWidget(QWidget *w, int stretch,Qt::Alignment alignment)
{
    ui->vlayout->addWidget(w,stretch,alignment);
}

void SAVGroupBoxPropertyItem::addLayout(QLayout *layout, int stretch)
{
    ui->vlayout->addLayout(layout,stretch);
}




