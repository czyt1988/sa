#include "SAFigureOptCommand.h"
#include <SAChart2D.h>
#include "SAChart.h"
#include "SAXYSeries.h"
#include "qwt_plot_item.h"
SAFigureOptCommand::SAFigureOptCommand(SAChart2D* chart,const QString &cmdName):QUndoCommand(cmdName)
  ,m_chart(chart)
{

}

SAChart2D *SAFigureOptCommand::plot()
{
    return m_chart;
}



SAFigureChartItemAddCommand::SAFigureChartItemAddCommand(SAChart2D *chart, QwtPlotItem *ser, const QString &cmdName)
    :SAFigureOptCommand(chart,cmdName)
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

SAFigureChartItemDeleteCommand::SAFigureChartItemDeleteCommand(SAChart2D *chart, QwtPlotItem *ser, const QString &cmdName)
    :SAFigureOptCommand(chart,cmdName)
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

SAFigureChartItemListAddCommand::SAFigureChartItemListAddCommand(SAChart2D *chart, const QList<QwtPlotItem *> &itemList, const QString &cmdName)
    :SAFigureOptCommand(chart,cmdName)
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

SAFigureChartSelectionRegionAddCommand::SAFigureChartSelectionRegionAddCommand(SAChart2D *chart, const QPainterPath &newRegion, const QString &cmdName)
    :SAFigureOptCommand(chart,cmdName)
    ,m_newPainterPath(newRegion)
{
    m_oldPainterPath = plot()->getSelectionRange();
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
SAFigureRemoveCurveDataInRangCommand::SAFigureRemoveCurveDataInRangCommand(SAChart2D *chart, QList<QwtPlotCurve *> curves, const QString &cmdName)
    :SAFigureOptCommand(chart,cmdName)
    ,m_curveList(curves)
{
    recordePlotCureData();
}

void SAFigureRemoveCurveDataInRangCommand::recordePlotCureData()
{
    const int count = m_curveList.size();
    for(int i=0;i<count;++i)
    {
        QwtPlotCurve *cur = m_curveList[i];
        Q_ASSERT_X(cur != nullptr,"recordePlotCureData","null curve ptr");
        cur->swapData()
    }
}
