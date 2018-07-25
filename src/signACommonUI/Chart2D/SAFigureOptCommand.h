#ifndef SAFIGUREOPTCOMMAND_H
#define SAFIGUREOPTCOMMAND_H
#include <QUndoCommand>
#include "SACommonUIGlobal.h"
class SAChart2D;
class SAFigureWindow;
class SA_COMMON_UI_EXPORT SAFigureOptCommand : public QUndoCommand
{
public:
    SAFigureOptCommand(SAChart2D* chart2D,const QString &cmdName,QUndoCommand *parent = Q_NULLPTR);
    SAFigureOptCommand(SAFigureWindow* fig,const QString &cmdName,QUndoCommand *parent = Q_NULLPTR);
    SAChart2D* chart2D() const;
    SAFigureWindow* figure() const;
protected:
    SAChart2D* m_chart;
    SAFigureWindow* m_fig;
};

#endif // SAFIGUREOPTCOMMAND_H
