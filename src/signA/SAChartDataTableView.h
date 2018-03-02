#ifndef SACHARTDATATABLEVIEW_H
#define SACHARTDATATABLEVIEW_H
#include "SATableView.h"
#include <functional>

///
/// \brief The SAChartDataTableView class
///
class SAChartDataTableView : public SATableView
{
    Q_OBJECT
public:
    SAChartDataTableView(QWidget* par = Q_NULLPTR);
    typedef std::function<void()> FunPtrCtrlV;
    void setCtrlVFunPtr(FunPtrCtrlV fun);
    FunPtrCtrlV getCtrlVFunPtr() const;
protected:
    //处理复制
    void onCtrlVPressed();
private:
    FunPtrCtrlV m_ctrlVFun;
};

#endif // SACHARTDATATABLEVIEW_H
