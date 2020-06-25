#include "FunFit.h"
#include "sa_fun_fit.h"
#include "SAUIReflection.h"
#include "SAUIInterface.h"
#include "SAUIHelper.h"
#include "SAPropertySetDialog.h"
#include "SAAbstractDatas.h"
#include "SAValueManager.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include <QMdiSubWindow>
#include "qwt_plot_curve.h"
#include "qwt_series_store.h"
#include "SAGUIGlobalConfig.h"
#include "ui_opt.h"
#include <QApplication>
#include "SADataConver.h"
#define TR(str)\
    QApplication::translate("FunFit", str, 0)
void polyfitInChart(SAUIInterface* ui);
void polyfitInValue(SAUIInterface* ui);
bool getPolyfitConfig(int &order,SAUIInterface* ui);
void splitPointF(const QVector<QPointF>& xys,QVector<double>& xs,QVector<double>& ys);
void splitPointF(const QVector<QPointF>& xys,QVector<double>& xs,QVector<double>& ys)
{
    xs.reserve(xys.size());
    ys.reserve(xys.size());
    std::for_each(xys.begin(),xys.end(),[&xs,&ys](const QPointF& p){
       xs.append(p.x());
       ys.append(p.y());
    });
}



void polyfit(SAUIInterface* ui)
{
    SAUIInterface::LastFocusType ft = ui->lastFocusWidgetType();
    if(SAUIInterface::FigureWindowFocus == ft)
    {
        polyfitInChart(ui);
    }
    else
    {
        polyfitInValue(ui);
    }
}

void polyfitInChart(SAUIInterface* ui)
{
    QList<QwtPlotItem*> curs;
    SAChart2D* chart = filter_xy_series(ui,curs);
    if(nullptr == chart || curs.size() <= 0)
    {
        ui->showMessageInfo(TR("unsupport chart items"),SA::WarningMessage);
        return;
    }
    int order = 1;
    if(!getPolyfitConfig(order,ui))
    {
        return;
    }

    QString strDes("");
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QVector<double> xs,ys;
        if(chart->isRegionVisible())
        {
            if(!chart->getXYDataInRange(&xs,&ys,nullptr,item))
            {
                continue;
            }
        }
        else
        {
            if(!chart->getXYData(&xs,&ys,item))
            {
                continue;
            }
        }
        QString title = item->title().text();

        std::shared_ptr<SAVectorDouble> factor;//拟合的系数
        std::shared_ptr<SATableVariant> info;//拟合的误差参数
        std::tie(factor,info) = saFun::polyfit(xs,ys,order);
        if(nullptr == factor)
        {
            continue;
        }
        info->setName(QString("%1_%2PolyFitInfo").arg(title).arg(order));
        std::shared_ptr<SAVectorPointF> pfitVal = SAValueManager::makeData<SAVectorPointF>(QString("%1_%2PolyFit").arg(title).arg(order));
        saFun::polyval(xs,factor.get(),pfitVal.get());
        saValueManager->addData(info);
        saValueManager->addData(pfitVal);
        //
        strDes += "<p>";
        strDes += TR("%1-Polynomial Fittin, order:%2 ")
                .arg(title).arg(order);
        strDes += "<br/>";
        const size_t factorSize = factor->getSize();
        QString fun;
        for (size_t j = 0;j<factorSize;++j)
        {
            double f = factor->getAt({j}).toDouble();
            if (0 == j)
            {
                fun = QString("y = %1").arg(factor->getAt({j}).toString());
                continue;
            }
            if (1 == j)
            {
                if (f>0)
                    fun += QString("+%1x").arg(f);
                else
                    fun += QString("%1x").arg(f);
                continue;
            }
            if(f>0)
                fun += QString("+%1x^%2").arg(f).arg(j);
            else
                fun += QString("%1x^%2").arg(f).arg(j);
        }
        strDes += (TR("fitting results:")+fun);
        strDes += "<br/>";
        //残差,残差平方和,回归平方和,均方根误差

        strDes += TR("error:");
        strDes += "<br/>";

        strDes += TR("SSR(Regression Square Sum)=%1").arg(info->getAt({0,1}).toString());
        strDes += "<br/>";

        strDes += TR("SSE(Sum of Squares for Error)=%1").arg(info->getAt({1,1}).toString());
        strDes += "<br/>";

        strDes += TR("SST=%1").arg(info->getAt({2,1}).toString());
        strDes += "<br/>";

        strDes += TR("RMSE(Root mean square error)=%1").arg(info->getAt({3,1}).toString());
        strDes += "<br/>";

        strDes += TR("R-square(coefficient of determination)=%1").arg(info->getAt({4,1}).toString());
        strDes += "<br/>";

        strDes += TR("Goodness=%1").arg(info->getAt({5,1}).toString());
        strDes += "</p>";

        chart->addCurve(pfitVal.get());
        saUI->showNormalMessageInfo(strDes);
    }

}

