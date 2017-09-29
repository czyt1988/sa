#include "SARibbonMainWindow.h"
#include "FramelessHelper.h"
#include "SAWindowButtonGroup.h"
#include "SARibbonBar.h"
#include <QApplication>
#include <QDebug>
class SARibbonMainWindowPrivate
{
public:
    SARibbonBar* ribbonBar;
};

SARibbonMainWindow::SARibbonMainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,m_d(new SARibbonMainWindowPrivate)
{
    FramelessHelper *pHelper = new FramelessHelper(this);
    pHelper->setTitleHeight(30);  //设置窗体的标题栏高度
    //
    m_d->ribbonBar = new SARibbonBar(this);
    setMenuWidget(m_d->ribbonBar);
    //
    new SAWindowButtonGroup(this);
}

const SARibbonBar *SARibbonMainWindow::ribbonBar() const
{
    return m_d->ribbonBar;
}

SARibbonBar *SARibbonMainWindow::ribbonBar()
{
    return m_d->ribbonBar;
}



void SARibbonMainWindow::resizeEvent(QResizeEvent *event)
{
    if(m_d->ribbonBar)
    {
        if(m_d->ribbonBar->size().width() != this->size().width())
        {
            m_d->ribbonBar->setFixedWidth(this->size().width());
        }
    }
    QMainWindow::resizeEvent(event);
}
