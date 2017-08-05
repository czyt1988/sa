#include "SAAligmentSetWidget.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QApplication>
#define ICON_AligmentTop QIcon(":/icon/icons/aligmentTop.png")
#define ICON_AligmentLeft QIcon(":/icon/icons/aligmentLeft.png")
#define ICON_AligmentRight QIcon(":/icon/icons/aligmentRight.png")
#define ICON_AligmentDown QIcon(":/icon/icons/aligmentDown.png")
#define ICON_AligmentCenter QIcon(":/icon/icons/aligmentCenter.png")

class SAAligmentSetWidget::UI
{
public:
    QButtonGroup* buttonGroup;
    QToolButton* bottomAlignment;
    QToolButton* topAlignment;
    QToolButton* leftAlignment;
    QToolButton* rightAlignment;
    QToolButton* centerAlignment;
    QHBoxLayout* hBoxLayout;
    void setupUI(SAAligmentSetWidget* par)
    {
        hBoxLayout = new QHBoxLayout(par);
        hBoxLayout->setMargin(0);
        hBoxLayout->setContentsMargins(0,0,0,0);
        buttonGroup = new QButtonGroup(par);

        bottomAlignment = createToolButton(ICON_AligmentCenter);
        topAlignment = createToolButton(ICON_AligmentTop);
        leftAlignment = createToolButton(ICON_AligmentLeft);
        rightAlignment = createToolButton(ICON_AligmentRight);
        centerAlignment = createToolButton(ICON_AligmentCenter);

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
        par->connect(buttonGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
                     ,par,&SAAligmentSetWidget::onButtonClicked);
        par->setMinimumHeight(23);
        retranslateUi();
    }

    void retranslateUi()
    {
        bottomAlignment->setToolTip(QApplication::translate("SAAligmentSetWidget", "Bottom Alignment", 0));
        topAlignment->setToolTip(QApplication::translate("SAAligmentSetWidget", "Top Alignment", 0));
        leftAlignment->setToolTip(QApplication::translate("SAAligmentSetWidget", "Left Alignment", 0));
        rightAlignment->setToolTip(QApplication::translate("SAAligmentSetWidget", "Right Alignment", 0));
        centerAlignment->setToolTip(QApplication::translate("SAAligmentSetWidget", "Center Alignment", 0));
    }
private:
    QToolButton* createToolButton(const QIcon& icon)
    {
        QToolButton* btn = new QToolButton;
        btn->setFixedSize(23,23);
        btn->setCheckable(true);
        btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        btn->setAutoRaise(true);
        btn->setIcon(icon);
        return btn;
    }
};

SAAligmentSetWidget::SAAligmentSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new SAAligmentSetWidget::UI)
{
    ui->setupUI(this);
}

SAAligmentSetWidget::~SAAligmentSetWidget()
{
    delete ui;
}

void SAAligmentSetWidget::setAlignment(Qt::Alignment al)
{
    QAbstractButton * btn = ui->buttonGroup->button(int(al));
    if(btn)
    {
        btn->setChecked(true);
    }
}

void SAAligmentSetWidget::onButtonClicked(int id)
{
    Qt::Alignment al = static_cast<Qt::Alignment>(id);
    emit alignmentStateChanged(al);
}
