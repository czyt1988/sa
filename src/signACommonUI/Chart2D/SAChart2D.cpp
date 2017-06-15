#include "SAChart2D.h"
#include "SAAbstractDatas.h"
#include "SAXYSeries.h"
#include "SARandColorMaker.h"
#include "SAFigureGlobalConfig.h"
#include <memory>
SAChart2D::SAChart2D(QWidget *parent):SA2DGraph(parent)
{

}

SAChart2D::~SAChart2D()
{

}
///
/// \brief 添加曲线
/// \param datas
/// \return
///
SAXYSeries *SAChart2D::addCurve(SAAbstractDatas *datas)
{
    std::unique_ptr<SAXYSeries> series(new SAXYSeries(datas->getName(),datas));
    if(series->dataSize() <= 0)
    {
        return nullptr;
    }
    series->setPen(SARandColorMaker::getCurveColor()
                ,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
    series->attach(this);
    return series.release();
}
///
/// \brief SAChart2D::addCurve
/// \param datas
/// \param xStart
/// \param xDetal
/// \return
///
SAXYSeries *SAChart2D::addCurve(SAAbstractDatas *datas, double xStart, double xDetal)
{
    std::unique_ptr<SAXYSeries> series(new SAXYSeries(datas->getName()));
    series->setSamples(datas,xStart,xDetal);
    if(series->dataSize() <= 0)
    {
        return nullptr;
    }
    series->setPen(SARandColorMaker::getCurveColor()
                ,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
    series->attach(this);
    return series.release();
}
///
/// \brief SAChart2D::addCurve
/// \param x
/// \param y
/// \param name
/// \return
///
SAXYSeries *SAChart2D::addCurve(SAAbstractDatas *x, SAAbstractDatas *y, const QString &name)
{
    std::unique_ptr<SAXYSeries> series(new SAXYSeries());
    series->setSamples(x,y);
    if(series->dataSize() <= 0)
    {
        return nullptr;
    }
    series->setPen(SARandColorMaker::getCurveColor()
                ,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
    series->attach(this);
    return series.release();
}

QwtPlotCurve *SAChart2D::addCurve(const QVector<QPointF> &xyDatas)
{
    return SA2DGraph::addCurve(xyDatas);
}

QwtPlotCurve *SAChart2D::addCurve(const double *xData, const double *yData, int size)
{
    return SA2DGraph::addCurve(xData,yData,size);
}

QwtPlotCurve *SAChart2D::addCurve(const QVector<double> &xData, const QVector<double> &yData)
{
    return SA2DGraph::addCurve(xData,yData);
}

void SAChart2D::addCurve(QwtPlotCurve *cur)
{
    cur->attach(this);
}


