#include "SARibbonToolButton.h"
#include <QStylePainter>
#include <QStyleOptionToolButton>
#include "SARibbonStyle.h"
SARibbonToolButton::SARibbonToolButton(QWidget *parent)
    :QToolButton(parent)
    ,m_buttonType(LargeButton)
{
    setStyleSheet(QString(""
                          "SARibbonToolButton::hover {"
                          " background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 #FEF9F4, stop:0.38 #FDE0BD,stop:0.39 #FFCE69, stop:1 #FFFFE7);"
                          "  }"
                          ""));
}

void SARibbonToolButton::paintEvent(QPaintEvent *event)
{
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    p.drawComplexControl((QStyle::ComplexControl)SARibbonStyle::CC_RibbonToolButton, opt);
    //QToolButton::paintEvent(event);
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
