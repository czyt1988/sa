#include "SAFigureOptCommands.h"

class SAFigureRemoveSeriesDatasInRangCommandPrivate
{
public:
    SAFigureRemoveSeriesDatasInRangCommandPrivate(SAChart2D *chart, QwtPlotItem *item)
        :m_chart(chart)
        ,m_item(item)
    {

    }
    virtual ~SAFigureRemoveSeriesDatasInRangCommandPrivate(){}
    virtual void redo() = 0;
    virtual void undo() = 0;
    virtual bool isValid() const = 0;
    SAChart2D* chart() const{return m_chart;}
    QwtPlotItem* item() const{return m_item;}
private:
    SAChart2D *m_chart;
    QwtPlotItem *m_item;
};


template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
class SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem : public SAFigureRemoveSeriesDatasInRangCommandPrivate
{
public:
    SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem(
            SAChart2D *chart
            ,QwtPlotItem *item
            ,const QPainterPath &selectRange
            );
    //~SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem();
    void init(const QPainterPath &selectRange);
    virtual void redo();
    virtual void undo();
    virtual bool isValid() const;
private:
    bool m_isValid;

};


template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>::SAFigureRemoveSeriesDatasInRangCommandPrivate_SeriesStoreItem(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QPainterPath &selectRange)
    :SAFigureRemoveSeriesDatasInRangCommandPrivate(chart,item)
{

}

///
/// \brief SAFigureRemoveSeriesDatasInRangCommand::SAFigureRemoveSeriesDatasInRangCommand
/// \param chart
/// \param item
/// \param selectRange
/// \param cmdName
/// \param parent
///
SAFigureRemoveSeriesDatasInRangCommand::SAFigureRemoveSeriesDatasInRangCommand(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QPainterPath &selectRange
        , const QString &cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,d_ptr(nullptr)
{

}

void SAFigureRemoveSeriesDatasInRangCommand::redo()
{
    if(d_ptr)
    {
        d_ptr->redo();
    }
}

void SAFigureRemoveSeriesDatasInRangCommand::undo()
{
    if(d_ptr)
    {
        d_ptr->undo();
    }
}

bool SAFigureRemoveSeriesDatasInRangCommand::isValid() const
{
    if(d_ptr)
    {
        return d_ptr->isValid();
    }
    return false;
}

