#ifndef SAFIGUREOPTCOMMAND_H
#define SAFIGUREOPTCOMMAND_H
#include <QUndoCommand>
class SAChart2D;
class SAXYSeries;
class SAAbstractDatas;
class SAFigureOptCommand : public QUndoCommand
{
public:
    SAFigureOptCommand(const QString &cmdName);
};

///
/// \brief zhic
///
class SAFigureChartAddCurveCommand : public SAFigureOptCommand
{
public:
    SAFigureChartAddCurveCommand(SAChart2D* chart,SAXYSeries *ser,const QString &cmdName);
    virtual void redo();
    virtual void undo();
private:
    SAChart2D* m_chart;
    SAXYSeries* m_series;
};

#endif // SAFIGUREOPTCOMMAND_H
