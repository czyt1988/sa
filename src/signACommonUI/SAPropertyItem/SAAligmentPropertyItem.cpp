#include "SAAligmentPropertyItem.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QApplication>
#include "SAAligmentSetWidget.h"
#define ICON_AligmentTop QIcon(":/icon/icons/aligmentTop.png")
#define ICON_AligmentLeft QIcon(":/icon/icons/aligmentLeft.png")
#define ICON_AligmentRight QIcon(":/icon/icons/aligmentRight.png")
#define ICON_AligmentDown QIcon(":/icon/icons/aligmentDown.png")
#define ICON_AligmentCenter QIcon(":/icon/icons/aligmentCenter.png")
class SAAligmentPropertyItem::UI
{
public:
    SAAligmentSetWidget* aligmentWidget;
    void setupUI(SAAligmentPropertyItem* par)
    {
        aligmentWidget = new SAAligmentSetWidget(par);
        par->setWidget(aligmentWidget);
        par->connect(aligmentWidget,&SAAligmentSetWidget::alignmentStateChanged
                     ,par,&SAAligmentPropertyItem::stateChanged);
        par->setMinimumHeight(23);
    }
};

SAAligmentPropertyItem::SAAligmentPropertyItem(QWidget *par)
    :SAPropertyItemContainer(par)
    ,ui(new SAAligmentPropertyItem::UI)
{
    ui->setupUI(this);
}

SAAligmentPropertyItem::~SAAligmentPropertyItem()
{
    delete ui;
}

void SAAligmentPropertyItem::setAlignment(Qt::Alignment al)
{
    ui->aligmentWidget->setAlignment(al);
}



