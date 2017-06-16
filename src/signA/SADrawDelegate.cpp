#include "SADrawDelegate.h"
#include "mainwindow.h"

#include <QTime>

//#include <SAPlotChart.h>
#include "SAChart2D.h"
#include "CurveSelectDialog.h"
//#include <SAChartWidget.h>
#include "SAFigureWindow.h"
#include "czyMath_Fit.h"
#include "SAValueManagerModel.h"
#include "SARandColorMaker.h"
#include "SASeries.h"
#include "SAData.h"
SADrawDelegate::SADrawDelegate(MainWindow* wnd):SAMainWindowDelegate(wnd)
  ,m_nUserChartCount(0)
{
}

SADrawDelegate::~SADrawDelegate()
{

}

void SADrawDelegate::drawTrend()
{
    MainWindow* m = getMainWindow ();
    QList<SAAbstractDatas*> datas = m->getSeletedDatas();
    //验证是否可以绘图
    if(datas.size()<=0)
    {
        return;
    }
    SAMdiSubWindow* pSubWnd = createFigureMdiSubWidget(datas.size()==1 ? datas[0]->getName () : "");
    SAFigureWindow* pFigure = qobject_cast<SAFigureWindow*>(pSubWnd->widget());
    if(!pSubWnd && !pFigure)
        return;//任何一个指针为0都退出
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        chart = pFigure->create2DPlot();
    }
    std::for_each(datas.begin(),datas.end(),[chart](SAAbstractDatas* data){
        chart->addCurve(data,1,1);
    });

    chart->setAutoReplot(false);
    chart->setAutoReplot(true);
    chart->enablePicker(true);
    chart->enableZoomer(false);
    chart->enableGrid(true);
    pSubWnd->show();
}
///
/// \brief 对数据的绘制
/// \param data
///
QwtPlotCurve* SADrawDelegate::draw(SAAbstractDatas* data)
{
    if(nullptr == data)
    {
        return nullptr;
    }

    SAMdiSubWindow* w = createFigureMdiSubWidget(data->getName ());
    SAFigureWindow* pFigure = getFigureWidgetFromMdiSubWindow(w);
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        pFigure->create2DPlot();
    }
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addCurve(data));
    w->show ();
    return cur;
}

QwtPlotCurve *SADrawDelegate::draw(SAAbstractDatas* x, SAAbstractDatas* y, QString name)
{
    if(name.isNull () || name.isEmpty ())
    {
        name = QStringLiteral("%1-%2").arg(x->getName ()).arg(y->getName ());
    }
    SAMdiSubWindow* w = createFigureMdiSubWidget(name);
    SAFigureWindow* pFigure = getFigureWidgetFromMdiSubWindow(w);
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        pFigure->create2DPlot();
    }
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addCurve (x,y,name));
    w->show ();
    return cur;
}
///
/// \brief 绘制柱状图
/// \param barSeries 柱状图数据
/// \return
///
QwtPlotHistogram *SADrawDelegate::drawBar(SAAbstractDatas* barSeries)
{
    if(barSeries->getType () != SA::VectorInterval)
    {
        return nullptr;
    }
    SAVectorInterval* intervalSeries = static_cast<SAVectorInterval*>(barSeries);
    SAMdiSubWindow* w = createFigureMdiSubWidget(barSeries->getName ());
    SAFigureWindow* pFigure = getFigureWidgetFromMdiSubWindow(w);
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        pFigure->create2DPlot();
    }
    QwtPlotHistogram* b = (QwtPlotHistogram*)(chart->addBar (intervalSeries));
    w->show ();
    return b;
}
/////
///// \brief 一元回归分析
/////
//void SADrawDelegate::markLeastSquarePolyfit(int n)
//{
//    SAFigureWindow* fig = getMainWindow()->getCurrentFigureWindow();
//    SAChart2D* chart = fig->current2DPlot();
//    if(!chart)
//        return;
//    QList<QwtPlotCurve*> curves = CurveSelectDialog::getSelCurve(chart,getMainWindow ());

//    czy::QtApp::QWaitCursor wait;
//    Q_UNUSED(wait);
//    QString strDes("");
//    for (int i = 0;i<curves.size();++i)
//    {
//        strDes = "";
//        std::vector<double> xs,ys;
//        chart->getPlottingRegionDatas(xs,ys,curves[i]);
//        czy::Math::PolyFit ff;
//        ff.polyfit(xs,ys,n);
//        strDes += QString("\n[%1]---------------------------------------------------------------")
//                .arg(QTime::currentTime().toString("hh:mm:ss"));
//        strDes += tr("\n%1-Least Squares Polynomial Fittin,%2 order:")
//                .arg(curves[i]->title().text()).arg(n);
//        strDes += tr("\n fit rang:(%1,%2)~(%3,%4)").arg(xs.at(0)).arg(ys.at(0))
//                .arg(xs.at(xs.size()-1)).arg(ys.at(ys.size()-1));
//        QString fun("");

