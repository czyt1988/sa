#ifndef SAFIGUREOPTCOMMAND_H
#define SAFIGUREOPTCOMMAND_H
#include <QUndoCommand>
#include "SACommonUIGlobal.h"
class SAChart2D;

class SA_COMMON_UI_EXPORT SAFigureOptCommand : public QUndoCommand
{
public:
    SAFigureOptCommand(SAChart2D* chart,const QString &cmdName);
    SAChart2D* plot();
protected:
    SAChart2D* m_chart;
};

#endif // SAFIGUREOPTCOMMAND_H
