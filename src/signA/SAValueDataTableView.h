#ifndef SAVALUEDATATABLEVIEW_H
#define SAVALUEDATATABLEVIEW_H
#include "SATableView.h"
#include <functional>
class SAValueDataTableViewPrivate;
///
/// \brief 用于数据显示的table view
///
class SAValueDataTableView : public SATableView
{
    Q_OBJECT
public:
    SAValueDataTableView(QWidget* par = Q_NULLPTR);
    typedef std::function<void()> FunPtrCtrlV;
    void setCtrlVFunPtr(FunPtrCtrlV fun);
    FunPtrCtrlV getCtrlVFunPtr() const;
protected:
    //处理复制
    void onCtrlVPressed();
private:
    FunPtrCtrlV m_ctrlVFun;
};

#endif // SADATATABLEVIEW_H
