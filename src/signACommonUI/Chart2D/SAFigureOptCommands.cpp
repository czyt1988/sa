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

SAFigureChartItemDeleteCommand::SAFigureChartItemDeleteCommand(SAChart2D *chart, QwtPlotItem *item, const QString &cmdName,QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_item(item)
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
    ,m_newPainterPath(newRegion)
    ,m_oldPainterPath(oldRegion)
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



