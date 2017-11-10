#include "SAFigureOptCommand.h"
#include <SAChart2D.h>
#include "SAXYSeries.h"
SAFigureOptCommand::SAFigureOptCommand(const QString &cmdName):QUndoCommand(cmdName)
{

}



SAFigureChartAddCurveCommand::SAFigureChartAddCurveCommand(SAChart2D *chart, SAXYSeries *ser,const QString &cmdName)
    :SAFigureOptCommand(cmdName)
    ,m_chart(chart)
    ,m_series(ser)
{

}

void SAFigureChartAddCurveCommand::redo()
{
    m_series->attach(m_chart);
}

void SAFigureChartAddCurveCommand::undo()
{
    m_series->detach();
}
