#include "SAFigureOptCommands.h"
#include <SAChart2D.h>
#include "SAChart.h"
#include "SAXYSeries.h"
#include "qwt_plot_item.h"
#include "qwt_plot_barchart.h"
#include "SAAbstractRegionSelectEditor.h"



SAFigureChartItemAddCommand::SAFigureChartItemAddCommand(SAChart2D *chart, QwtPlotItem *ser, const QString &cmdName,QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_item(ser)
{

}

SAFigureChartItemAddCommand::~SAFigureChartItemAddCommand()
{
    if(nullptr == m_item->plot())
    {
        delete m_item;
    }
}

void SAFigureChartItemAddCommand::redo()
{
    m_item->attach(plot());
}

void SAFigureChartItemAddCommand::undo()
{
    m_item->detach();
}

SAFigureChartItemDeleteCommand::SAFigureChartItemDeleteCommand(SAChart2D *chart, QwtPlotItem *ser, const QString &cmdName,QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_item(ser)
{

}

SAFigureChartItemDeleteCommand::~SAFigureChartItemDeleteCommand()
{
    if(nullptr == m_item->plot())
    {
        delete m_item;
    }
}

void SAFigureChartItemDeleteCommand::redo()
{
    m_item->detach();
}

void SAFigureChartItemDeleteCommand::undo()
{
    m_item->attach(plot());
}

SAFigureChartItemListAddCommand::SAFigureChartItemListAddCommand(SAChart2D *chart, const QList<QwtPlotItem *> &itemList, const QString &cmdName,QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_itemList(itemList)
{

}

SAFigureChartItemListAddCommand::~SAFigureChartItemListAddCommand()
{
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        if(nullptr == m_itemList[i]->plot())
        {
            delete m_itemList[i];
            m_itemList[i];
        }
    }
}


void SAFigureChartItemListAddCommand::redo()
{
    plot()->setAutoReplot(false);
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        m_itemList[i]->attach(plot());
    }
    plot()->setAutoReplot(true);
    plot()->replot();
}

void SAFigureChartItemListAddCommand::undo()
{
    plot()->setAutoReplot(false);
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        m_itemList[i]->detach();
    }
    plot()->setAutoReplot(true);
    plot()->replot();
}