void polyfitInValue(SAUIInterface* ui)
{
    SAAbstractDatas* data = ui->getSelectSingleData();
    if(nullptr == data)
    {
        return;
    }
    QVector<QPointF> xys;
    if(!SADataConver::converToPointFVector(data,xys))
    {
        ui->showWarningMessageInfo(TR("data:[\"%1\"] can not to conver to points array").arg(data->getName()));
        ui->raiseMessageInfoDock();
        return;
    }
    int order = 1;
    if(!getPolyfitConfig(order,ui))
    {
        return;
    }
    QVector<double> xs,ys;
    splitPointF(xys,xs,ys);
    std::shared_ptr<SAVectorDouble> factor;//拟合的系数
    std::shared_ptr<SATableVariant> info;//拟合的误差参数
    std::tie(factor,info) = saFun::polyfit(xs,ys,order);
    if(nullptr == factor)
    {
        return;
    }
    QString title = data->getName();
    info->setName(QString("%1_%2PolyFitInfo").arg(title).arg(order));
    std::shared_ptr<SAVectorPointF> pfitVal = SAValueManager::makeData<SAVectorPointF>(QString("%1_%2PolyFit").arg(title).arg(order));
    saFun::polyval(xs,factor.get(),pfitVal.get());
    saValueManager->addData(info);
    saValueManager->addData(pfitVal);
    //
    QString strDes;
    strDes += "<p>";
    strDes += TR("%1-Polynomial Fittin, order:%2 ")
            .arg(title).arg(order);
    strDes += "<br/>";
    const size_t factorSize = factor->getSize();
    QString fun;
    for (size_t j = 0;j<factorSize;++j)
    {
        double f = factor->getAt({j}).toDouble();
        if (0 == j)
        {
            fun = QString("y = %1").arg(factor->getAt({j}).toString());
            continue;
        }
        if (1 == j)
        {
            if (f>0)
                fun += QString("+%1x").arg(f);
            else
                fun += QString("%1x").arg(f);
            continue;
        }
        if(f>0)
            fun += QString("+%1x^%2").arg(f).arg(j);
        else
            fun += QString("%1x^%2").arg(f).arg(j);
    }
    strDes += (TR("fitting results:")+fun);
    strDes += "<br/>";
    //残差,残差平方和,回归平方和,均方根误差

    strDes += TR("error:");
    strDes += "<br/>";

    strDes += TR("SSR(Regression Square Sum)=%1").arg(info->getAt({0,1}).toString());
    strDes += "<br/>";

    strDes += TR("SSE(Sum of Squares for Error)=%1").arg(info->getAt({1,1}).toString());
    strDes += "<br/>";

    strDes += TR("SST=%1").arg(info->getAt({2,1}).toString());
    strDes += "<br/>";

    strDes += TR("RMSE(Root mean square error)=%1").arg(info->getAt({3,1}).toString());
    strDes += "<br/>";

    strDes += TR("R-square(coefficient of determination)=%1").arg(info->getAt({4,1}).toString());
    strDes += "<br/>";

    strDes += TR("Goodness=%1").arg(info->getAt({5,1}).toString());
    strDes += "</p>";

    saUI->showNormalMessageInfo(strDes);
}

bool getPolyfitConfig(int &order,SAUIInterface* ui)
{
    const QString idPolyN = "polyN";
    SAPropertySetDialog dlg(saUI->getMainWindowPtr()
                            ,SAPropertySetDialog::GroupBoxType);
    dlg.appendGroup(TR("property set"));
    dlg.appendIntProperty(idPolyN,TR("order of poly")
                             ,1,20
                             ,1
                           ,TR("order of poly fit"));
    if(QDialog::Accepted != dlg.exec())
    {
        return false;
    }
    order = dlg.getDataByID<int>(idPolyN);

    return true;
}


