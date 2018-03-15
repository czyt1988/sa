#include "SAFigureOptCommands.h"
#include <SAChart2D.h>
#include "SAChart.h"
#include "SAXYSeries.h"
#include "qwt_plot_item.h"
#include "qwt_plot_barchart.h"
#include "SAAbstractRegionSelectEditor.h"
#include "qwt_series_data.h"
#include "czyAlgorithm.h"

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
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        m_itemList[i]->attach(plot());
    }
}

void SAFigureChartItemListAddCommand::undo()
{
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        m_itemList[i]->detach();
    }
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
SAFigureRemoveXYSeriesDataInRangCommand::SAFigureRemoveXYSeriesDataInRangCommand(SAChart2D* chart
                                                                                 , QwtSeriesStore<QPointF>* curve
                                                                                 , const QString &cmdName
                                                                                 , int xaxis
                                                                                 , int yaxis
                                                                                 , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_curve(curve)
{
    SAChart::getXYDatas(m_oldData,m_curve);//记录原有的数据
    QPainterPath region = plot()->getSelectionRange();
    SAAbstractRegionSelectEditor* editor = plot()->getRegionSelectEditor();
    if(editor)
    {
        if(xaxis != editor->getXAxis() || yaxis != editor->getYAxis())
        {
            region = editor->transformToOtherAxis(xaxis,yaxis);
        }
    }
    SAChart::removeDataInRang(region,m_oldData,m_newData);
}

void SAFigureRemoveXYSeriesDataInRangCommand::redo()
{
   m_curve->setData(new QwtPointSeriesData(m_newData));
}

void SAFigureRemoveXYSeriesDataInRangCommand::undo()
{
    m_curve->setData(new QwtPointSeriesData(m_oldData));
}

//========================================

///
/// \brief 曲线QwtPlotCurve值变更
/// \param chart
/// \param curve
/// \param cmdName
/// \param newPoints
/// \param parent
///
SAFigureChangeXYSeriesDataCommand::SAFigureChangeXYSeriesDataCommand(SAChart2D *chart
                                                                     , QwtSeriesStore<QPointF> *curve
                                                                     , const QString &cmdName
                                                                     ,const QVector<QPointF>& newPoints
                                                                     , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_curve(curve)
    ,m_newData(newPoints)
{
    SAChart::getXYDatas(m_oldData,m_curve);//记录原有的数据
}

void SAFigureChangeXYSeriesDataCommand::redo()
{
    m_curve->setData(new QwtPointSeriesData(m_newData));
}

void SAFigureChangeXYSeriesDataCommand::undo()
{
    m_curve->setData(new QwtPointSeriesData(m_oldData));
}





//================================
SAFigureChangeXYZSeriesDataCommand::SAFigureChangeXYZSeriesDataCommand(
        SAChart2D *chart
        , QwtSeriesStore<QwtPoint3D> *curve
        , const QString &cmdName
        , const QVector<QwtPoint3D> &newPoints
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_curve(curve)
    ,m_newData(newPoints)
{
    SAChart::getXYZDatas(m_oldData,m_curve);//记录原有的数据
}

void SAFigureChangeXYZSeriesDataCommand::redo()
{
    m_curve->setData(new QwtPoint3DSeriesData(m_newData));
}

void SAFigureChangeXYZSeriesDataCommand::undo()
{
    m_curve->setData(new QwtPoint3DSeriesData(m_oldData));
}

//==================================



