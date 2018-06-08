#include "SAFigureOptCommands.h"


///
/// \brief The SAFigureEditSeriesDataCommandPrivate class
/// 通用impl接口
///
template<typename T,typename FpSetFun>
class SAFigureEditSeriesDataCommandPrivate
{
public:
    SAFigureEditSeriesDataCommandPrivate(SAChart2D *chart, QwtPlotItem *item){}
    virtual ~SAFigureEditSeriesDataCommandPrivate(){}
    //获取chart
    SAChart2D *chart() const{return m_chart;}
    //获取item
    QwtPlotItem *item() const{return m_item;}
    //api isValid
    virtual bool isValid() const = 0;
    //api redo
    virtual bool redo() = 0;
    //api undo
    virtual bool undo() = 0;
private:
    SAChart2D *m_chart;
    QwtPlotItem *m_item;

};



class SAFigureEditSeriesDataCommandPrivateSeries






SAFigureEditSeriesDataCommand::SAFigureEditSeriesDataCommand(
        SAChart2D *chart
        , QwtPlotItem *item
        , double val
        , int indexRow
        , int indexCol
        , const QString &cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
{
    switch(item->rtti ())
    {
    case QwtPlotItem::Rtti_PlotCurve:
        //d_ptr = new
        break;
    default:
        break;
    }
}

void SAFigureEditSeriesDataCommand::redo()
{
    if(d_ptr)
    {
       d_ptr->redo ();
    }
}

void SAFigureEditSeriesDataCommand::undo()
{
    if(d_ptr)
    {
       d_ptr->undo ();
    }
}

bool SAFigureEditSeriesDataCommand::isValid() const
{
    if(d_ptr)
    {
        return d_ptr->isValid ();
    }
    return false;
}



