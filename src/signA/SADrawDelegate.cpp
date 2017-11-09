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
    drawTrend(datas);
}

void SADrawDelegate::drawTrend(const QList<SAAbstractDatas *> &datas)
{
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
        if(SA::VectorPoint == data->getType())
        {
            chart->addCurve(data);
        }
        else
        {
            chart->addCurve(data,1,1);
        }
    });

    chart->setAutoReplot(false);
    chart->enablePicker(true);
    chart->enableZoomer(false);
    chart->enableGrid(true);
    chart->setAutoReplot(true);
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

