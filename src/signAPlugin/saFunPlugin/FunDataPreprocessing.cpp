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
#include "SAFigureOptCommands.h"
#include "SAChart.h"
#include "qwt_symbol.h"
#include <QMdiSubWindow>
#include "SAGUIGlobalConfig.h"
#include "ui_opt.h"
#include "czyAlgorithm.h"
#include "SARandColorMaker.h"
#define TR(str)\
    QApplication::translate("FunDataPreprocessing", str, 0)

void FunDataPreprocessing::sigmaDetect()
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        saUI->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    double sigma;
    bool isMark,isChangedPlot;
    if(!getSigmaDetectPorperty(sigma,&isMark,&isChangedPlot))
    {
        return;
    }
    if(!isMark && !isChangedPlot)
    {
        return;
    }
    QStringList infos;
    QScopedPointer<SAFigureOptCommand> topCmd(new SAFigureOptCommand(chart,QString("sigma %1").arg(sigma)));
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QwtSeriesStore<QPointF>* series = dynamic_cast<QwtSeriesStore<QPointF>*>(item);
        if(nullptr == series)
        {
            continue;
        }
        QVector<double> xs,ys;
        if(!chart->getXYData(xs,ys,item))
        {
            continue;
        }

        QString info;
        QString title = item->title().text();
        QVector<int> indexs;
        saFun::sigmaDenoising(xs,ys,sigma,indexs);
        info = QString("sigma(\"%1\") out range datas count:%2").arg(title).arg(indexs.size());
        infos.append(info);
        if(0 == indexs.size())
        {
            continue;
        }
        if(isMark)
        {
            QVector<double> oxs,oys;
            czy::copy_inner_indexs(xs.begin(),indexs.begin(),indexs.end(),std::back_inserter(oxs));
            czy::copy_inner_indexs(ys.begin(),indexs.begin(),indexs.end(),std::back_inserter(oys));
            QwtPlotCurve* curs = new QwtPlotCurve(QString("%1_outSigmaMarker").arg(title));
            curs->setSamples(oxs,oys);
            SAChart::setCurvePenStyle(curs,Qt::NoPen);
            QwtSymbol* sym = new QwtSymbol(QwtSymbol::XCross);
            sym->setColor(SARandColorMaker::getCurveColor());
            sym->setSize(QSize(6,6));
            curs->setSymbol(sym);
            new SAFigureChartItemAddCommand(chart
                                            ,curs
                                            ,QString("%1 - sigma out rang").arg(title)
                                            ,topCmd.data());
        }
        if(isChangedPlot)
        {
            QVector<int> allIndex;
            QVector<int> innerIndex;
            const int count = xs.size();
            innerIndex.resize(count);
            allIndex.reserve(count);
            for(int i=0;i<count;++i)
            {
                allIndex.append(i);
            }
            czy::copy_out_of_indexs(allIndex.begin(),allIndex.end(),indexs.begin(),indexs.end(),std::back_inserter(innerIndex));
            QVector<double> oxs,oys;
            czy::copy_inner_indexs(xs.begin(),innerIndex.begin(),innerIndex.end(),std::back_inserter(oxs));
            czy::copy_inner_indexs(ys.begin(),innerIndex.begin(),innerIndex.end(),std::back_inserter(oys));
            QVector<QPointF> oxys;
            saFun::makeVectorPointF(oxs,oys,oxys);
            new SAFigureChangeXYSeriesDataCommand(chart
                                                 ,series
                                                 ,TR("%1 sigma %2").arg(title).arg(sigma)
                                                 ,oxys
                                                 ,topCmd.data());
        }
    }
    if(topCmd->childCount() > 0)
    {
        chart->appendCommand(topCmd.take());
        saUI->showNormalMessageInfo(infos.join('\n'));
    }
}

void FunDataPreprocessing::pointSmooth()
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        saUI->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    int m,n;
    if(!getPointSmoothPorperty(m,n))
    {
        return;
    }
    QStringList infos;
    QScopedPointer<SAFigureOptCommand> topCmd(new SAFigureOptCommand(chart,QString("%1 point %2 power").arg(m).arg(n)));
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QString title = item->title().text();
        QwtSeriesStore<QPointF>* series = dynamic_cast<QwtSeriesStore<QPointF>*>(item);
        if(nullptr == series)
        {
            continue;
        }
        QVector<double> xs,ys;

        if(!chart->getXYData(xs,ys,item))
        {
            continue;
        }
        if(!chart->getXYData(xs,ys,item))
        {
            continue;
        }
        QVector<double> res;
        if(!saFun::pointSmooth(ys,m,n,res))
        {
            continue;
        }
        QVector<QPointF> xys;
        saFun::makeVectorPointF(xs,res,xys);
        new SAFigureChangeXYSeriesDataCommand(chart
                                             ,series
                                             ,TR("%1 m%2n%3").arg(title).arg(m).arg(n)
                                             ,xys
                                             ,topCmd.data());

        infos.append(TR("%1 m%2n%3 smooth").arg(title).arg(m).arg(n));
    }
    if(topCmd->childCount() > 0)
    {
        chart->appendCommand(topCmd.take());
        saUI->showNormalMessageInfo(infos.join('\n'));
    }
}


///
/// \brief sigma异常值判断
///
void FunDataPreprocessing::sigmaDetectInValue()
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
/// \brief m点n次滤波的设置
/// \param m m值
/// \param n n值
/// \param isNewPlotOrChangPlot 是否新建绘图或者改变原来绘图
/// \return
///
bool FunDataPreprocessing::getPointSmoothPorperty(int &m, int &n)
{
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    dlg.appendEnumProperty("m",TR("points")
                           ,{"3","5","7"}
                           ,0
                           ,TR("set smooth points"));
    dlg.appendEnumProperty("n",TR("power")
                           ,{"1","2","3"}
                           ,0
                           ,TR("set smooth power"));

    if(QDialog::Accepted != dlg.exec())
    {
        return false;
    }
    m = 3;
    switch(dlg.getDataByID<int>("m"))
    {
    case 0:m = 3;break;
    case 1:m = 5;break;
    case 2:m = 7;break;
    default:m = 3;
    }
    n = 1;
    switch(dlg.getDataByID<int>("n"))
    {
    case 0:n = 1;break;
    case 1:n = 2;break;
    case 2:n = 3;break;
    default:n = 1;
    }

    return true;
}
///
/// \brief m点n次滤波
///
void FunDataPreprocessing::pointSmoothInValue()
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

bool FunDataPreprocessing::getSigmaDetectPorperty(double &sigma,bool* isMark,bool* isChangPlot)
{
    SAPropertySetDialog dlg(saUI->getMainWindowPtr(),static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    dlg.appendDoubleProperty("sigma",TR("sigma")
                             ,0,std::numeric_limits<double>::max()
                             ,3
                           ,TR("set sigma value"));
    if(isChangPlot)
    {
        dlg.appendBoolProperty("isChangPlot"
                               ,TR("is modify plot curve")
                               ,true
                               ,TR("if checked,will use the new datas chang the curve"));
    }
    if(isMark)
    {
        dlg.appendBoolProperty("isMark"
                               ,TR("is mark out rang datas")
                               ,false
                               ,TR("if checked,will plot the out range datas"));
    }

    if(QDialog::Accepted != dlg.exec())
    {
        return false;
    }
    sigma = dlg.getDataByID<double>("sigma");
    if(isMark)
        *isMark = dlg.getDataByID<double>("isMark");
    if(isChangPlot)
        *isChangPlot = dlg.getDataByID<double>("isChangPlot");
    return true;
}

