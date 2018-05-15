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
#include "SAData.h"
#include "SAScatterSeries.h"
#include "SAAddLineChartSetDialog.h"
#include "SAAddCurveTypeDialog.h"
#include "SAIntervalSeries.h"
#include <QInputDialog>
#include "SAAddIntervalCurveDialog.h"
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


QList<QwtPlotCurve *> SADrawDelegate::drawLine(const QList<SAAbstractDatas *> &datas, SAChart2D *chart)
{
    QList<QwtPlotCurve*> res;
    if(datas.size()<=0)
    {
        return res;
    }
    //如果没指定figure，就创建figure
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
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
    chart->updateAxes();
    chart->replot();
    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return res;

}
///
/// \brief 在已有图上添加曲线
/// \param data
///
QwtPlotCurve* SADrawDelegate::drawLine(SAAbstractDatas* data, double xStart, double xDetal, SAChart2D* chart, const QString &name)
{
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
    }
    if(nullptr == data || nullptr == chart)
    {
        return nullptr;
    }
    QString title = (name.isEmpty() ? data->getName() : name);
    chart->setAutoReplot(false);
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addCurve(data,xStart,xDetal,title));
    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();
    if(pSubWnd)
    {
        pSubWnd->show();
    }
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
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
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
    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return cur;
}

QwtPlotCurve *SADrawDelegate::drawLine(const QVector<double> &xData, const QVector<double> &yData, SAChart2D *chart, const QString &name)
{
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
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
    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return cur;
}
///
/// \brief 通过对话框引导添加图线
/// \return
///
QList<QwtPlotCurve *> SADrawDelegate::drawLineWithWizard()
{
    QList<SAAbstractDatas *> datas = getMainWindow()->getSeletedDatas();
    SAChart2D* chart = nullptr;
    //如果datas是点序列直接绘制
    if(1 == datas.size())
    {
        if(SA::VectorPoint == datas[0]->getType())
        {
           QwtPlotCurve *p = drawLineWithWizard(datas[0]);
           if(p)
               return {p};
           return QList<QwtPlotCurve *>();
        }
    }


    //如果选中了两个数据或者没有选中数据，使用对话框引导
    SAAddLineChartSetDialog dlg(getMainWindow());
    if(1 == datas.size())
    {
        dlg.setXAsixSet(SAAddLineChartSetDialog::UserDefineSet);
        dlg.setXUserDefineValues(0,1);
        dlg.setYAsixSet(SAAddLineChartSetDialog::NormalSet);
        dlg.setYSelectName(datas[0]->getName());
        dlg.setChartTitle(datas[0]->getName());
    }
    else if(2 == datas.size())
    {
        dlg.setXAsixSet(SAAddLineChartSetDialog::NormalSet);
        dlg.setYAsixSet(SAAddLineChartSetDialog::NormalSet);
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
        SAAbstractDatas* y = dlg.getYDatas();

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

    return QList<QwtPlotCurve *>();
}
///
/// \brief SADrawDelegate::drawLineWithWizard
/// \param pointVector
/// \return
///
QwtPlotCurve *SADrawDelegate::drawLineWithWizard(SAAbstractDatas *pointVector)
{
    if(SA::VectorPoint == pointVector->getType())
    {
        //如果当前有窗口，弹出询问添加方式
        if(getMainWindow()->isHaveFigureWindow())
        {
            SAAddCurveTypeDialog::AddCurveType type = SAAddCurveTypeDialog::getAddCurveType(getMainWindow());
            QList<QwtPlotCurve *> res;
            if(SAAddCurveTypeDialog::AddInNewFig == type)
            {
                res = drawLine({pointVector},nullptr);

            }
            else if(SAAddCurveTypeDialog::AddInCurrentFig == type)
            {
                SAChart2D* chart = getCurSubWindowChart();
                res = drawLine({pointVector},chart);
            }
            return ((res.size() > 0) ? res[0] : nullptr);
        }
        else
        {
            QList<QwtPlotCurve *> res = drawLine({pointVector},nullptr);
            return ((res.size() > 0) ? res[0] : nullptr);
        }
    }
    return nullptr;
}
///
/// \brief 散点图
/// \param datas
/// \return
///
QList<QwtPlotCurve *> SADrawDelegate::drawScatter(const QList<SAAbstractDatas *> &datas, SAChart2D *chart)
{
    QList<QwtPlotCurve*> res;
    if(datas.size()<=0)
    {
        return res;
    }
    //如果没指定figure，就创建figure
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
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
            p = (QwtPlotCurve*)chart->addScatter(data);
        }
        else if(SA::VectorDouble == data->getType())
        {
            p = (QwtPlotCurve*)chart->addScatter(data,1,1);
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
        }

        res.append(p);
    });

    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();

    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return res;
}

