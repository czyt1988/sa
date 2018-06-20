#include "SAFigureTableCommands.h"
#include "SAChart.h"


class SAFigureTableInsertCommandPrivate
{
public:
    SAFigureTableInsertCommandPrivate(SAChart2D *chart, QwtPlotItem *item)
        :m_chart(chart)
        ,m_item(item)
    {

    }
    virtual ~SAFigureTableInsertCommandPrivate()
    {

    }
    //获取chart
    SAChart2D *chart() const{return m_chart;}
    //获取item
    QwtPlotItem *item() const{return m_item;}
    virtual void redo() = 0;
    virtual void undo() = 0;
    virtual bool isValid() const = 0;
private:
    SAChart2D *m_chart;
    QwtPlotItem *m_item;
};
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
class SAFigureTableInsertCommandPrivate_SeriesStoreItem : public SAFigureTableInsertCommandPrivate
{
public:
    SAFigureTableInsertCommandPrivate_SeriesStoreItem(
            SAChart2D *chart
            , QwtPlotItem *item
            , int indexRow
            , FpSetSeriesSampleFun fpSetSeries
            );
    void init();
    //api isValid
    virtual bool isValid() const;
    //api redo
    virtual void redo();
    //api undo
    virtual void undo();
private:
    bool m_isValid;
    FpSetSeriesSampleFun m_fpSetSample;///< fun(QwtPlotItem* item,cosnt QVector<T>& series);
    PlotItemType* m_plotItem;
    int m_indexRow;///< 插入的位置
};

template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
SAFigureTableInsertCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::SAFigureTableInsertCommandPrivate_SeriesStoreItem(
        SAChart2D *chart
        , QwtPlotItem *item
        , int indexRow
        , FpSetSeriesSampleFun fpSetSeries)
    :SAFigureTableInsertCommandPrivate(chart,item)
    ,m_isValid(false)
    ,m_fpSetSample(fpSetSeries)
    ,m_indexRow(indexRow)
{
    m_plotItem = static_cast<PlotItemType*>(item);
    init();
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureTableInsertCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::init()
{
    if(m_indexRow<0 || m_indexRow>((int)m_plotItem->dataSize()))
        return;
    m_isValid = true;
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
bool SAFigureTableInsertCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::isValid() const
{
    return m_isValid;
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureTableInsertCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::redo()
{
    if(!isValid())
        return;
    QVector<T> vecDatas;
    vecDatas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(vecDatas,m_plotItem);
    vecDatas.insert(m_indexRow,T());
    m_fpSetSample(item(),vecDatas);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureTableInsertCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::undo()
{
    if(!isValid())
        return;
    QVector<T> vecDatas;
    vecDatas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(vecDatas,m_plotItem);
    vecDatas.remove(m_indexRow);
    m_fpSetSample(item(),vecDatas);
}





















SAFigureTableInsertCommand::SAFigureTableInsertCommand(
        SAChart2D *chart
        , QwtPlotItem *item
        , int row
        , const QString &cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
{
    d_ptr = nullptr;
    switch(item->rtti ())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        d_ptr = new SAFigureTableInsertCommandPrivate_SeriesStoreItem
                <QPointF
                ,QwtPlotCurve
                ,decltype(&SAChart::setPlotCurveSample)
                >
                (chart,item,row,SAChart::setPlotCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        d_ptr = new SAFigureTableInsertCommandPrivate_SeriesStoreItem
                <QwtIntervalSample
                ,QwtPlotHistogram
                ,decltype(&SAChart::setPlotHistogramSample)
                >
                (chart,item,row,SAChart::setPlotHistogramSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        d_ptr = new SAFigureTableInsertCommandPrivate_SeriesStoreItem
                <QwtIntervalSample
                ,QwtPlotIntervalCurve
                ,decltype(&SAChart::setPlotIntervalCurveSample)
                >
                (chart,item,row,SAChart::setPlotIntervalCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        d_ptr = new SAFigureTableInsertCommandPrivate_SeriesStoreItem
                <QwtSetSample
                ,QwtPlotMultiBarChart
                ,decltype(&SAChart::setPlotMultiBarChartSample)
                >
                (chart,item,row,SAChart::setPlotMultiBarChartSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        d_ptr = new SAFigureTableInsertCommandPrivate_SeriesStoreItem
                <QwtOHLCSample
                ,QwtPlotTradingCurve
                ,decltype(&SAChart::setPlotTradingCurveSample)
                >
                (chart,item,row,SAChart::setPlotTradingCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        d_ptr = new SAFigureTableInsertCommandPrivate_SeriesStoreItem
                <QwtPoint3D
                ,QwtPlotSpectroCurve
                ,decltype(&SAChart::setPlotSpectroCurveSample)
                >
                (chart,item,row,SAChart::setPlotSpectroCurveSample);
        break;
    }
    default:
        break;
    }
}

SAFigureTableInsertCommand::~SAFigureTableInsertCommand()
{
    if(d_ptr)
        delete d_ptr;
}

void SAFigureTableInsertCommand::redo()
{
    if(d_ptr)
        d_ptr->redo();
}

void SAFigureTableInsertCommand::undo()
{
    if(d_ptr)
        d_ptr->undo();
}

bool SAFigureTableInsertCommand::isValid() const
{
    if(d_ptr)
        return d_ptr->isValid();
    return false;
}


