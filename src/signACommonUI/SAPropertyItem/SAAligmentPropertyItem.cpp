#include "SAAligmentPropertyItem.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QButtonGroup>
class SAAligmentPropertyItem::UI
{
public:
    QButtonGroup* buttonGroup;
    QToolButton* bottomAlignment;
    QToolButton* topAlignment;
    QToolButton* leftAlignment;
    QToolButton* rightAlignment;
    QToolButton* centerAlignment;
    QHBoxLayout* hBoxLayout;
    void setupUI(SAAligmentPropertyItem* par)
    {
        hBoxLayout = new QHBoxLayout;
        hBoxLayout->setMargin(0);
        hBoxLayout->setContentsMargins(0,0,0,0);
        buttonGroup = new QButtonGroup(par);

        bottomAlignment = new QToolButton;
        bottomAlignment->setFixedSize(23,23);
        bottomAlignment->setCheckable(true);

        topAlignment = new QToolButton;
        topAlignment->setFixedSize(23,23);
        topAlignment->setCheckable(true);

        leftAlignment = new QToolButton;
        leftAlignment->setFixedSize(23,23);
        leftAlignment->setCheckable(true);

        rightAlignment = new QToolButton;
        rightAlignment->setFixedSize(23,23);
        rightAlignment->setCheckable(true);

        centerAlignment = new QToolButton;
        centerAlignment->setFixedSize(23,23);
        centerAlignment->setCheckable(true);

        hBoxLayout->addStretch();
        hBoxLayout->addWidget(leftAlignment);
        hBoxLayout->addWidget(topAlignment);
        hBoxLayout->addWidget(bottomAlignment);
        hBoxLayout->addWidget(rightAlignment);
        hBoxLayout->addWidget(centerAlignment);

        buttonGroup->addButton(leftAlignment,Qt::AlignLeft);
        buttonGroup->addButton(topAlignment,Qt::AlignTop);
        buttonGroup->addButton(bottomAlignment,Qt::AlignBottom);
        buttonGroup->addButton(rightAlignment,Qt::AlignRight);
        buttonGroup->addButton(centerAlignment,Qt::AlignCenter);
        buttonGroup->setExclusive(true);
        par->addLayout(hBoxLayout);
        par->connect(buttonGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
                     ,par,&SAAligmentPropertyItem::onButtonClicked);
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
    QAbstractButton * btn = ui->buttonGroup->button(int(al));
    if(btn)
    {
        btn->setChecked(true);
    }
}

QSize SAAligmentPropertyItem::sizeHint() const
{
    return QSize(300,25);
}


void SAAligmentPropertyItem::onButtonClicked(int id)
{
    Qt::Alignment al = static_cast<Qt::Alignment>(id);
    emit stateChanged(al);
}

