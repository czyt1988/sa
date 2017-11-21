#include "SAFigureOptCommand.h"

SAFigureOptCommand::SAFigureOptCommand(SAChart2D* chart,const QString &cmdName):QUndoCommand(cmdName)
  ,m_chart(chart)
{

}

SAChart2D *SAFigureOptCommand::plot()
{
    return m_chart;
}
