#include "SARibbonTabBar.h"

SARibbonTabBar::SARibbonTabBar(QWidget *parent):QTabBar(parent)
  ,m_tabMargin(6,0,0,0)
{
    setExpanding(false);
    setStyleSheet(QString(""
                  "SARibbonTabBar::tab "
                  "{"
                  " background: transparent;"
                  " margin-top: %1px;"
                  " margin-right: %2px;"
                  " margin-left: %3px;"
                  " margin-bottom: %4px;"
                  " min-width:60px;"
                  " max-width:200px;"
                  " min-height:30px;"
                  " max-height:30px;"
                  ""
                  "}"
                  "SARibbonTabBar::tab:selected, QTabBar::tab:hover "
                  "{ "
                  " border-top-left-radius: 2px;"
                  " border-top-right-radius: 2px;"
                  "}"
                  "SARibbonTabBar::tab:selected{"
                  " border: 1px solid #BAC9DB; "
                  " background: white;"
                  " border-bottom-color: #FFFFFF;"
                  "}"
                  "SARibbonTabBar::tab:hover:!selected"
                  "{"
                  " border: 1px solid #ECBC3D;"
                  "}"
                  "SARibbonTabBar::tab:!selected "
                  "{"
                  " margin-top: 0px;"
                  "}"
                  "")
                  .arg(m_tabMargin.top())
                  .arg(m_tabMargin.right())
                  .arg(m_tabMargin.left())
                  .arg(m_tabMargin.bottom()));
}

QMargins SARibbonTabBar::tabMargin() const
{
    return m_tabMargin;
}

void SARibbonTabBar::setTabMargin(const QMargins &tabMargin)
{
    m_tabMargin = tabMargin;
}
