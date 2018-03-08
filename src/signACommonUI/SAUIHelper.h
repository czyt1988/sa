#ifndef SAUIHELPER_H
#define SAUIHELPER_H
#include <QList>
#include "SACommonUIGlobal.h"
#include <QModelIndexList>
class QtVariantProperty;
class SAPropertySetDialog;
class SAFigureWindow;
///
/// \brief 封装一些常见的ui操作
///
class SA_COMMON_UI_EXPORT SAUIHelper
{
public:
    SAUIHelper();
    //把当前的绘图插入到属性表中
    static QtVariantProperty* appenFigureListInPropertyDialog(SAPropertySetDialog* dlg, QList<SAFigureWindow *> *figList
                                                              , const QString& id
                                                              , const QString& propName
                                                              , const QString& tooltips = QString()
                                                              );

    //=================================================================
    //从QModelIndexList获取排完序的列号
    static QList<int> getColumnsFromModelList(const QModelIndexList& indexs);
    //从QModelIndexList获取排完序的行号
    static QList<int> getRowsFromModelList(const QModelIndexList& indexs);

};

#endif // SAUIHELPER_H