QwtPlotCurve *SADrawDelegate::drawScatter(SAAbstractDatas *x, SAAbstractDatas *y, SAChart2D *chart, const QString &name)
{
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
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
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addScatter (x,y,title));
    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();
    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return cur;
}

QwtPlotCurve *SADrawDelegate::drawScatter(const QVector<double> &xData, const QVector<double> &yData, SAChart2D *chart, const QString &name)
{
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
    }
    if(0 == xData.size() || 0 == yData.size())
    {
        return nullptr;
    }
    chart->setAutoReplot(false);
    QwtPlotCurve* cur = chart->addScatter (xData,yData);
    if(cur)
    {
        cur->setTitle(name);
    }
    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();
    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return cur;
}

QwtPlotCurve *SADrawDelegate::drawScatter(SAAbstractDatas *data, double xStart, double xDetal, SAChart2D *chart, const QString &name)
{
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
    }
    if(nullptr == data || nullptr == chart)
    {
        return nullptr;
    }
    QString title = (name.isEmpty() ? data->getName() : name);
    chart->setAutoReplot(false);
    QwtPlotCurve* cur = (QwtPlotCurve*)(chart->addScatter(data,xStart,xDetal,title));
    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();
    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return cur;
}

QList<QwtPlotCurve *> SADrawDelegate::drawScatterWithWizard()
{
    QList<SAAbstractDatas *> datas = getMainWindow()->getSeletedDatas();
    SAChart2D* chart = nullptr;
    //如果datas是点序列直接绘制
    if(1 == datas.size())
    {
        if(SA::VectorPoint == datas[0]->getType())
        {
           QwtPlotCurve *p = drawScatterWithWizard(datas[0]);
           if(p)
               return {p};
           return QList<QwtPlotCurve *>();
        }
    }


    //如果选中了两个数据或者没有选中数据，使用对话框引导
    SAAddLineChartSetDialog dlg(getMainWindow());
    if(1 == datas.size())
    {
        dlg.setXAsixSet(SAAddLineChartSetDialog::UserDefineSet);
        dlg.setXUserDefineValues(0,1);
        dlg.setYAsixSet(SAAddLineChartSetDialog::NormalSet);
        dlg.setYSelectName(datas[0]->getName());
        dlg.setChartTitle(datas[0]->getName());
    }
    else if(2 == datas.size())
    {
        dlg.setXAsixSet(SAAddLineChartSetDialog::NormalSet);
        dlg.setYAsixSet(SAAddLineChartSetDialog::NormalSet);
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
        QwtPlotCurve *p = drawScatter(x,y,chart,title);
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
            QwtPlotCurve *p = drawScatter(xarr,yarr,chart,title);
            return {p};
        }
    }
    else if(SAAddLineChartSetDialog::UserDefineSet == asX && SAAddLineChartSetDialog::NormalSet == asY)
    {
        SAAbstractDatas* y = dlg.getYDatas();

        if(nullptr == y)
        {
            getMainWindow()->showWarningMessageInfo(tr("select invalid x datas or y datas"));
            return QList<QwtPlotCurve *>();
        }
        double start,detal;
        dlg.getXUserDefineValues(start,detal);
        QwtPlotCurve *p = drawScatter(y,start,detal,chart,title);
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
        QwtPlotCurve *p = drawScatter(xarr,yarr,chart,title);
        return {p};

    }

    return QList<QwtPlotCurve *>();

}

