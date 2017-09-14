#include "SAChart2D.h"
#include "SAChart.h"
#include "SAAbstractDatas.h"
#include "SARandColorMaker.h"
#include "SAFigureGlobalConfig.h"
#include <memory>
#include "SAXYSeries.h"
#include "SABarSeries.h"
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include "SAValueManagerMimeData.h"
#include "SAValueManager.h"
#include "SATendencyChartDataSelectDialog.h"
#include "SALog.h"
#include "SARectSelectEditor.h"
SAChart2D::SAChart2D(QWidget *parent):SA2DGraph(parent)
{
    setAcceptDrops(true);
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
    std::unique_ptr<SAXYSeries> series(new SAXYSeries(name));
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

void SAChart2D::startSelectMode()
{
    if(nullptr == m_chartRectEditor)
    {
        m_chartRectEditor.reset(new SARectSelectEditor(this));
    }
    m_chartRectEditor->setEnable(true);
}

void SAChart2D::stopSelectMode()
{
    if(nullptr == m_chartRectEditor)
    {
        return;
    }
    m_chartRectEditor->setEnable(false);
}
///
/// \brief 向chart添加一组数据
/// \param datas
///
void SAChart2D::addDatas(const QList<SAAbstractDatas *> &datas)
{
    const int dataCount = datas.size();
    if(1 == dataCount)
    {
        if(SA::Dim1 == datas[0]->getDim())
        {
            SATendencyChartDataSelectDialog dlg(this);
            if(QDialog::Accepted != dlg.exec())
            {
                return;
            }
            if(dlg.isFollow())
            {
                QwtPlotCurve* cur = dlg.getSelFollowCurs();
                std::unique_ptr<SAXYSeries> series(new SAXYSeries(datas[0]->getName()));
                QVector<double> x,y;
                if(!SAAbstractDatas::converToDoubleVector(datas[0],y))
                {
                    return;
                }
                SAChart::getXDatas(x,cur);
                if(x.size() < y.size())
                {
                    y.resize(x.size());
                }
                else
                {
                    x.resize(y.size());
                }
                series->setSamples(x,y);
                series->setPen(SARandColorMaker::getCurveColor()
                            ,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
                series->attach(this);
                emit plotCurveChanged(series.release());
            }
            else if(dlg.isSelDef())
            {
                double startData,addedData;
                dlg.getSelDefData(startData,addedData);
                addCurve(datas[0],startData,addedData);
            }
            return;
        }
        else
        {
            addCurve(datas[0]);
        }
    }
    else if (2 == dataCount)
    {
        if(SA::Dim1 == datas[0]->getDim()
                &&SA::Dim1 == datas[1]->getDim())
        {
            addCurve(datas[0],datas[1]
                    ,QString("%1-%2")
                    .arg(datas[0]->getName())
                    .arg(datas[1]->getName()));
        }
    }
    else
    {
        std::for_each(datas.cbegin(),datas.cend(),[this](SAAbstractDatas* data){
            addCurve(data);
        });
    }
}

void SAChart2D::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void SAChart2D::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void SAChart2D::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
    {
        QList<int> ids;
        if(SAValueManagerMimeData::getValueIDsFromMimeData(event->mimeData(),ids))
        {
            QList<SAAbstractDatas*> datas = saValueManager->findDatas(ids);
            addDatas(datas);
        }
    }
}


