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
#include "SAAddLineChartSetDialog.h"
#include "SAAddCurveTypeDialog.h"
#include <QInputDialog>
SADrawDelegate::SADrawDelegate(MainWindow* wnd):SAMainWindowDelegate(wnd)
{
}

SADrawDelegate::~SADrawDelegate()
{

}
///
/// \brief 新建一个figure
/// \param title figure的标题，可以为空
/// \return 返回一个tuple<SAChart2D *, SAFigureWindow *, QMdiSubWindow *>
/// 可以通过如下方式获取返回值：
/// \code
/// QMdiSubWindow* pSubWnd = nullptr;
/// SAChart2D* chart = nullptr;
/// std::tie(chart,std::ignore,pSubWnd) = createFigure();
/// \endcode
/// \see createFigureMdiSubWidget
///
std::tuple<SAChart2D *, SAFigureWindow *, QMdiSubWindow *> SADrawDelegate::createFigure(const QString &title)
{
    QMdiSubWindow* pSubWnd = createFigureMdiSubWidget(title);
    SAFigureWindow* pFigure = qobject_cast<SAFigureWindow*>(pSubWnd->widget());
    if(!pSubWnd && !pFigure)
    {
        return std::make_tuple<SAChart2D *, SAFigureWindow *, QMdiSubWindow *>(nullptr,nullptr,nullptr);//任何一个指针为0都退出
    }
    SAChart2D* chart = pFigure->create2DPlot();
    return std::make_tuple(chart,pFigure,pSubWnd);
}


///
/// \brief 创建一个figure并绘制线图
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
    QMdiSubWindow* pSubWnd = nullptr;
    SAChart2D* chart = nullptr;
    std::tie(chart,std::ignore,pSubWnd) = createFigure();

    if(nullptr == chart)
    {
        return res;
    }
    chart->setAutoReplot(false);
    res = drawLine(datas,chart);

    chart->enablePicker(true);
    chart->enableZoomer(false);
    chart->enableGrid(true);
    chart->setAutoReplot(true);
    chart->replot();
    pSubWnd->show();
    return res;
}

QList<QwtPlotCurve *> SADrawDelegate::drawLine(const QList<SAAbstractDatas *> &datas, SAChart2D *chart)
{
    if(nullptr == chart)
    {
        QMdiSubWindow* pSubWnd = nullptr;
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
        pSubWnd->show();
    }
    QList<QwtPlotCurve*> res;
    if(datas.size()<=0)
    {
        return res;
    }
    if(nullptr == chart)
    {
        getMainWindow()->showWarningMessageInfo(tr("can not create chart"));
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
    chart->setAutoReplot(true);
    chart->replot();
    return res;

}
///
/// \brief 在已有图上添加曲线
/// \param data
///
QwtPlotCurve* SADrawDelegate::drawLine(SAAbstractDatas* data, double xStart, double xDetal, SAChart2D* chart, const QString &name)
{
    if(nullptr == chart)
    {
        QMdiSubWindow* pSubWnd = nullptr;
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
        pSubWnd->show();
    }
    if(nullptr == data || nullptr == chart)
    {
        return nullptr;
    }
    QString title = (name.isEmpty() ? data->getName() : name);
    chart->setAutoReplot(false);
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addCurve(data,xStart,xDetal,title));
    chart->setAutoReplot(true);
    chart->replot();
    return cur;
}
///
/// \brief 在已有图上添加曲线
/// \param x x值
/// \param y y值
/// \param name 曲线名，为空会以y值作为曲线名
/// \param chart 为nullptr时会新创建一个figure并绘图
/// \return
///
QwtPlotCurve *SADrawDelegate::drawLine(SAAbstractDatas* x, SAAbstractDatas* y, SAChart2D *chart, const QString& name)
{
    if(nullptr == chart)
    {
        QMdiSubWindow* pSubWnd = nullptr;
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
        pSubWnd->show();
    }
    if(nullptr == x || nullptr == y || nullptr == chart)
    {
        return nullptr;
    }
    QString title = name;
    if(title.isEmpty ())
    {
        title = y->getName ();
    }
    chart->setAutoReplot(false);
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addCurve (x,y,title));
    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();
    return cur;
}

