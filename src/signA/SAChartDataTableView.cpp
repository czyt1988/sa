#include "SAChartDataTableView.h"

SAChartDataTableView::SAChartDataTableView(QWidget *par):SATableView(par)
  ,m_ctrlVFun(nullptr)
{

}

void SAChartDataTableView::setCtrlVFunPtr(SAChartDataTableView::FunPtrCtrlV fun)
{
    m_ctrlVFun = fun;
}

SAChartDataTableView::FunPtrCtrlV SAChartDataTableView::getCtrlVFunPtr() const
{
    return m_ctrlVFun;
}

void SAChartDataTableView::onCtrlVPressed()
{
    if(m_ctrlVFun)
    {
        m_ctrlVFun();
    }
}
