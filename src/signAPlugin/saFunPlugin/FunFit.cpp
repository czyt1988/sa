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
#define TR(str)\
    QApplication::translate("FunFit", str, 0)

FunFit::FunFit()
{

}

void FunFit::polyfitInChart()
{
    SAChart2D* chart = saUI->getCurSubWindowChart();
    if(!chart)
    {
        saUI->showWarningMessageInfo(TR("you should select a chart at first"));
        saUI->raiseMessageInfoDock();
        return;
    }
    QList<QwtPlotItem*> curs = chart->getCurrentSelectItems();
    if(0 == curs.size())
    {
        curs = saUI->selectPlotItems(chart,SAChart2D::getXYSeriesItemsRTTI().toSet());
    }
    if(curs.size() <= 0)
    {
        return;
    }
    const QString idPolyN = "polyN";
    SAPropertySetDialog dlg(saUI->getMainWindowPtr()
                            ,static_cast<SAPropertySetDialog::BrowserType>(SAGUIGlobalConfig::getDefaultPropertySetDialogType()));
    dlg.appendGroup(TR("property set"));
    dlg.appendIntProperty(idPolyN,TR("order of poly")
                             ,1,20
                             ,1
                           ,TR("order of poly fit"));
    if(QDialog::Accepted != dlg.exec())
    {
        return;
    }
    const int order = dlg.getDataByID<int>(idPolyN);
    QString strDes("");
    for (int i = 0;i<curs.size();++i)
    {
        QwtPlotItem* item = curs[i];
        QVector<double> xs,ys;
        if(!chart->getXYDataInRange(xs,ys,item,true))
        {
            continue;
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
        const int factorSize = factor->getSize();
        QString fun;
        for (int j = 0;j<factorSize;++j)
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
