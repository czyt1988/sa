#include "SARibbonPannel.h"
#include <QToolButton>
SARibbonPannel::SARibbonPannel(QWidget *parent):QWidget(parent)
{
    setFixedHeight(98);
}

void SARibbonPannel::addLargeAction(QAction *action)
{
    QToolButton* btn = new QToolButton(this);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setGeometry(3,3,42,78);
    btn->setAutoRaise(true);
    btn->setDefaultAction(action);
    btn->setIconSize(QSize(32,32));
    btn->setPopupMode(QToolButton::MenuButtonPopup);
}
