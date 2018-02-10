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
#include "SABarSeries.h"
#include "SAXYSeries.h"
#include "SASeries.h"
#include "SAData.h"
#include "SAScatterSeries.h"
SADrawDelegate::SADrawDelegate(MainWindow* wnd):SAMainWindowDelegate(wnd)
  ,m_nUserChartCount(0)
{
}

SADrawDelegate::~SADrawDelegate()
{

}


///
/// \brief 绘制线图
/// \param datas
/// \return
///
QList<QwtPlotCurve*> SADrawDelegate::drawLine(const QList<SAAbstractDatas *> &datas)
{
    QList<QwtPlotCurve*> res;
    if(datas.size()<=0)
    {
        return res;
    }

    QMdiSubWindow* pSubWnd = createFigureMdiSubWidget(datas.size()==1 ? datas[0]->getName () : "");
    SAFigureWindow* pFigure = qobject_cast<SAFigureWindow*>(pSubWnd->widget());
    if(!pSubWnd && !pFigure)
    {
        return res;//任何一个指针为0都退出
    }
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        chart = pFigure->create2DPlot();
    }
    chart->setAutoReplot(false);
    res = drawLine(datas,chart);

    chart->enablePicker(true);
    chart->enableZoomer(false);
    chart->enableGrid(true);
    chart->setAutoReplot(true);
    pSubWnd->show();
    return res;
}

QList<QwtPlotCurve *> SADrawDelegate::drawLine(const QList<SAAbstractDatas *> &datas, SAChart2D *chart)
{
    QList<QwtPlotCurve*> res;
    if(datas.size()<=0)
    {
        return res;
    }
    chart->setAutoReplot(false);
    std::for_each(datas.begin(),datas.end(),[chart,&res](SAAbstractDatas* data){
        QwtPlotCurve* p = nullptr;
        if(SA::VectorPoint == data->getType())
        {
            p = (QwtPlotCurve*)chart->addCurve(data);
        }
        else if(SA::VectorDouble == data->getType())
        {
            p = (QwtPlotCurve*)chart->addCurve(data,1,1);
        }
        else
        {
            if(SA::Dim1 == data->getDim())
            {
                QVector<double> v;
                SAAbstractDatas::converToDoubleVector(data,v);
                if(v.size() > 0)
                {
                    QVector<double> x(v.size());
                    double start = 1;
                    std::for_each(x.begin(),x.end(),[&start](double& d){
                        d = start;
                        start += 1.0;
                    });
                    SAXYSeries* serise = new SAXYSeries(data->getName());
                    serise->setSamples(x,v);
                    serise->insertData(data);
                    chart->addItem(serise,tr("add Line %1").arg(data->getName()));
                    p = static_cast<QwtPlotCurve*>(serise);
                }
            }
            else if(SA::Dim2 == data->getDim())
            {
                QVector<QPointF> v;
                SAAbstractDatas::converToPointFVector(data,v);
                if(v.size() > 0)
                {
                    SAXYSeries* serise = new SAXYSeries(data->getName());
                    serise->setSamples(v);
                    serise->insertData(data);
                    chart->addItem(serise,tr("add Line %1").arg(data->getName()));
                    p = static_cast<QwtPlotCurve*>(serise);
                }
            }
        }
        res.append(p);
    });
    chart->replot();
    return res;

}
///
/// \brief 在已有图上添加曲线
/// \param data
///
QwtPlotCurve* SADrawDelegate::drawLine(SAAbstractDatas* data,SAChart2D* chart)
{
    if(nullptr == data || nullptr == chart)
    {
        return nullptr;
    }
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addCurve(data));
    return cur;
}
///
/// \brief 在已有图上添加曲线
/// \param x
/// \param y
/// \param name
/// \param chart
/// \return
///
QwtPlotCurve *SADrawDelegate::drawLine(SAAbstractDatas* x, SAAbstractDatas* y, QString name,SAChart2D *chart)
{
    if(nullptr == x || nullptr == y || nullptr == chart)
    {
        return nullptr;
    }
    if(name.isNull () || name.isEmpty ())
    {
        name = QStringLiteral("%1-%2").arg(x->getName ()).arg(y->getName ());
    }
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addCurve (x,y,name));
    return cur;
}