QwtPlotCurve *SADrawDelegate::drawScatterWithWizard(SAAbstractDatas *pointVector)
{
    if(SA::VectorPoint == pointVector->getType())
    {
        //如果当前有窗口，弹出询问添加方式
        if(getMainWindow()->isHaveFigureWindow())
        {
            SAAddCurveTypeDialog::AddCurveType type = SAAddCurveTypeDialog::getAddCurveType(getMainWindow());
            QList<QwtPlotCurve *> res;
            if(SAAddCurveTypeDialog::AddInNewFig == type)
            {
                res = drawScatter({pointVector},nullptr);

            }
            else if(SAAddCurveTypeDialog::AddInCurrentFig == type)
            {
                SAChart2D* chart = getCurSubWindowChart();
                res = drawLine({pointVector},chart);
            }
            return ((res.size() > 0) ? res[0] : nullptr);
        }
        else
        {
            QList<QwtPlotCurve *> res = drawLine({pointVector},nullptr);
            return ((res.size() > 0) ? res[0] : nullptr);
        }
    }
    return nullptr;
}
///
/// \brief SADrawDelegate::drawIntervalCurve
/// \param v
/// \param min
/// \param max
/// \param chart
/// \return
///
QwtPlotIntervalCurve *SADrawDelegate::drawIntervalCurve(SAAbstractDatas *v, SAAbstractDatas *min, SAAbstractDatas *max, SAChart2D *chart)
{
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
    }
    if(nullptr == chart)
    {
        getMainWindow()->showWarningMessageInfo(tr("can not create chart"));
        return nullptr;
    }
    chart->setAutoReplot(false);

    QwtPlotIntervalCurve* p = (QwtPlotIntervalCurve*)chart->addIntervalCurve(v,min,max);


    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();

    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return p;
}
///
/// \brief 绘制IntervalCurve
/// \param datas
/// \param chart
/// \return
///
QList<QwtPlotIntervalCurve *> SADrawDelegate::drawIntervalCurve(const QList<SAAbstractDatas *> &datas, SAChart2D *chart)
{
    QList<QwtPlotIntervalCurve*> res;
    if(datas.size()<=0)
    {
        return res;
    }
    //如果没指定figure，就创建figure
    QMdiSubWindow* pSubWnd = nullptr;
    if(nullptr == chart)
    {
        std::tie(chart,std::ignore,pSubWnd) = createFigure();
    }
    if(nullptr == chart)
    {
        getMainWindow()->showWarningMessageInfo(tr("can not create chart"));
        return res;
    }

    chart->setAutoReplot(false);
    std::for_each(datas.begin(),datas.end(),[chart,&res](SAAbstractDatas* data){
        QwtPlotIntervalCurve* p = nullptr;
        if(SA::VectorInterval == data->getType())
        {
            p = (QwtPlotIntervalCurve*)chart->addIntervalCurve(data);
        }
        res.append(p);
    });

    chart->setAutoReplot(true);
    chart->updateAxes();
    chart->replot();

    if(pSubWnd)
    {
        pSubWnd->show();
    }
    return res;
}
///
/// \brief 区域图IntervalCurve
/// \return
///
QList<QwtPlotIntervalCurve *> SADrawDelegate::drawIntervalCurveWithWizard()
{
    QList<QwtPlotIntervalCurve*> res;
    QList<SAAbstractDatas *> datas = getMainWindow()->getSeletedDatas();
    SAChart2D* chart = nullptr;
    if(1 == datas.size())
    {
        if(SA::VectorInterval == datas[0]->getType())
        {
           QwtPlotIntervalCurve *p = drawIntervalCurveWithWizard(datas[0]);
           if(p)
           {
               res << p;
           }
           else
           {
               getMainWindow()->showWarningMessageInfo(tr("<div>can not draw IntervalCurve with</div>"
                                                          "<div>data:[%1]</div>"
                                                          )
                                                       .arg(datas[0]->getName())
                                                       );
           }
           return res;
        }
    }
    SAAddIntervalCurveDialog dlg(getMainWindow());
    switch(datas.size())
    {
    case 1:
    {
        dlg.setValueDatas(datas[0]);
        break;
    }
    case 2:
    {
        dlg.setValueDatas(datas[0]);
        dlg.setMinDatas(datas[1]);
        break;
    }
    case 3:
    {
        dlg.setValueDatas(datas[0]);
        dlg.setMinDatas(datas[1]);
        dlg.setMaxDatas(datas[2]);
        break;
    }
    default:
        break;
    }
    if(QDialog::Accepted != dlg.exec())
    {
        return res;
    }
    SAAbstractDatas* v = dlg.getValueDatas();
    SAAbstractDatas* min = dlg.getMinDatas();
    SAAbstractDatas* max = dlg.getMaxDatas();
    if(nullptr == v || nullptr == min || nullptr == max)
    {
        return res;
    }
    QString title = dlg.getChartTitle();
    if(dlg.isAddInCurrentChart())
    {
        chart = getCurSubWindowChart();
    }
    else
    {
        chart = nullptr;
    }
    QwtPlotIntervalCurve* p = drawIntervalCurve(v,min,max,chart);
    if(p)
    {
        p->setTitle(title);
        res << p;
    }
    else
    {
        getMainWindow()->showWarningMessageInfo(tr("<div>can not draw IntervalCurve with</div>"
                                                   "<div>value:[%1]</div>"
                                                   "<div>minValue:[%2]</div>"
                                                   "<div>maxValue:[%3]</div>")
                                                .arg(v->getName())
                                                .arg(min->getName())
                                                .arg(max->getName())
                                                );
    }
    return res;
}

