#ifndef SAFIGUREREPLACEDATASCOMMAND_H
#define SAFIGUREREPLACEDATASCOMMAND_H
#include "SAFigureOptCommands.h"
#include <functional>
#include "SAChart.h"
/////////////////////////////////////////////////////////////////////
///
///  Series值按照索引替换
///
/////////////////////////////////////////////////////////////////////

///
/// \brief 按照索引替换部分数据
///
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
class SAFigureReplaceDatasCommand : public SAFigureOptCommand
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


///
/// \brief 替换所有的图线数据
///
template<typename T,typename PlotItemType,typename FpSetSeriesSampleFun>
class SAFigureReplaceAllDatasCommand : public SAFigureOptCommand
{
public:
    SAFigureReplaceAllDatasCommand(SAChart2D* chart
                                , QwtPlotItem* item
                                , const QVector<T>& oldDatas
                                , const QVector<T>& newDatas
                                , const QString& cmdName
                                , FpSetSeriesSampleFun fpSetSample
                                , QUndoCommand *parent = Q_NULLPTR
                                );
    SAFigureReplaceAllDatasCommand(SAChart2D* chart
                                , QwtPlotItem* item
                                , const QVector<T>& newDatas
                                , const QString& cmdName
                                , FpSetSeriesSampleFun fpSetSample
                                , std::function<void(QwtPlotItem*,QVector<T>&)> fpGetSample
                                , QUndoCommand *parent = Q_NULLPTR
                                );
    ~SAFigureReplaceAllDatasCommand();
    virtual void redo();
    virtual void undo();
private:
    SAChart2D *m_chart;
    PlotItemType* m_plotItem;
    QVector<T> m_oldDatas;
    QVector<T> m_newDatas;
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



//////////////////////////////////////////////////

template<typename T, typename PlotItemType, typename FpSetSeriesSampleFun>
SAFigureReplaceAllDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>
::SAFigureReplaceAllDatasCommand(SAChart2D *chart
                                 , QwtPlotItem *item
                                 , const QVector<T> &oldDatas
                                 , const QVector<T> &newDatas
                                 , const QString &cmdName
                                 , FpSetSeriesSampleFun fpSetSample
                                 , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_fpSetSample(fpSetSample)
{
    m_plotItem = static_cast<PlotItemType*>(item);
    m_oldDatas = oldDatas;
    m_newDatas = newDatas;
}

template<typename T, typename PlotItemType, typename FpSetSeriesSampleFun>
SAFigureReplaceAllDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>
::SAFigureReplaceAllDatasCommand(SAChart2D *chart
                                 , QwtPlotItem *item
                                 , const QVector<T> &newDatas
                                 , const QString &cmdName
                                 , FpSetSeriesSampleFun fpSetSample
                                 , std::function<void(QwtPlotItem*,QVector<T>&)> fpGetSample
                                 , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_fpSetSample(fpSetSample)
{
    m_plotItem = static_cast<PlotItemType*>(item);
    fpGetSample(item,m_oldDatas);
    m_newDatas = newDatas;
}



template<typename T, typename PlotItemType, typename FpSetSeriesSampleFun>
SAFigureReplaceAllDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>
::~SAFigureReplaceAllDatasCommand()
{

}

template<typename T, typename PlotItemType, typename FpSetSeriesSampleFun>
void SAFigureReplaceAllDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>
::redo()
{
    m_fpSetSample(m_plotItem,m_newDatas);
}

template<typename T, typename PlotItemType, typename FpSetSeriesSampleFun>
void SAFigureReplaceAllDatasCommand<T,PlotItemType,FpSetSeriesSampleFun>
::undo()
{
    m_fpSetSample(m_plotItem,m_oldDatas);
}



#endif // SAFIGUREREPLACEDATASCOMMAND_H


