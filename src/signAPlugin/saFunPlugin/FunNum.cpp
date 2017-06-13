#include "FunNum.h"
#include <memory>
#include "sa_fun_core.h"
#include "sa_fun_num.h"
#include "SAAbstractDatas.h"
#include "SAPropertySetDialog.h"
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include "SAAbstractDatas.h"
#include "SAVariantDatas.h"
#include "SAValueManager.h"
#include "SAFigureWindow.h"

#include "SAVectorInterval.h"
#include "SAGUIGlobalConfig.h"
#include <QMdiSubWindow>
#define TR(str)\
    QApplication::translate("FunStatistics", str, 0)

FunNum::FunNum()
{

}
///
/// \brief 求统计参数
///
void FunNum::statistics()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    std::shared_ptr<SATableVariant> res = saFun::statistics(data);
    if(nullptr == res)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        saUI->raiseMessageInfoDock();
        return;
    }
    res->setName(TR("%1_statistics").arg(data->getName()));
    saValueManager->addData(res);
    saUI->showNormalMessageInfo(TR("%1 sum is %2").arg(data->getName()).arg(res->getName()));

}
///
/// \brief 求差分
///
void FunNum::diff()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    auto tmp = dlg.appendIntProperty(TR("diff count")
                             ,1,std::numeric_limits<int>::max()
                             ,1,TR("diff count"));
    dlg.recorder("diff",tmp);
    int diffCount = dlg.getDataByID<int>("diff");
    std::shared_ptr<SAVectorDouble> res = saFun::diff(data,diffCount);
    if(nullptr == res)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        saUI->raiseMessageInfoDock();
        return;
    }
    res->setName(TR("%1_diff%2").arg(data->getName()).arg(diffCount));
    saValueManager->addData(res);
    saUI->showNormalMessageInfo(TR("%1 diff(%2) is %3")
                                .arg(data->getName())
                                .arg(diffCount)
                                .arg(res->getName()));
}
///
/// \brief 求均值
///
void FunNum::mean()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    std::shared_ptr<SAVariantDatas> res = saFun::mean(data);
    if(nullptr == res)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        saUI->raiseMessageInfoDock();
        return;
    }
    res->setName(TR("%1-mean").arg(data->getName()));
    saValueManager->addData(res);
    saUI->showNormalMessageInfo(TR("%1 sum is %2").arg(data->getName()).arg(res->toData<double>()));
}
///
/// \brief 求和
///
void FunNum::sum()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    std::shared_ptr<SAVariantDatas> res = saFun::sum(data);
    if(nullptr == res)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        saUI->raiseMessageInfoDock();
        return;
    }
    res->setName(TR("%1-sum").arg(data->getName()));
    saValueManager->addData(res);
    saUI->showNormalMessageInfo(TR("%1 sum is %2").arg(data->getName()).arg(res->toData<double>()));
}


///
/// \brief 频率统计
///
void FunNum::hist()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    const QString idHistCount = "histCount";
    const QString idIsPlot = "isPlot";
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    dlg.appendIntProperty(idHistCount,TR("hist count")
                          ,1,std::numeric_limits<int>::max()
                          ,100,TR("set hist count"));
    dlg.appendGroup(TR("plot set"));
    dlg.appendBoolProperty(idIsPlot,TR("is plot")
                           ,true
                           ,TR("is plot after calc"));
    if(QDialog::Accepted != dlg.exec())
    {
        return;
    }
    auto res = saFun::hist(data,dlg.getDataByID<int>(idHistCount));
    if(nullptr == res)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        saUI->raiseMessageInfoDock();
        return;
    }
    saValueManager->addData(res);
    if(dlg.getDataByID<bool>(idIsPlot))
    {
        QMdiSubWindow* sub = saUI->createFigureWindow();
        SAFigureWindow* fig = saUI->getFigureWidgetFromMdiSubWindow(sub);
        fig->create2DPlot();
        if(fig)
        {
            fig->addBar(res.get());
        }
        saUI->raiseMainDock();
        sub->show();
    }
}
