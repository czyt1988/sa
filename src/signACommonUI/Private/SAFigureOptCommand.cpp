#include "SAFigureOptCommand.h"
#include <SAChart2D.h>
#include "SAXYSeries.h"
#include "qwt_plot_item.h"
SAFigureOptCommand::SAFigureOptCommand(const QString &cmdName):QUndoCommand(cmdName)
{

}



SAFigureChartItemAddCommand::SAFigureChartItemAddCommand(SAChart2D *chart, QwtPlotItem *ser, const QString &cmdName)
    :SAFigureOptCommand(cmdName)
    ,m_chart(chart)
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
    m_item->attach(m_chart);
}

void SAFigureChartItemAddCommand::undo()
{
    m_item->detach();
}

SAFigureChartItemDeleteCommand::SAFigureChartItemDeleteCommand(SAChart2D *chart, QwtPlotItem *ser, const QString &cmdName)
    :SAFigureOptCommand(cmdName)
    ,m_chart(chart)
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
    m_item->attach(m_chart);
}