//        for (size_t j = 0;j<ff.getFactorSize();++j)
//        {
//            if (0 == j)
//            {
//                fun = QString("y = %1").arg(ff.getFactor(j));
//                continue;
//            }
//            if (1 == j)
//            {
//                if (ff.getFactor(j)>0)
//                    fun += QString("+%1x").arg(ff.getFactor(j));
//                else
//                    fun += QString("%1x").arg(ff.getFactor(j));
//                continue;
//            }
//            if(ff.getFactor(j)>0)
//                fun += QString("+%1x^%2").arg(ff.getFactor(j)).arg(j);
//            else
//                fun += QString("%1x^%2").arg(ff.getFactor(j)).arg(j);
//        }
//        strDes += (tr("\n fitting results:")+fun);
//        //残差,残差平方和,回归平方和,均方根误差
//        strDes += tr("\nerror:\nSSE(Sum of Squares for Error)=%1\nSSR(Regression Square Sum)=%2\nRMSE(Root mean square error)=%3")
//                .arg(ff.getSSE())
//                .arg(ff.getSSR())
//                .arg(ff.getRMSE());
//        //决定系数
//        strDes += tr("\nR-square(coefficient of determination)=%1").arg(ff.getRSquare());
//        //大于0.001为可接受
//        strDes += tr("\ngammq=%1 (Greater than 0.001 for acceptable)").arg(ff.getGoodness());
//        getMainWindow()->showNormalInfo(strDes);
//        //绘制曲线
//        std::vector<double> fitYs;
//        fitYs.resize(xs.size());
//        ff.getYis(xs.begin(),xs.end(),fitYs.begin());
//        QVector<QPointF> points;

//        SAChart2D::makeVectorPointF(xs.begin(),xs.end()
//                                    ,fitYs.begin(),fitYs.end()
//                                    ,points);
//        QwtPlotCurve* cur = fig->addVirtualCurve(
//                                tr("Polyfit('%1',%2)").arg(curves[i]->title().text()).arg(n)
//                                ,points);
//        QPen pen(cur->pen());
//        pen.setWidth(2);
//        pen.setColor(SARandColorMaker::getCurveColor());
//        pen.setStyle(Qt::DashLine);
//        cur->setPen(pen);
//    }
//}


///
/// \brief 创建一个绘图窗口
/// \param title 窗口名字
/// \return
///
SAMdiSubWindow*SADrawDelegate::createFigureMdiSubWidget(const QString &title)
{
    m_nUserChartCount++;
    MainWindow* m = getMainWindow ();
    QString str = title;
    if(title.isNull () || title.isEmpty ())
    {
        str = tr("figure[%1]").arg(m_nUserChartCount);
    }
    SAMdiSubWindow* pSubWnd =  m->createMdiSubWindow<SAFigureWindow>(SA::Normal2DChartWnd,str);
    if(!pSubWnd)
    {
        --m_nUserChartCount;
        return nullptr;
    }
    SAFigureWindow* pChartWnd = qobject_cast<SAFigureWindow*>(pSubWnd->widget());
    if(!pChartWnd)
    {
        --m_nUserChartCount;
        return nullptr;
    }
    m->connect (pChartWnd,&SAFigureWindow::chartDataChanged,m,&MainWindow::onChartDataChanged);
    return pSubWnd;
}


QwtPlotCurve *SADrawDelegate::drawVPoint(SAFigureWindow *fig , SAVectorPointF* points)
{
    SAChart2D* chart = fig->current2DPlot();
    if(nullptr == chart)
    {
        chart = fig->create2DPlot();
    }
    return (QwtPlotCurve *)(chart->addCurve(points));
}
///
/// \brief 从subwindow指针中查找是否含有SAFigureWindow
/// \param sub subwindow指针
/// \return 存在返回指针，否则返回nullptr
///
SAFigureWindow*SADrawDelegate::getFigureWidgetFromMdiSubWindow(QMdiSubWindow *w)
{
    return MainWindow::getFigureWidgetFromMdiSubWindow(w);
}

