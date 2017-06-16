#include "SAChart2D.h"
#include "SAAbstractDatas.h"
#include "SARandColorMaker.h"
#include "SAFigureGlobalConfig.h"
#include <memory>
#include "SAXYSeries.h"
#include "SABarSeries.h"
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
    emit plotCurveChanged(series.get());
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
    emit plotCurveChanged(series.get());
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
    emit plotCurveChanged(series.get());
    return series.release();
}


void SAChart2D::addCurve(QwtPlotCurve *cur)
{
    cur->attach(this);
}
///
/// \brief SAChart2D::addBar
/// \param datas
/// \return
///
SABarSeries *SAChart2D::addBar(SAAbstractDatas *datas)
{
    std::unique_ptr<SABarSeries> series(new SABarSeries(datas,datas->getName()));
    if(series->dataSize() <= 0)
    {
        return nullptr;
    }
    QColor clr = SARandColorMaker::getCurveColor();
    series->setBrush(QBrush(clr));
    series->setPen(clr,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
    series->setStyle(QwtPlotHistogram::Columns);
    series->attach(this);
    emit plotCurveChanged(series.get());
    return series.release();
}

///
/// \brief 移除范围内数据
/// \param curves 需要移除的曲线列表
///
void SAChart2D::removeDataInRang(QList<QwtPlotCurve *> curves)
{
    setAutoReplot(false);
    QRectF rang = getPlottingRegionRang();
    for(int i=0;i<curves.size();++i)
    {
        removeDataInRang(rang,curves[i]);
    }
    setAutoReplot(true);
}