SAFigureChartSelectionRegionAddCommand::SAFigureChartSelectionRegionAddCommand(SAChart2D *chart, const QPainterPath &newRegion, const QString &cmdName, QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_newPainterPath(newRegion)
{
    m_oldPainterPath = plot()->getSelectionRange();
}

SAFigureChartSelectionRegionAddCommand::SAFigureChartSelectionRegionAddCommand(SAChart2D *chart, const QPainterPath &oldRegion, const QPainterPath &newRegion, const QString &cmdName, QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_oldPainterPath(oldRegion)
    ,m_newPainterPath(newRegion)
{

}

void SAFigureChartSelectionRegionAddCommand::redo()
{
    plot()->setSelectionRange(m_newPainterPath);
}

void SAFigureChartSelectionRegionAddCommand::undo()
{
    plot()->setSelectionRange(m_oldPainterPath);
}

///
/// \brief 移除曲线范围内的数据
/// \param chart
/// \param curves
/// \param cmdName
///
SAFigureRemoveCurveDataInRangCommand::SAFigureRemoveCurveDataInRangCommand(SAChart2D *chart, const QList<QwtPlotCurve *> &curves, const QString &cmdName, QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_curveList(curves)
    ,m_redoCount(0)
{
    recordPlotCureData(m_backupData);

    plot()->setAutoReplot(false);
    QPainterPath region = plot()->getSelectionRange();
    SAAbstractRegionSelectEditor* editor = plot()->getRegionSelectEditor();

    Q_ASSERT_X(editor != nullptr,"SAFigureRemoveCurveDataInRangCommand::redo","null RegionSelectEditor");

    QHash<QPair<int,int>,QPainterPath> otherScaleMap;
    const int count = m_curveList.size();
    for(int i=0;i<count;++i)
    {
        QwtPlotCurve* curve = m_curveList[i];
        int xa = curve->xAxis();
        int ya = curve->yAxis();
        if(xa == editor->getXAxis() && ya == editor->getYAxis())
        {
            SAChart::removeDataInRang(region,curve);
        }
        else
        {
            QPair<int,int> axiss=qMakePair(xa,ya);
            if(!otherScaleMap.contains(axiss))
            {
                otherScaleMap[axiss] = editor->transformToOtherAxis(xa,ya);
            }
            SAChart::removeDataInRang(otherScaleMap.value(axiss)
                                      ,curve);
        }
    }
    plot()->setAutoReplot(true);
}

void SAFigureRemoveCurveDataInRangCommand::redo()
{
    if(0 == m_redoCount)
    {
        //首次进入不进行操作，首次操作在构造函数完成
        ++m_redoCount;
        plot()->replot();
        return;
    }
    recover();
}
///
/// \brief 记录现有曲线值，并把原来记录的曲线值还原
///
void SAFigureRemoveCurveDataInRangCommand::undo()
{
    recover();
}

void SAFigureRemoveCurveDataInRangCommand::recordPlotCureData(QList<QSharedPointer<QVector<QPointF> > > &recorder)
{
    const int count = m_curveList.size();
    for(int i=0;i<count;++i)
    {
        QwtPlotCurve *cur = m_curveList[i];
        Q_ASSERT_X(cur != nullptr,"recordePlotCureData","null curve ptr");
        QSharedPointer<QVector<QPointF> > vecPtr(new QVector<QPointF>);
        SAChart::getXYDatas(*vecPtr,cur);
        recorder.append(vecPtr);
    }
}

void SAFigureRemoveCurveDataInRangCommand::recover()
{
    Q_ASSERT_X(m_curveList.size() == m_backupData.size(),"SAFigureRemoveCurveDataInRangCommand::undo","recorde size not equal");
    plot()->setAutoReplot(false);
    const int count = m_curveList.size();
    //先记录现有的曲线值
    QList<QSharedPointer<QVector<QPointF> > > recordeCurveData;
    recordPlotCureData(recordeCurveData);
    //还原原来的记录
    for(int i=0;i<count;++i)
    {
        QwtPlotCurve *cur = m_curveList[i];
        cur->setSamples(*m_backupData[i]);
    }
    //替换记录
    m_backupData = recordeCurveData;
    plot()->setAutoReplot(true);
    plot()->replot();
}

SAFigureMovePointDataInIndexsCommand::SAFigureMovePointDataInIndexsCommand(SAChart2D *chart
                                                                           , SAFigureMoveSeriseDataInfoBase* baseInfo
                                                                           , const QString &cmdName
                                                                           , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_baseInfo(baseInfo)
{

}

SAFigureMovePointDataInIndexsCommand::~SAFigureMovePointDataInIndexsCommand()
{
    if(m_baseInfo)
    {
        delete m_baseInfo;
    }
}

void SAFigureMovePointDataInIndexsCommand::redo()
{
    //数据进行移动
    QwtPlotItem* item = m_baseInfo->item();
    switch(item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        SAFigureMoveSerisePointDataInfo* info = static_cast<SAFigureMoveSerisePointDataInfo*>(m_baseInfo);
        QVector<QPointF> xyData;
        QwtPlotCurve*cur = static_cast<QwtPlotCurve*>(item);
        SAChart::getXYDatas(xyData,cur);
        const int indexCount = info->indexs().size();
        const int inRangNewDataCount = info->newPoints().size();
        const int curveDataCount = info->originPoints().size();
        const QVector<int>& indexs = info->indexs();
        const QVector<QPointF>& newPoints = info->newPoints();
        for(int j=0;j<indexCount && j<inRangNewDataCount && j<curveDataCount;++j)
        {
            xyData[indexs[j]] = newPoints[j];
        }
        cur->setSamples(xyData);
        break;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        SAFigureMoveSerisePointDataInfo* info = static_cast<SAFigureMoveSerisePointDataInfo*>(m_baseInfo);
        QVector<QPointF> xyData;
        QwtPlotBarChart*bar = static_cast<QwtPlotBarChart*>(item);
        SAChart::getXYDatas(xyData,bar);
        const int indexCount = info->indexs().size();
        const int inRangNewDataCount = info->newPoints().size();
        const int curveDataCount = info->originPoints().size();
        const QVector<int>& indexs = info->indexs();
        const QVector<QPointF>& newPoints = info->newPoints();
        for(int j=0;j<indexCount && j<inRangNewDataCount && j<curveDataCount;++j)
        {
            xyData[indexs[j]] = newPoints[j];
        }
        bar->setSamples(xyData);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    case QwtPlotItem::Rtti_PlotHistogram:
    case QwtPlotItem::Rtti_PlotSpectrogram:
    case QwtPlotItem::Rtti_PlotTradingCurve:
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    default:
        break;
    }
}

void SAFigureMovePointDataInIndexsCommand::undo()
{
    QwtPlotItem* item = m_baseInfo->item();
    switch(item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        SAFigureMoveSerisePointDataInfo* info = static_cast<SAFigureMoveSerisePointDataInfo*>(m_baseInfo);
        QVector<QPointF> xyData;
        QwtPlotCurve*cur = static_cast<QwtPlotCurve*>(item);
        SAChart::getXYDatas(xyData,cur);
        const int indexCount = info->indexs().size();
        const int inRangNewDataCount = info->newPoints().size();
        const int curveDataCount = info->originPoints().size();
        for(int j=0;j<indexCount && j<inRangNewDataCount && j<curveDataCount;++j)
        {
            xyData[info->indexs()[j]] = info->originPoints()[j];
        }
        cur->setSamples(xyData);
        break;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        SAFigureMoveSerisePointDataInfo* info = static_cast<SAFigureMoveSerisePointDataInfo*>(m_baseInfo);
        QVector<QPointF> xyData;
        QwtPlotBarChart*bar = static_cast<QwtPlotBarChart*>(item);
        SAChart::getXYDatas(xyData,bar);
        const int indexCount = info->indexs().size();
        const int inRangNewDataCount = info->newPoints().size();
        const int curveDataCount = info->originPoints().size();
        for(int j=0;j<indexCount && j<inRangNewDataCount && j<curveDataCount;++j)
        {
            xyData[info->indexs()[j]] = info->originPoints()[j];
        }
        bar->setSamples(xyData);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    case QwtPlotItem::Rtti_PlotHistogram:
    case QwtPlotItem::Rtti_PlotSpectrogram:
    case QwtPlotItem::Rtti_PlotTradingCurve:
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    default:
        break;
    }
}
