#include "SARibbonToolButton.h"

SARibbonToolButton::SARibbonToolButton(QWidget *parent)
    :QToolButton(parent)
    ,m_buttonType(LargeButton)
{

}

void SARibbonToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
//    switch(m_buttonType)
//    {
//    case LargeButton:paintLargeButton(event);return;
//    case SmallButton:paintSmallButton(event);return;
//    default:
//        return;
//    }
}

void SARibbonToolButton::enterEvent(QEvent *event)
{
    QToolButton::enterEvent(event);
}

void SARibbonToolButton::leaveEvent(QEvent *e)
{
    QToolButton::leaveEvent(e);
}

void SARibbonToolButton::paintLargeButton(QPaintEvent *event)
{

}

void SARibbonToolButton::paintSmallButton(QPaintEvent *event)
{

}

SARibbonToolButton::RibbonButtonType SARibbonToolButton::buttonType() const
{
    return m_buttonType;
}

void SARibbonToolButton::setButtonType(const RibbonButtonType &buttonType)
{
    m_buttonType = buttonType;
}
