#ifndef SAFIGUREREPLACEDATASCOMMAND_H
#define SAFIGUREREPLACEDATASCOMMAND_H
#include "SAFigureOptCommands.h"
#include "SAChart.h"
/////////////////////////////////////////////////////////////////////
///
///  Series值按照索引替换
///
/////////////////////////////////////////////////////////////////////

template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
class SA_COMMON_UI_EXPORT SAFigureReplaceDatasCommand : public SAFigureOptCommand
{
public:
    SAFigureReplaceDatasCommand(SAChart2D* chart
                                , QwtPlotItem* item
                                , const QVector<int>& indexs
                                , const QVector<T>& oldDatas
                                , const QVector<T>& newDatas
                                , const QString& cmdName
                                , FpSetSeriesSampleFun fpSetSample
                                , QUndoCommand *parent = Q_NULLPTR
                                );
    ~SAFigureReplaceDatasCommand();
    virtual void redo();
    virtual void undo();
private:
    SAChart2D *m_chart;
    PlotItemType* m_plotItem;
    QVector<T> m_oldDatas;
    QVector<T> m_newDatas;
    QVector<int> m_indexs;
    FpSetSeriesSampleFun m_fpSetSample;///< fun(QwtPlotItem* item,cosnt QVector<T>& series);
};




template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
SAFigureReplaceDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>::SAFigureReplaceDatasCommand(SAChart2D *chart
        , QwtPlotItem *item
        , const QVector<int> &indexs
        , const QVector<T> &oldDatas
        , const QVector<T> &newDatas
        , const QString& cmdName
        , FpSetSeriesSampleFun fpSetSample, QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_fpSetSample(fpSetSample)
{
    m_plotItem = static_cast<PlotItemType*>(item);
    m_oldDatas = oldDatas;
    m_newDatas = newDatas;
    m_indexs = indexs;
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
SAFigureReplaceDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>::~SAFigureReplaceDatasCommand()
{

}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureReplaceDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>::redo()
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
    m_fpSetSample(m_plotItem,datas);
}
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
void SAFigureReplaceDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>::undo()
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
    m_fpSetSample(m_plotItem,datas);
}









#endif // SAFIGUREREPLACEDATASCOMMAND_H
