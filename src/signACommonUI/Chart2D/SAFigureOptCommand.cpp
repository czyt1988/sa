#include "SAFigureOptCommand.h"
#include "SAChart2D.h"
#include <QByteArray>
#include "SAFigureWindow.h"
SAFigureOptCommand::SAFigureOptCommand(SAChart2D* chart, const QString &cmdName, QUndoCommand *parent):QUndoCommand(cmdName,parent)
  ,m_chart(chart)
{
    m_fig = qobject_cast<SAFigureWindow*>(chart->parentWidget());
}

SAFigureOptCommand::SAFigureOptCommand(SAFigureWindow *fig, const QString &cmdName, QUndoCommand *parent):QUndoCommand(cmdName,parent)
  ,m_fig(fig)
{
    m_chart = nullptr;
}

SAChart2D *SAFigureOptCommand::chart2D() const
{
    return m_chart;
}

SAFigureWindow *SAFigureOptCommand::figure() const
{
    return m_fig;
}
