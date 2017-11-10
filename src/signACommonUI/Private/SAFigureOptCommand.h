#ifndef SAFIGUREOPTCOMMAND_H
#define SAFIGUREOPTCOMMAND_H
#include <QUndoCommand>
class SAChart2D;
class QwtPlotItem;
class SAAbstractDatas;


class SAFigureOptCommand : public QUndoCommand
{
public:
    SAFigureOptCommand(const QString &cmdName);
};

///
/// \brief 绘图元素的添加
///
class SAFigureChartItemAddCommand : public SAFigureOptCommand
{
public:
    SAFigureChartItemAddCommand(SAChart2D* chart,QwtPlotItem *ser,const QString &cmdName);
    ~SAFigureChartItemAddCommand();
    virtual void redo();
    virtual void undo();
private:
    SAChart2D* m_chart;
    QwtPlotItem* m_item;
};

///
/// \brief 绘图元素的删除
///
class SAFigureChartItemDeleteCommand : public SAFigureOptCommand
{
public:
    SAFigureChartItemDeleteCommand(SAChart2D* chart,QwtPlotItem *ser,const QString &cmdName);
    ~SAFigureChartItemDeleteCommand();
    virtual void redo();
    virtual void undo();
private:
    SAChart2D* m_chart;
    QwtPlotItem* m_item;
};
#endif // SAFIGUREOPTCOMMAND_H
