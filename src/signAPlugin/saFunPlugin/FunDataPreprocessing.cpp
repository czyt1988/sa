#include "FunDataPreprocessing.h"
#include <QApplication>
#include <QVariant>
#include "sa_fun_core.h"
#include "sa_fun_preproc.h"
#include "SAPropertySetDialog.h"
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include "SAUIHelper.h"
#include "SAAbstractDatas.h"
#include "SAVariantDatas.h"
#include "SAValueManager.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include <QMdiSubWindow>
#include "SAGUIGlobalConfig.h"

#define TR(str)\
    QApplication::translate("FunDataPreprocessing", str, 0)

///
/// \brief sigma异常值判断
///
void FunDataPreprocessing::sigmaDetect()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    QList<SAFigureWindow*> figList;//用于保存当前主界面所有的绘图窗口
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    const QString idSigma = "sigma";
    const QString idPlotInNewFigure = "isPlotInNewFigure";
    const QString idPlotOriginDataAndOutRangDataInNewFigure = "isPlotOriginDataAndOutRangDataInNewFigure";
    const QString idPlotDenoiseDataInNewFigure = "isPlotDenoiseDataInNewFigure";
    const QString idFigWindowList = "figureWindowList";
    const QString idIsPlotDenoiseDataInCurrentFig = "isPlotDenoiseDataInCurrentFig";
    const QString idIsPlotOutRangMarkInCurrentFig = "isPlotOutRangMarkInCurrentFig";
    dlg.appendGroup(TR("property set"));
    dlg.appendDoubleProperty(idSigma,TR("sigma")
                             ,0,std::numeric_limits<double>::max()
                             ,3
                           ,TR("set sigma value"));
    dlg.appendGroup(TR("out put set"));
    dlg.appendBoolProperty("isSaveOutRangIndex",TR("save out rang index"),false
                           ,TR("if this select true,will return the index which is out of sigma rang"));
    dlg.appendBoolProperty("isSaveOutRangData",TR("save out rang data"),false
                           ,TR("if this select true,will return the datas which is out of sigma rang"));
    dlg.appendGroup(TR("new figure set"));
    //
    auto propPlot = dlg.appendBoolProperty(idPlotInNewFigure,TR("plot in new chart"),true
                           ,TR("if this select true,will plot data in new figure"));
    auto propPlotOrigin = dlg.appendBoolProperty(idPlotOriginDataAndOutRangDataInNewFigure,TR("plot origin data"),true
                           ,TR("if this select true,will plot origin data in figure and will plot the out rang data"));
    auto propPlotDenoise = dlg.appendBoolProperty(idPlotDenoiseDataInNewFigure,TR("plot Denoise data"),true
                           ,TR("if this select true,will plot origin data in figure"));
    auto fun1 = [&](SAPropertySetDialog* dlg,QtProperty* prop,const QVariant& var){
        Q_UNUSED(dlg);
        Q_UNUSED(prop);
        propPlotOrigin->setEnabled(var.toBool());
        propPlotDenoise->setEnabled(var.toBool());
    };
    dlg.setPropertyChangEvent(propPlot,fun1);
    dlg.appendGroup(TR("current figure set"));
    auto propPlotCur = dlg.appendBoolProperty("isPlotInCurrentFigure",TR("plot in current chart"),false
                           ,TR("if this select true,will plot data in user select figure"));
    auto propFigSelVar = SAUIHelper::appenFigureListInPropertyDialog(&dlg
                                                         ,&figList
                                                        ,idFigWindowList
                                                        ,TR("select figure")
                                                        ,TR("choose the figure window to plot the out rang point"));
    propFigSelVar->setEnabled(false);
    auto propPlotDenoiseCurFig = dlg.appendBoolProperty(idIsPlotDenoiseDataInCurrentFig
                                                        ,TR("plot denoise data"),true
                                                      ,TR("if this select true,will plot denoise data in select figure"));
    propPlotDenoiseCurFig->setEnabled(false);
    auto propPlotOutRangMarkCurFig = dlg.appendBoolProperty(idIsPlotOutRangMarkInCurrentFig,TR("plot out rang data"),true
                                                      ,TR("if this select true,will plot out rang data in select figure"));
    propPlotOutRangMarkCurFig->setEnabled(false);
    auto fun2 = [&](SAPropertySetDialog* dlg,QtProperty* prop,const QVariant& var){
        Q_UNUSED(dlg);
        Q_UNUSED(prop);
        propFigSelVar->setEnabled(var.toBool());
        propPlotDenoiseCurFig->setEnabled(var.toBool());
        propPlotOutRangMarkCurFig->setEnabled(var.toBool());
    };
    dlg.setPropertyChangEvent(propPlotCur,fun2);
    if(QDialog::Accepted != dlg.exec())
    {
        return;
    }
    std::shared_ptr<SAAbstractDatas> waveDenoising = nullptr;
    std::shared_ptr<SAAbstractDatas> waveRemove = nullptr;
    std::shared_ptr<SAVectorInt> outRangIndex = nullptr;
    std::shared_ptr<SAVectorInt> inRangIndex = nullptr;
    std::tie(waveDenoising,waveRemove,outRangIndex,inRangIndex)
            = saFun::sigmaDenoising(data,dlg.getDataByID<double>(idSigma));

    if(nullptr == waveDenoising || nullptr == waveRemove)
    {
        saUI->showErrorMessageInfo(saFun::getLastErrorString());
        saUI->raiseMessageInfoDock();
        return;
    }
    //========================
    //save data to value manager
    //========================
    saValueManager->addData(waveDenoising);
    if(dlg.getDataByID<bool>("isSaveOutRangIndex"))
    {
        saValueManager->addData(outRangIndex);
    }
    if(dlg.getDataByID<bool>("isSaveOutRangData"))
    {
        saValueManager->addData(waveRemove);
    }
    //========================
    //plot
    //========================
    if(dlg.getDataByID<bool>(idPlotInNewFigure))
    {
        QMdiSubWindow* sub = saUI->createFigureWindow();
        SAFigureWindow* fig = saUI->getFigureWidgetFromMdiSubWindow(sub);
        SAChart2D* chart = fig->current2DPlot();
        if(nullptr == chart)
        {
            chart = fig->create2DPlot();
        }
        if(fig)
        {
            if(dlg.getDataByID<bool>(idPlotOriginDataAndOutRangDataInNewFigure))
            {
                chart->addCurve(data);
                if(SA::VectorPoint == data->getType() )
                {
                    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addCurve(waveRemove.get()));
                    if(cur)
                    {
                        SAChart::setCurveSymbol(cur,QwtSymbol::Cross,QSize(5,5));
                        SAChart::setCurveLinePenStyle(cur,Qt::NoPen);
                    }
                }
                else
                {
                    QwtPlotCurve* cur  = (QwtPlotCurve*)(chart->addCurve(outRangIndex.get(),waveRemove.get()));
                    if(cur)
                    {
                        SAChart::setCurveSymbol(cur,QwtSymbol::Cross,QSize(5,5));
                        SAChart::setCurveLinePenStyle(cur,Qt::NoPen);
                    }
                }
            }
            if(dlg.getDataByID<bool>(idPlotDenoiseDataInNewFigure))
            {
                if(SA::VectorPoint == data->getType() )
                {
                    chart->addCurve(waveDenoising.get());
                }
                else
                {
                    chart->addCurve(inRangIndex.get(),waveDenoising.get());
                }
            }
        }
        saUI->raiseMainDock();
        sub->show();
    }
    if(dlg.getDataByID<bool>("isPlotInCurrentFigure"))
    {
        if(figList.size() <= 0)
        {
            return;
        }
        int index = dlg.getDataByID<int>(idFigWindowList);
        if(index <= 0)
        {
            return;
        }
        if((index-1) >= figList.size())
        {
            return;
        }
        SAFigureWindow* fig = figList[index-1];
        SAChart2D* chart = fig->current2DPlot();
        if(nullptr == chart)
        {
            chart = fig->create2DPlot();
        }
        if(dlg.getDataByID<bool>(idIsPlotDenoiseDataInCurrentFig))
        {
            if(SA::VectorPoint == data->getType() )
            {
                chart->addCurve(waveDenoising.get());
            }
            else
            {
                chart->addCurve(inRangIndex.get(),waveDenoising.get());
            }
        }
        if(dlg.getDataByID<bool>(idIsPlotOutRangMarkInCurrentFig))
        {
            QwtPlotCurve* cur = nullptr;
            if(SA::VectorPoint == data->getType() )
            {
                cur = (QwtPlotCurve*)(chart->addCurve(waveRemove.get()));
            }
            else
            {
                cur = (QwtPlotCurve*)(chart->addCurve(outRangIndex.get(),waveRemove.get()));
            }
            if(cur)
            {
                SAChart::setCurveSymbol(cur,QwtSymbol::Cross,QSize(5,5));
                SAChart::setCurveLinePenStyle(cur,Qt::NoPen);
            }
        }
    }
}
///
/// \brief m点n次滤波
///
void FunDataPreprocessing::pointSmooth()
{
    SAAbstractDatas* data = saUI->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    const QString idPoint = "points";
    const QString idPower = "power";
    const QString idIsPlot = "isPlot";
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    dlg.appendEnumProperty(idPoint,TR("points")
                           ,{"3","5","7"}
                           ,0
                           ,TR("set smooth points"));
    dlg.appendEnumProperty(idPower,TR("power")
                           ,{"1","2","3"}
                           ,0
                           ,TR("set smooth power"));
    dlg.appendGroup(TR("plot set"));
    dlg.appendBoolProperty(idIsPlot,TR("is plot")
                           ,true
                           ,TR("is plot after calc"));
    if(QDialog::Accepted != dlg.exec())
    {
        return;
    }
    int point = 3;
    switch(dlg.getDataByID<int>(idPoint))
    {
    case 0:point = 3;break;
    case 1:point = 5;break;
    case 2:point = 7;break;
    default:point = 3;
    }
    int power = 1;
    switch(dlg.getDataByID<int>(idPower))
    {
    case 0:power = 1;break;
    case 1:power = 2;break;
    case 2:power = 3;break;
    default:power = 1;
    }
    std::shared_ptr<SAAbstractDatas> res = saFun::pointSmooth(data,point,power);
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
        SAChart2D* chart = fig->current2DPlot();
        if(nullptr == chart)
        {
            chart = fig->create2DPlot();
        }
        if(chart)
        {
            chart->addCurve(res.get());
        }
        saUI->raiseMainDock();
        sub->show();
    }
}

