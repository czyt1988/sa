#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include <QAction>
#include <QIcon>
#include <QDebug>
SARibbonPannel::SARibbonPannel(QWidget *parent):QWidget(parent)
  ,m_nextElementPosition(3,3)
{
    setFixedHeight(98);

}

SARibbonToolButton *SARibbonPannel::addLargeAction(QAction *action)
{
    SARibbonToolButton* btn = new SARibbonToolButton(this);
    btn->setButtonType(SARibbonToolButton::LargeButton);
    btn->move(m_nextElementPosition);
    btn->setAutoRaise(true);
    btn->setDefaultAction(action);
    QSize iconSize = maxHightIconSize(action->icon().actualSize(QSize(32,32)),32);
    btn->setIconSize(iconSize);
    btn->setFixedWidth(iconSize.width() + 10);
    if(action->menu())
        btn->setPopupMode(QToolButton::MenuButtonPopup);
    m_nextElementPosition.setX(btn->geometry().right());
    m_nextElementPosition.setY(3);
    return btn;
}

SARibbonToolButton* SARibbonPannel::addSmallAction(QAction *action)
{
    SARibbonToolButton* btn = new SARibbonToolButton(this);
    btn->setButtonType(SARibbonToolButton::SmallButton);
    btn->move(m_nextElementPosition);
    btn->setAutoRaise(true);
    btn->setDefaultAction(action);
    QSize iconSize = maxHightIconSize(action->icon().actualSize(QSize(16,16)),16);
    btn->setIconSize(iconSize);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    return btn;
}

QSize SARibbonPannel::maxHightIconSize(const QSize &size, int height)
{
    if(size.height() < height)
    {
        return  (size * ((float)height/size.height()));
    }
    return size;
}