QwtPlotCurve *SADrawDelegate::drawLine(const QVector<double> &xData, const QVector<double> &yData, SAChart2D *chart, const QString &name)
{
    if(nullptr == chart)
    {
        QMdiSubWindow* pSubWnd = nullptr;
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
        pSubWnd->show();
    }
    if(0 == xData.size() || 0 == yData.size())
    {
        return nullptr;
    }
    chart->setAutoReplot(false);
    QwtPlotCurve* cur = chart->addCurve (xData,yData);
    if(cur)
    {
        cur->setTitle(name);
    }
    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();
    return cur;
}
///
/// \brief SADrawDelegate::drawLineWithWizard
/// \return
///
QList<QwtPlotCurve *> SADrawDelegate::drawLineWithWizard()
{
    QList<SAAbstractDatas *> datas = getMainWindow()->getSeletedDatas();
    SAChart2D* chart = nullptr;
    if( (datas.size() > 0) && (2 != datas.size()))
    {
        //如果选中了1个数据或者多个数据，就绘制0-1的趋势图
        SAAddCurveTypeDialog::AddCurveType type = SAAddCurveTypeDialog::getAddCurveType(getMainWindow());
        if(SAAddCurveTypeDialog::Unknow == type)
        {
            return QList<QwtPlotCurve *>();
        }
        if(SAAddCurveTypeDialog::AddInCurrentFig == type)
        {
           chart = getCurSubWindowChart();
        }
        else if(SAAddCurveTypeDialog::AddInNewFig == type)
        {
            chart = nullptr;//空指针时会自动创建figure
        }
        else if(SAAddCurveTypeDialog::AddInCurrentFigWithSubplot == type)
        {
            //TODO

        }
        return drawLine(datas,chart);
    }
    else
    {
        //如果选中了两个数据或者没有选中数据，使用对话框引导
        SAAddLineChartSetDialog dlg(getMainWindow());
        if(2 == datas.size())
        {
            dlg.setXSelectName(datas[0]->getName());
            dlg.setYSelectName(datas[1]->getName());
            dlg.setChartTitle(datas[1]->getName());
        }
        if(QDialog::Accepted != dlg.exec())
        {
            return QList<QwtPlotCurve *>();
        }
        SAAddLineChartSetDialog::AsixSet asX = dlg.getXAxisSet();
        SAAddLineChartSetDialog::AsixSet asY = dlg.getYAxisSet();
        QString title = dlg.getChartTitle();
        bool isAddInCur = dlg.isAddInCurrentChart();

        if(isAddInCur)
        {
            chart = getCurSubWindowChart();
        }
        else
        {
            chart = nullptr;
        }

        if(SAAddLineChartSetDialog::NormalSet == asX && SAAddLineChartSetDialog::NormalSet == asY)
        {
            SAAbstractDatas* x = dlg.getXDatas();
            SAAbstractDatas* y = dlg.getYDatas();


            if(nullptr == x || nullptr == y)
            {
                getMainWindow()->showWarningMessageInfo(tr("select invalid x datas or y datas"));
                return QList<QwtPlotCurve *>();
            }
            QwtPlotCurve *p = drawLine(x,y,chart,title);
            return {p};

        }
        else if(SAAddLineChartSetDialog::NormalSet == asX && SAAddLineChartSetDialog::UserDefineSet == asY)
        {
            double start,detal;
            dlg.getYUserDefineValues(start,detal);
            SAAbstractDatas* x = dlg.getXDatas();
            QVector<double> xarr,yarr;
            if(SAAbstractDatas::converToDoubleVector(x,xarr))
            {
                yarr.resize(xarr.size());
                for(int i=0;i<yarr.size();++i)
                {
                    yarr[i] = start + i*detal;
                }
                QwtPlotCurve *p = drawLine(xarr,yarr,chart,title);
                return {p};
            }
        }
        else if(SAAddLineChartSetDialog::UserDefineSet == asX && SAAddLineChartSetDialog::NormalSet == asY)
        {
            SAAbstractDatas* y = dlg.getXDatas();

            if(nullptr == y)
            {
                getMainWindow()->showWarningMessageInfo(tr("select invalid x datas or y datas"));
                return QList<QwtPlotCurve *>();
            }
            double start,detal;
            dlg.getXUserDefineValues(start,detal);
            QwtPlotCurve *p = drawLine(y,start,detal,chart,title);
            return {p};
        }
        else if(SAAddLineChartSetDialog::UserDefineSet == asX && SAAddLineChartSetDialog::UserDefineSet == asY)
        {
            bool isOK = false;
            int dataSize = QInputDialog::getInt(getMainWindow()
                                     ,tr("input data size")
                                     ,tr("input define data size:")
                                     ,1000
                                     ,1
                                     ,99999999
                                     ,1
                                     ,&isOK);
            if(!isOK)
            {
                return QList<QwtPlotCurve *>();
            }
            double xstart,xdetal;
            double ystart,ydetal;
            dlg.getXUserDefineValues(xstart,xdetal);
            dlg.getYUserDefineValues(ystart,ydetal);
            QVector<double> xarr,yarr;
            xarr.resize(dataSize);
            yarr.resize(dataSize);
            for(int i=0;i<dataSize;++i)
            {
                xarr[i] = xstart + i*xdetal;
                yarr[i] = ystart + i*ydetal;
            }
            QwtPlotCurve *p = drawLine(xarr,yarr,chart,title);
            return {p};

        }
    }
    return QList<QwtPlotCurve *>();
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

SAChart2D *SADrawDelegate::getCurSubWindowChart() const
{
    return getMainWindow()->getCurSubWindowChart();
}
///
/// \brief 获取当前的figure 窗口
/// \return
///
SAFigureWindow *SADrawDelegate::getCurrentFigureWindow() const
{
    return getMainWindow()->getCurrentFigureWindow();
}

