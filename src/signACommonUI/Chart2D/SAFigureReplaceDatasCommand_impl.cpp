#include "SAFigureOptCommands.h"



class SAFigureReplaceDatasCommandPrivate
{
public:
    SAFigureReplaceDatasCommandPrivate(SAChart2D *chart
                                       ,QwtPlotItem *item)
        :m_chart(chart)
        ,m_item(item)
    {
    }
    virtual ~SAFigureReplaceDatasCommandPrivate(){}
    virtual void redo() = 0;
    virtual void undo() = 0;
    SAChart2D *chart() const;
    QwtPlotItem *item() const;
private:
    SAChart2D *m_chart;
    QwtPlotItem *m_item;
};

SAChart2D *SAFigureReplaceDatasCommandPrivate::chart() const
{
    return m_chart;
}


QwtPlotItem *SAFigureReplaceDatasCommandPrivate::item() const
{
    return m_item;
}


template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
class SAFigureReplaceDatasCommandPrivate_SeriesStoreItem : public SAFigureReplaceDatasCommandPrivate
{
public:
    SAFigureReplaceDatasCommandPrivate_SeriesStoreItem(
            SAChart2D *chart
           , QwtPlotItem *item
           , const QVector<int> &indexs
           , const QVector<T> &oldData
           , const QVector<T> &newData
           , FpSetSeriesSampleFun fpSetSample
            );
    virtual void redo();
    virtual void undo();
private:
    PlotItemType* m_plotItem;
    QVector<T> m_oldDatas;
    QVector<T> m_newDatas;
    QVector<int> m_indexs;
    FpSetSeriesSampleFun m_fpSetSample;///< fun(QwtPlotItem* item,cosnt QVector<T>& series);
};

template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
SAFigureReplaceDatasCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::SAFigureReplaceDatasCommandPrivate_SeriesStoreItem(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QVector<int> &indexs
        , const QVector<T> &oldData
        , const QVector<T> &newData
        , FpSetSeriesSampleFun fpSetSample
        )
    :SAFigureReplaceDatasCommandPrivate(chart,item)
    ,m_fpSetSample(fpSetSample)
{
    m_plotItem = static_cast<PlotItemType*>(item);
    m_oldDatas = oldData;
    m_newDatas = newData;
    m_indexs = indexs;
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureReplaceDatasCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::redo()
{
    QVector<T> datas;
    datas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(datas,m_plotItem);
    for(int i=0;i<m_indexs.size()&&i<m_newDatas.size();++i)
    {
        if(m_indexs[i] < datas.size())
        {
            datas[m_indexs[i]] = m_newDatas[i];
        }
    }
    m_fpSetSample(item(),datas);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureReplaceDatasCommandPrivate_SeriesStoreItem<T,PlotItemType,FpSetSeriesSampleFun>
::undo()
{
    QVector<T> datas;
    datas.reserve(m_plotItem->dataSize());
    SAChart::getSeriesData(datas,m_plotItem);
    for(int i=0;i<m_indexs.size()&&i<m_oldDatas.size();++i)
    {
        if(m_indexs[i] < datas.size())
        {
            datas[m_indexs[i]] = m_oldDatas[i];
        }
    }
    m_fpSetSample(item(),datas);
}



















template<typename T>
SAFigureReplaceDatasCommand<T>::SAFigureReplaceDatasCommand(
        SAChart2D *chart
        , QwtPlotItem *item
        , const QVector<int> &indexs
        , const QVector<T> &oldDatas
        , const QVector<T> &newDatas
        , const QString& cmdName
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
{
    d_ptr = nullptr;

    switch(item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        typedef SAFigureReplaceDatasCommandPrivate_SeriesStoreItem
                <T
                ,QwtPlotCurve
                ,decltype(&SAChart::setPlotCurveSample)
                > PriType;
        d_ptr = new PriType(chart,item,indexs,oldDatas,newDatas,&SAChart::setPlotCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        typedef SAFigureReplaceDatasCommandPrivate_SeriesStoreItem
                <T
                ,QwtPlotBarChart
                ,decltype(&SAChart::setPlotBarChartSample)
                > PriType;
        d_ptr = new PriType(chart,item,indexs,oldDatas,newDatas,&SAChart::setPlotBarChartSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        typedef SAFigureReplaceDatasCommandPrivate_SeriesStoreItem
                <T
                ,QwtPlotSpectroCurve
                ,decltype(&SAChart::setPlotSpectroCurveSample)
                > PriType;
        d_ptr = new PriType(chart,item,indexs,oldDatas,newDatas,&SAChart::setPlotSpectroCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        typedef SAFigureReplaceDatasCommandPrivate_SeriesStoreItem
                <T
                ,QwtPlotIntervalCurve
                ,decltype(&SAChart::setPlotIntervalCurveSample)
                > PriType;
        d_ptr = new PriType(chart,item,indexs,oldDatas,newDatas,&SAChart::setPlotIntervalCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        typedef SAFigureReplaceDatasCommandPrivate_SeriesStoreItem
                <T
                ,QwtPlotHistogram
                ,decltype(&SAChart::setPlotHistogramSample)
                > PriType;
        d_ptr = new PriType(chart,item,indexs,oldDatas,newDatas,&SAChart::setPlotHistogramSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        typedef SAFigureReplaceDatasCommandPrivate_SeriesStoreItem
                <T
                ,QwtPlotTradingCurve
                ,decltype(&SAChart::setPlotTradingCurveSample)
                > PriType;
        d_ptr = new PriType(chart,item,indexs,oldDatas,newDatas,&SAChart::setPlotTradingCurveSample);
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        typedef SAFigureReplaceDatasCommandPrivate_SeriesStoreItem
                <T
                ,QwtPlotMultiBarChart
                ,decltype(&SAChart::setPlotMultiBarChartSample)
                > PriType;
        d_ptr = new PriType(chart,item,indexs,oldDatas,newDatas,&SAChart::setPlotMultiBarChartSample);
        break;
    }
    default:
        break;
    }

}
template<typename T>
SAFigureReplaceDatasCommand<T>::~SAFigureReplaceDatasCommand()
{
    if(d_ptr)
        delete d_ptr;
}
template<typename T>
void SAFigureReplaceDatasCommand<T>::redo()
{
    if(d_ptr)
        d_ptr->redo();
}
template<typename T>
void SAFigureReplaceDatasCommand<T>::undo()
{
    if(d_ptr)
        d_ptr->undo();
}