QwtPlotHistogram *SADrawDelegate::drawHistogram(SAAbstractDatas *data)
{
    if(data->getType () != SA::VectorInterval)
    {
        getMainWindow()->showWarningMessageInfo(tr("invalid data type,bar chart accept VectorInterval type"));
        return nullptr;
    }
    SAVectorInterval* intervalSeries = static_cast<SAVectorInterval*>(data);
    QMdiSubWindow* w = createFigureMdiSubWidget(data->getName ());
    SAFigureWindow* pFigure = getFigureWidgetFromMdiSubWindow(w);
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        chart = pFigure->create2DPlot();
    }
    QwtPlotHistogram* b = (QwtPlotHistogram*)(chart->addHistogram (intervalSeries));
    w->show ();
    return b;
}
///
/// \brief 绘制柱状图
/// \param barSeries 柱状图数据
/// \return
///
QList<QwtPlotHistogram *> SADrawDelegate::drawHistogram(const QList<SAAbstractDatas *> &datas)
{
    QList<QwtPlotHistogram *> res;
    QList<SAAbstractDatas *> invalidDatas;
    for(int i=0;i<datas.size();++i)
    {
        if(datas[i]->getType () == SA::VectorInterval)
        {
            invalidDatas.append(datas[i]);
        }
    }
    if(0 == invalidDatas.size())
    {
        getMainWindow()->showWarningMessageInfo(tr("invalid data type,bar chart accept VectorInterval type"));
        return res;
    }
    QMdiSubWindow* w = createFigureMdiSubWidget(datas.size()==1 ? datas[0]->getName () : "");
    SAFigureWindow* pFigure = getFigureWidgetFromMdiSubWindow(w);
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        chart = pFigure->create2DPlot();
    }
    chart->setAutoReplot(false);
    for(int i=0;i<invalidDatas.size();++i)
    {
        QwtPlotHistogram* p = (QwtPlotHistogram *)chart->addHistogram(invalidDatas[i]);
        res.append(p);
    }
    chart->enablePicker(true);
    chart->enableZoomer(false);
    chart->enableGrid(true);
    chart->setAutoReplot(true);
    w->show();
    return res;
}
///
/// \brief bar图
/// \param datas
/// \return
///
QList<QwtPlotBarChart *> SADrawDelegate::drawBar(const QList<SAAbstractDatas *> &datas)
{
    QList<QwtPlotBarChart *> res;
    if(datas.size()<=0)
    {
        return res;
    }
    QMdiSubWindow* pSubWnd = createFigureMdiSubWidget(datas.size()==1 ? datas[0]->getName () : "");
    SAFigureWindow* pFigure = qobject_cast<SAFigureWindow*>(pSubWnd->widget());
    if(!pSubWnd && !pFigure)
    {
        return res;//任何一个指针为0都退出
    }
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        chart = pFigure->create2DPlot();
    }


    std::for_each(datas.begin(),datas.end(),[chart,&res](SAAbstractDatas* data){
        QwtPlotBarChart* p = nullptr;
        if(SA::VectorPoint == data->getType())
        {
            p = (QwtPlotBarChart*)chart->addBar(data);
        }
        else if(SA::VectorDouble == data->getType())
        {
            p = (QwtPlotBarChart*)chart->addBar(data);
        }
        else
        {
            if(SA::Dim1 == data->getDim())
            {
                QVector<double> v;
                SAAbstractDatas::converToDoubleVector(data,v);
                if(v.size() > 0)
                {
                    SABarSeries* barSerise = new SABarSeries(data->getName());
                    barSerise->setSamples(v);
                    barSerise->insertData(data);
                    chart->addItem(barSerise,tr("add Bar %1").arg(data->getName()));
                    p = static_cast<QwtPlotBarChart*>(barSerise);
                }
            }
            else if(SA::Dim2 == data->getDim())
            {
                QVector<QPointF> v;
                SAAbstractDatas::converToPointFVector(data,v);
                if(v.size() > 0)
                {
                    SABarSeries* barSerise = new SABarSeries(data->getName());
                    barSerise->setSamples(v);
                    barSerise->insertData(data);
                    chart->addItem(barSerise,tr("add Bar %1").arg(data->getName()));
                    p = static_cast<QwtPlotBarChart*>(barSerise);
                }
            }
        }
        res.append(p);
    });
    chart->enablePicker(true);
    chart->enableZoomer(false);
    chart->enableGrid(true);
    pSubWnd->show();
    return res;
}
///
/// \brief 散点图
/// \param datas
/// \return
///
QList<QwtPlotCurve *> SADrawDelegate::drawScatter(const QList<SAAbstractDatas *> &datas)
{
    QList<QwtPlotCurve*> res;
    if(datas.size()<=0)
    {
        return res;
    }

    QMdiSubWindow* pSubWnd = createFigureMdiSubWidget(datas.size()==1 ? datas[0]->getName () : "");
    SAFigureWindow* pFigure = qobject_cast<SAFigureWindow*>(pSubWnd->widget());
    if(!pSubWnd && !pFigure)
    {
        return res;//任何一个指针为0都退出
    }
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        chart = pFigure->create2DPlot();
    }
    chart->setAutoReplot(false);
    std::for_each(datas.begin(),datas.end(),[chart,&res](SAAbstractDatas* data){
        QwtPlotCurve* p = nullptr;
        if(SA::VectorPoint == data->getType())
        {
            p = (QwtPlotCurve*)chart->addScatter(data);
        }
        else if(SA::VectorDouble == data->getType() || SA::Dim1 == data->getDim())
        {
            QVector<double> v;
            SAAbstractDatas::converToDoubleVector(data,v);
            if(v.size() > 0)
            {
                QVector<double> x(v.size());
                double start = 1;
                std::for_each(x.begin(),x.end(),[&start](double& d){
                    d = start;
                    start += 1.0;
                });
                SAScatterSeries* serise = new SAScatterSeries(data->getName());
                serise->setSamples(x,v);
                serise->insertData(data);
                chart->addItem(serise,tr("add Scatter %1").arg(data->getName()));
                p = static_cast<QwtPlotCurve*>(serise);
            }
        }
        else if(SA::Dim2 == data->getDim())
        {
            QVector<QPointF> v;
            SAAbstractDatas::converToPointFVector(data,v);
            if(v.size() > 0)
            {
                SAScatterSeries* serise = new SAScatterSeries(data->getName());
                serise->setSamples(v);
                serise->insertData(data);
                chart->addItem(serise,tr("add Scatter %1").arg(data->getName()));
                p = static_cast<QwtPlotCurve*>(serise);
            }
        }
        res.append(p);
    });

    chart->enablePicker(true);
    chart->enableZoomer(false);
    chart->enableGrid(true);
    chart->setAutoReplot(true);
    pSubWnd->show();
    return res;
}
///
/// \brief 绘制箱盒图
/// \param datas
/// \return
///
QList<QwtPlotTradingCurve *> SADrawDelegate::drawBoxChart(const QList<SAAbstractDatas *> &datas)
{
    QList<QwtPlotTradingCurve*> res;
    if(datas.size()<=0)
    {
        return res;
    }

    QMdiSubWindow* pSubWnd = createFigureMdiSubWidget(datas.size()==1 ? datas[0]->getName () : "");
    SAFigureWindow* pFigure = qobject_cast<SAFigureWindow*>(pSubWnd->widget());
    if(!pSubWnd && !pFigure)
    {
        return res;//任何一个指针为0都退出
    }
    SAChart2D* chart = pFigure->current2DPlot();
    if(!chart)
    {
        chart = pFigure->create2DPlot();
    }
    chart->setAutoReplot(false);
    std::for_each(datas.begin(),datas.end(),[chart,&res](SAAbstractDatas* data){
        QwtPlotTradingCurve* p = nullptr;
        if(SA::VectorOHLC == data->getType())
        {
            p = (QwtPlotTradingCurve*)chart->addBox(data);
        }
        res.append(p);
    });

    chart->enablePicker(true);
    chart->enableZoomer(false);
    chart->enableGrid(true);
    chart->setAutoReplot(true);
    pSubWnd->show();
    return res;
}


///
/// \brief 创建一个绘图窗口
/// \param title 窗口名字
/// \return
///
QMdiSubWindow*SADrawDelegate::createFigureMdiSubWidget(const QString &title)
{ 
    MainWindow* m = getMainWindow ();
    return m->createFigureWindow(title);
}

QMdiSubWindow *SADrawDelegate::createFigureMdiSubWidget(SAFigureWindow *fig, const QString &title)
{
    MainWindow* m = getMainWindow ();
    return m->createFigureWindow(fig,title);
}


QwtPlotCurve *SADrawDelegate::drawLine(SAChart2D *chart , SAVectorPointF* points)
{
    if(nullptr == chart)
    {
        return nullptr;
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

