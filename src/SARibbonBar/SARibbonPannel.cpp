#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include <QAction>
#include <QIcon>
#include <QDebug>
#include <QGridLayout>
#include <QFontMetrics>
#include <QPainter>
SARibbonPannel::SARibbonPannel(QWidget *parent):QWidget(parent)
  ,m_nextElementPosition(3,3)
  ,m_row(0)
{
    setStyleSheet(QString(""
                          "SARibbonToolButton::pressed{"
                          " border: 1px solid #f2ca58;"
                          " background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 #FDEEB3, stop:0.1282 #FDE38A,stop:0.8333 #FCE58C, stop:1 #FDFDEB);"
                          "}"
                          "SARibbonToolButton::checked{"
                          " border: 1px solid #f2ca58;"
                          " background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 #FDEEB3, stop:0.1282 #FDE38A,stop:0.8333 #FCE58C, stop:1 #FDFDEB);"
                          "}"
                          "SARibbonToolButton::hover {"
                          " border: 1px solid #f2ca58;"
                          " background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,stop:0 #FDEEB3, stop:0.1282 #FDE38A,stop:0.8333 #FCE58C, stop:1 #FDFDEB);"
                          " }"
                          ""));
    setFixedHeight(98);
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(3,2,3,21);
}

SARibbonToolButton *SARibbonPannel::addLargeAction(QAction *action)
{
    SARibbonToolButton* btn = new SARibbonToolButton(this);
    btn->setButtonType(SARibbonToolButton::LargeButton);
    btn->setAutoRaise(true);
    btn->setDefaultAction(action);
    QSize iconSize = maxHightIconSize(action->icon().actualSize(QSize(32,32)),32);
    btn->setIconSize(iconSize);
    if(action->menu())
        btn->setPopupMode(QToolButton::MenuButtonPopup);
    m_gridLayout->addWidget(btn,0,m_gridLayout->columnCount(),6,1);
    m_row = 0;
    return btn;
}

SARibbonToolButton* SARibbonPannel::addSmallAction(QAction *action)
{
    SARibbonToolButton* btn = new SARibbonToolButton(this);
    btn->setButtonType(SARibbonToolButton::SmallButton);
    btn->setAutoRaise(true);
    btn->setDefaultAction(action);
    QSize iconSize = maxHightIconSize(action->icon().actualSize(QSize(16,16)),16);
    btn->setIconSize(iconSize);
    if(action->menu())
        btn->setPopupMode(QToolButton::MenuButtonPopup);
    if(0 == m_row)
        m_gridLayout->addWidget(btn,m_row,m_gridLayout->columnCount(),2,1);
    else
        m_gridLayout->addWidget(btn,m_row,m_gridLayout->columnCount()-1,2,1);
    m_row += 2;
    if(m_row >= 6)
        m_row = 0;
    return btn;
}

void SARibbonPannel::addWidget(QWidget *w)
{
    m_gridLayout->addWidget(w,0,m_gridLayout->columnCount(),6,1);
}

QSize SARibbonPannel::maxHightIconSize(const QSize &size, int height)
{
    if(size.height() < height)
    {
        return  (size * ((float)height/size.height()));
    }
    return size;
}

void SARibbonPannel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QFont f = font();
    f.setPixelSize(11);
    p.setFont(f);
    p.drawText(0,77,width(),21,Qt::AlignCenter,windowTitle());
}
