#ifndef SAUIHELPER_H
#define SAUIHELPER_H
#include <QList>
#include "SACommonUIGlobal.h"
class QtVariantProperty;
class SAPropertySetDialog;
class SAFigureWindow;

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
};

#endif // SAUIHELPER_H
