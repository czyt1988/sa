#include "SAValueDataTableView.h"


SAValueDataTableView::SAValueDataTableView(QWidget *par):SATableView(par)
  ,m_ctrlVFun(nullptr)
{
}

void SAValueDataTableView::setCtrlVFunPtr(SAValueDataTableView::FunPtrCtrlV fun)
{
    m_ctrlVFun = fun;
}

SAValueDataTableView::FunPtrCtrlV SAValueDataTableView::getCtrlVFunPtr() const
{
    return m_ctrlVFun;
}

void SAValueDataTableView::onCtrlVPressed()
{
    if(m_ctrlVFun)
        m_ctrlVFun();
}


