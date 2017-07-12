#include "SAVGroupBoxPropertyItem.h"
#include <QVBoxLayout>
class SAVGroupBoxPropertyItem::UI
{
public:
    QVBoxLayout* vlayout;
    void setupUI(SAVGroupBoxPropertyItem* par)
    {
        vlayout = new QVBoxLayout;
        par->setLayout(vlayout);
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
