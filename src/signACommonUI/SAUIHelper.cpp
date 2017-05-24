#include "SAUIHelper.h"
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include "SAPropertySetDialog.h"
#include "SAFigureWindow.h"
#include <QApplication>
#define TR(str)\
    QApplication::translate("SAUIHelper", str, 0)

SAUIHelper::SAUIHelper()
{

}
///
/// \brief 在属性对话框中显示绘图窗口列表
/// \param dlg 属性对话框
/// \param figList 绘图窗口列表
/// \param id 属性对话框id
/// \param propName 属性名
/// \param tooltips 属性的说明
/// \return
///
QtVariantProperty *SAUIHelper::appenFigureListInPropertyDialog(SAPropertySetDialog *dlg, QList<SAFigureWindow *> *figList, const QString &id, const QString &propName, const QString &tooltips)
{
    *figList = saUI->getFigureWindowList();
    SAFigureWindow* curFigure = saUI->getCurrentFigureWindow();
    QStringList nameList = {TR("none")};
    int selectIndex = -1;
    for(int i=0;i<figList->size();++i)
    {
        if(curFigure == figList->at(i))
        {
            selectIndex = i;
        }
        nameList.append(figList->at(i)->windowTitle());
    }
    return dlg->appendEnumProperty(id,propName,nameList
                            ,selectIndex >= 0 ? (selectIndex+1) : 0
                                                ,tooltips);

}
