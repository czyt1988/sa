#include "SAVGroupBoxPropertyItem.h"
#include <QVBoxLayout>
#include <QStyle>
#include <QStyleOption>

const char* sty =
        "QGroupBox::indicator {"
        "width: 16px;"
        "height: 16px;"
        "}"
        "QGroupBox::indicator:unchecked {"
              "image: url(:/icon/icons/colled.png);"
        "}"
        "QGroupBox::indicator:checked {"
              "image: url(:/icon/icons/expand.png);"
        "}"
    ;

class SAVGroupBoxPropertyItem::UI
{
public:
    QVBoxLayout* vlayout;
    void setupUI(SAVGroupBoxPropertyItem* par)
    {
        par->setStyleSheet(sty);
        vlayout = new QVBoxLayout;
        vlayout->setContentsMargins(4, 4, 4, 4);
        vlayout->setSpacing(4);
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