QwtPlotIntervalCurve *SADrawDelegate::drawIntervalCurveWithWizard(SAAbstractDatas *datas)
{
    if(SA::VectorInterval == datas->getType())
    {
        //如果当前有窗口，弹出询问添加方式
        if(getMainWindow()->isHaveFigureWindow())
        {
            SAAddCurveTypeDialog::AddCurveType type = SAAddCurveTypeDialog::getAddCurveType(getMainWindow());
            QList<QwtPlotIntervalCurve *> res;
            if(SAAddCurveTypeDialog::AddInNewFig == type)
            {
                res = drawIntervalCurve({datas},nullptr);

            }
            else if(SAAddCurveTypeDialog::AddInCurrentFig == type)
            {
                SAChart2D* chart = getCurSubWindowChart();
                res = drawIntervalCurve({datas},chart);
            }
            return ((res.size() > 0) ? res[0] : nullptr);
        }
        else
        {
            QList<QwtPlotIntervalCurve *> res = drawIntervalCurve({datas},nullptr);
            return ((res.size() > 0) ? res[0] : nullptr);
        }
    }
    return nullptr;
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

QList<QwtPlotHistogram *> SADrawDelegate::drawHistogramWithWizard()
{
    QList<QwtPlotHistogram *> res;
    return res;
}

QwtPlotHistogram *SADrawDelegate::drawHistogramWithWizard(SAAbstractDatas *boxSeries)
{
    QwtPlotHistogram* p = nullptr;
    return p;
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

QList<QwtPlotCurve *> SADrawDelegate::drawBoxWithWizard()
{
    QList<QwtPlotCurve *> res;
    return res;
}

QwtPlotTradingCurve *SADrawDelegate::drawBoxWithWizard(SAAbstractDatas *boxSeries)
{
    QwtPlotTradingCurve* p = nullptr;
    return p;
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

