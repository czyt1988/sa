#ifndef SAVALUEDATATABLEVIEW_H
#define SAVALUEDATATABLEVIEW_H
#include "SATableView.h"
///
/// \brief 用于数据显示的table view
///
class SAValueDataTableView : public SATableView
{
    Q_OBJECT
public:
    SAValueDataTableView(QWidget* par = Q_NULLPTR);
};

#endif // SADATATABLEVIEW_H
