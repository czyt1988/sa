#include "SAAligmentPropertyItem.h"
#include <QToolButton>
#include <QHBoxLayout>
class SAAligmentPropertyItem::UI
{
public:
    QToolButton* bottomScale;
    QToolButton* topScale;
    QToolButton* leftScale;
    QToolButton* rightScale;
    QHBoxLayout* hBoxLayout;
    void setupUI(SAAligmentPropertyItem* par)
    {
        hBoxLayout = new QHBoxLayout;

        bottomScale = new QToolButton;
        bottomScale->setFixedSize(23,23);
        topScale = new QToolButton;
        topScale->setFixedSize(23,23);
        leftScale = new QToolButton;
        leftScale->setFixedSize(23,23);
        rightScale = new QToolButton;
        rightScale->setFixedSize(23,23);
        hBoxLayout->addStretch();
        hBoxLayout->addWidget(leftScale);
        hBoxLayout->addWidget(topScale);
        hBoxLayout->addWidget(bottomScale);
        hBoxLayout->addWidget(rightScale);
        par->addLayout(hBoxLayout);
        par->connect(bottomScale,&QToolButton::toggled
                     ,par,&SAAligmentPropertyItem::onButtonBottomToggled);
        par->connect(topScale,&QToolButton::toggled
                     ,par,&SAAligmentPropertyItem::onButtonBottomToggled);
        par->connect(leftScale,&QToolButton::toggled
                     ,par,&SAAligmentPropertyItem::onButtonBottomToggled);
        par->connect(rightScale,&QToolButton::toggled
                     ,par,&SAAligmentPropertyItem::onButtonBottomToggled);
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

void SAAligmentPropertyItem::onButtonBottomToggled(bool checked)
{
    if(checked)
    {
        emit stateChanged(Qt::AlignBottom);
    }
}

void SAAligmentPropertyItem::onButtonTopToggled(bool checked)
{
    if(checked)
    {
        emit stateChanged(Qt::AlignTop);
    }
}

void SAAligmentPropertyItem::onButtonLeftToggled(bool checked)
{
    if(checked)
    {
        emit stateChanged(Qt::AlignLeft);
    }
}

void SAAligmentPropertyItem::onButtonRightToggled(bool checked)
{
    if(checked)
    {
        emit stateChanged(Qt::AlignRight);
    }
}
