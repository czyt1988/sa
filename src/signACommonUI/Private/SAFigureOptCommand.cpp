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

SAFigureChartItemListAddCommand::SAFigureChartItemListAddCommand(SAChart2D *chart, const QList<QwtPlotItem *> &itemList, const QString &cmdName)
    :SAFigureOptCommand(cmdName)
    ,m_chart(chart)
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
    m_chart->setAutoReplot(false);
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        m_itemList[i]->attach(m_chart);
    }
    m_chart->setAutoReplot(true);
}

void SAFigureChartItemListAddCommand::undo()
{
    m_chart->setAutoReplot(false);
    const int size = m_itemList.size();
    for(int i=0;i<size;++i)
    {
        m_itemList[i]->detach();
    }
    m_chart->setAutoReplot(true);
}
