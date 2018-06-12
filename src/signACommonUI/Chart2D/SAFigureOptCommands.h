#ifndef SAFIGUREOPTCOMMANDS_H
#define SAFIGUREOPTCOMMANDS_H
#include "SAFigureOptCommand.h"
#include <memory>
#include <QList>
#include <QVector>
#include "SAChart.h"
#include <QSharedPointer>
#include "qwt_plot_item.h"
#include "qwt_series_data.h"
#include "qwt_samples.h"
#include "qwt_point_3d.h"
#include "qwt_series_data.h"
#include "qwt_series_store.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_spectrocurve.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_tradingcurve.h"
#include "qwt_plot_multi_barchart.h"
class SAAbstractDatas;
class QwtPlotCurve;


///
/// \brief 绘图元素的添加
///
class SA_COMMON_UI_EXPORT SAFigureChartItemAddCommand : public SAFigureOptCommand
{
public:
    SAFigureChartItemAddCommand(SAChart2D* chart,QwtPlotItem *ser,const QString &cmdName,QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureChartItemAddCommand();
    virtual void redo();
    virtual void undo();
private:
    SAChart2D* m_chart;
    QwtPlotItem* m_item;
};

///
/// \brief 绘图元素群的添加
///
class SA_COMMON_UI_EXPORT SAFigureChartItemListAddCommand : public SAFigureOptCommand
{
public:
    SAFigureChartItemListAddCommand(SAChart2D* chart,const QList<QwtPlotItem*>& itemList,const QString &cmdName,QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureChartItemListAddCommand();
    virtual void redo();
    virtual void undo();
private:
    SAChart2D* m_chart;
    QList<QwtPlotItem*> m_itemList;
};

///
/// \brief 绘图元素的删除
///
class SA_COMMON_UI_EXPORT SAFigureChartItemDeleteCommand : public SAFigureOptCommand
{
public:
    SAFigureChartItemDeleteCommand(SAChart2D* chart, QwtPlotItem *item, const QString &cmdName, QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureChartItemDeleteCommand();
    virtual void redo();
    virtual void undo();
private:
    SAChart2D* m_chart;
    QwtPlotItem* m_item;
};

///
/// \brief 图像添加选区
///
class SA_COMMON_UI_EXPORT SAFigureChartSelectionRegionAddCommand : public SAFigureOptCommand
{
public:
    SAFigureChartSelectionRegionAddCommand(SAChart2D* chart,const QPainterPath& newRegion,const QString &cmdName, QUndoCommand *parent = Q_NULLPTR);
    SAFigureChartSelectionRegionAddCommand(SAChart2D* chart,const QPainterPath& oldRegion,const QPainterPath& newRegion,const QString &cmdName, QUndoCommand *parent = Q_NULLPTR);
    virtual void redo();
    virtual void undo();
private:
    QPainterPath m_newPainterPath;
    QPainterPath m_oldPainterPath;
};


////////////////////////////////////////////////////
///   Series值的范围移除
////////////////////////////////////////////////////
class SAFigureRemoveSeriesDatasInRangCommandPrivate;
///
/// \brief 移除选区范围内的数据
///
class SA_COMMON_UI_EXPORT SAFigureRemoveSeriesDatasInRangCommand : public SAFigureOptCommand
{
public:
    SAFigureRemoveSeriesDatasInRangCommand(SAChart2D* chart
                                            , QwtPlotItem* item
                                            , const QPainterPath& selectRange
                                            , const QString &cmdName
                                            , QUndoCommand *parent = Q_NULLPTR);
    virtual ~SAFigureRemoveSeriesDatasInRangCommand();
    virtual void redo();
    virtual void undo();
    bool isValid() const;
private:
    SAFigureRemoveSeriesDatasInRangCommandPrivate* d_ptr;
};




///
/// \brief 移除曲线QwtPlotCurve范围内的数据
///
class SA_COMMON_UI_EXPORT SAFigureRemoveXYSeriesDataInRangCommand : public SAFigureOptCommand
{
public:
    SAFigureRemoveXYSeriesDataInRangCommand(SAChart2D* chart
                                            , QwtSeriesStore<QPointF> *curve
                                            , const QString &cmdName
                                            , int xaxis
                                            , int yaxis
                                            , QUndoCommand *parent = Q_NULLPTR);
    virtual void redo();
    virtual void undo();
private:
    QwtSeriesStore<QPointF>* m_curve;
    QVector<QPointF> m_newData;
    QVector<QPointF> m_oldData;
};

/////////////////////////////////////////////////////////////////////
///
///  Series值的编辑，这个编辑是对应表格的编辑，需要提供indexRow和indexCol信息
///
/////////////////////////////////////////////////////////////////////
class SAFiguresEditSeriesCommandPrivate;
///
/// \brief 编辑序列数值的命令在编辑的位置超过原有位置时，会把长度扩展到指定的长度，并初始化为默认的数据
///
class SA_COMMON_UI_EXPORT SAFiguresEditSeriesCommand : public SAFigureOptCommand
{
public:
    SAFiguresEditSeriesCommand(SAChart2D* chart
                                  , QwtPlotItem* item
                                  , double val
                                  , int indexRow
                                  , int indexCol
                                  , const QString &cmdName = QString()
                                  , QUndoCommand *parent = Q_NULLPTR);
    ~SAFiguresEditSeriesCommand();
    virtual void redo();
    virtual void undo();
    //判断是否有效
    bool isValid() const;
    //判断是否尺寸有变化
    bool isSizeChanged() const;
private:
    SAFiguresEditSeriesCommandPrivate* d_ptr;
};







///
/// \brief 曲线QwtSeriesStore<QPointF>值变更
///
class SA_COMMON_UI_EXPORT SAFigureChangeXYSeriesDataCommand : public SAFigureOptCommand
{
public:
    SAFigureChangeXYSeriesDataCommand(SAChart2D* chart
                                      , QwtSeriesStore<QPointF> *curve
                                      , const QString &cmdName
                                      , const QVector<QPointF> &newPoints
                                      , QUndoCommand *parent = Q_NULLPTR);
    virtual void redo();
    virtual void undo();
private:
    QwtSeriesStore<QPointF>* m_curve;
    QVector<QPointF> m_newData;
    QVector<QPointF> m_oldData;
};
///
/// \brief 曲线QwtSeriesStore<QwtPoint3D>值变更
///
class SA_COMMON_UI_EXPORT SAFigureChangeXYZSeriesDataCommand : public SAFigureOptCommand
{
public:
    SAFigureChangeXYZSeriesDataCommand(SAChart2D* chart
                                      , QwtSeriesStore<QwtPoint3D> *curve
                                      , const QString &cmdName
                                      , const QVector<QwtPoint3D> &newPoints
                                      , QUndoCommand *parent = Q_NULLPTR);
    virtual void redo();
    virtual void undo();
private:
    QwtSeriesStore<QwtPoint3D>* m_curve;
    QVector<QwtPoint3D> m_newData;
    QVector<QwtPoint3D> m_oldData;
};


/////////////////////////////////////////////////////////////////////
///
///  Series值在指定索引开始顺序替换
///
/////////////////////////////////////////////////////////////////////

///
/// \brief Series值在指定索引开始插入newData，原有的值会被保留，如果startIndexs为-1，则从最后追加
///
template<typename T,typename TQwtSeries>
class SAFigureReplaceSeriesDataCommand : public SAFigureOptCommand
{
public:
    SAFigureReplaceSeriesDataCommand(SAChart2D* chart
                                         ,QwtSeriesStore<T> *curve
                                         ,const QString &cmdName
                                         ,const int startIndexs
                                         ,const QVector<T>& newData
                                         , QUndoCommand *parent = Q_NULLPTR);
    virtual void redo();
    virtual void undo();
private:
    QVector<T> m_oldData;
    const int mc_startIndex;
    QVector<T> m_newData;
    QwtSeriesStore<T> *m_curve;
};

template<typename T,typename TQwtSeries>
SAFigureReplaceSeriesDataCommand<T,TQwtSeries>::SAFigureReplaceSeriesDataCommand(
        SAChart2D *chart
        , QwtSeriesStore<T> *curve
        , const QString &cmdName
        , const int startIndexs
        , const QVector<T> &newData
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
      ,m_newData(newData)
      ,mc_startIndex(startIndexs)
      ,m_curve(curve)
{
    if(startIndexs >= 0)
    {
        int index = startIndexs;
        //说明数据从中间插入，记录旧的数据
        int end = ((size_t)(index + newData.size()) > curve->dataSize())
                ? curve->dataSize()
                : index + newData.size();
        int dis = end - index;
        if(dis>0)
            m_oldData.reserve(dis);
        while(index != end)
        {
            m_oldData.append(curve->sample(index));
            ++index;
        }
    }
}
template<typename T,typename TQwtSeries>
void SAFigureReplaceSeriesDataCommand<T,TQwtSeries>::redo()
{
    QVector<T> curveDatas;
    SAChart::getSeriesData(curveDatas,m_curve);
    if(mc_startIndex < 0)
    {
        //插入
        curveDatas.append(m_newData);
    }
    else
    {
        int newDataIndex = 0;
        const int newDataSize = m_newData.size();
        int index = mc_startIndex;
        const int curveDatasSize = curveDatas.size();
        while(newDataIndex < newDataSize && index < curveDatasSize)
        {
            curveDatas[index] = m_newData[newDataIndex];
            ++index;
            ++newDataIndex;
        }
        while(newDataIndex < newDataSize)
        {
            curveDatas.append(m_newData[newDataIndex]);
            ++newDataIndex;
        }
    }
    m_curve->setData(new TQwtSeries(curveDatas));
}
template<typename T,typename TQwtSeries>
void SAFigureReplaceSeriesDataCommand<T,TQwtSeries>::undo()
{
    QVector<T> curveDatas;
    SAChart::getSeriesData(curveDatas,m_curve);
    if(mc_startIndex < 0)
    {
        const int oldDataSize = curveDatas.size() - m_newData.size();
        if(oldDataSize >= 0)
            curveDatas.resize(oldDataSize);
    }
    else
    {
        //还原
        int oldDataIndex = 0;
        const int oldDataSize = m_oldData.size();
        int index = mc_startIndex;
        int curveDatasSize = curveDatas.size();
        if(mc_startIndex + m_oldData.size() < curveDatasSize)
        {
            //说明原来替换数据后还插入了新数据
            curveDatas.resize(mc_startIndex + m_oldData.size());
            curveDatasSize = curveDatas.size();
        }
        while(oldDataIndex < oldDataSize && index < curveDatasSize)
        {
            curveDatas[index] = m_newData[oldDataIndex];
            ++index;
            ++oldDataIndex;
        }
    }
    m_curve->setData(new TQwtSeries(curveDatas));
}


///
/// \brief 序列数据QPointF的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceXYSeriesDataCommand
        : public SAFigureReplaceSeriesDataCommand<QPointF,QwtPointSeriesData>
{
public:
    using SAFigureReplaceSeriesDataCommand::SAFigureReplaceSeriesDataCommand;
    ~SAFigureReplaceXYSeriesDataCommand();
};


///
/// \brief 序列数据QwtPoint3D的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceXYZSeriesDataCommand
        : public SAFigureReplaceSeriesDataCommand<QwtPoint3D,QwtPoint3DSeriesData>
{
public:
    using SAFigureReplaceSeriesDataCommand::SAFigureReplaceSeriesDataCommand;
};

///
/// \brief 序列数据Interval的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceIntervalSeriesDataCommand
        : public SAFigureReplaceSeriesDataCommand<QwtIntervalSample,QwtIntervalSeriesData>
{
public:
   using SAFigureReplaceSeriesDataCommand::SAFigureReplaceSeriesDataCommand;
};
///
/// \brief 序列数据QwtOHLCSample的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceOHLCSeriesDataCommand
        : public SAFigureReplaceSeriesDataCommand<QwtOHLCSample,QwtTradingChartData>
{
public:
   using SAFigureReplaceSeriesDataCommand::SAFigureReplaceSeriesDataCommand;
};
///
/// \brief 序列数据QwtPlotMultiBarChart的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceMultiBarSeriesDataCommand
        : public SAFigureReplaceSeriesDataCommand<QwtSetSample,QwtSetSeriesData>
{
public:
   using SAFigureReplaceSeriesDataCommand::SAFigureReplaceSeriesDataCommand;
};
/////////////////////////////////////////////////////////////////////
///
///  Series值按照索引替换
///
/////////////////////////////////////////////////////////////////////
template<typename T,typename TQwtSeries>
class SAFigureReplaceSeriesDataInIndexsCommand : public SAFigureOptCommand
{
public:
    SAFigureReplaceSeriesDataInIndexsCommand(SAChart2D* chart
                                         ,QwtSeriesStore<T> *curve
                                         ,const QString &cmdName
                                         ,const QVector<int>& inRangIndexs
                                         ,const QVector<T>& inRangNewData
                                         , QUndoCommand *parent = Q_NULLPTR);
    SAFigureReplaceSeriesDataInIndexsCommand(SAChart2D* chart
                                         ,QwtSeriesStore<T> *curve
                                         ,const QString &cmdName
                                         ,const QVector<int>& inRangIndexs
                                         ,const QVector<T>& inRangOldData
                                         ,const QVector<T>& inRangNewData
                                         , QUndoCommand *parent = Q_NULLPTR);

    virtual void redo();
    virtual void undo();
private:
    QVector<T> m_inRangOldData;
    QVector<int> m_inRangIndexs;
    QVector<T> m_inRangNewData;
    QwtSeriesStore<T> *m_curve;
};
template<typename T,typename TQwtSeries>
SAFigureReplaceSeriesDataInIndexsCommand<T,TQwtSeries>::SAFigureReplaceSeriesDataInIndexsCommand(
        SAChart2D *chart
        , QwtSeriesStore<T> *curve
        , const QString &cmdName
        , const QVector<int> &inRangIndexs
        , const QVector<T> &inRangNewData
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_inRangIndexs(inRangIndexs)
    ,m_inRangNewData(inRangNewData)
    ,m_curve(curve)
{
    m_inRangOldData.reserve(inRangIndexs.size());
    copyInRangeData(inRangIndexs.begin(),inRangIndexs.end(),curve,std::back_inserter(m_inRangOldData));

}
template<typename T,typename TQwtSeries>
SAFigureReplaceSeriesDataInIndexsCommand<T,TQwtSeries>::SAFigureReplaceSeriesDataInIndexsCommand(
        SAChart2D *chart
        , QwtSeriesStore<T> *curve
        , const QString &cmdName
        , const QVector<int> &inRangIndexs
        , const QVector<T> &inRangOldData
        , const QVector<T> &inRangNewData
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_inRangOldData(inRangOldData)
    ,m_inRangIndexs(inRangIndexs)
    ,m_inRangNewData(inRangNewData)
    ,m_curve(curve)
{

}
template<typename T,typename TQwtSeries>
void SAFigureReplaceSeriesDataInIndexsCommand<T,TQwtSeries>::redo()
{
    QVector<T> curveDatas;
    SAChart::getSeriesData(curveDatas,m_curve);
    const int maxSize = curveDatas.size();
    const int indexCount = m_inRangIndexs.size();
    const int inRangDataCount = m_inRangNewData.size();

    for(int i=0;i<indexCount && i<inRangDataCount;++i)
    {
        int index = m_inRangIndexs[i];
        if(index < maxSize)
        {
            curveDatas[index] = m_inRangNewData[i];
        }
    }
    m_curve->setData(new TQwtSeries(curveDatas));
}
template<typename T,typename TQwtSeries>
void SAFigureReplaceSeriesDataInIndexsCommand<T,TQwtSeries>::undo()
{
    QVector<T> curveDatas;
    SAChart::getSeriesData(curveDatas,m_curve);
    const int maxSize = curveDatas.size();
    const int indexCount = m_inRangIndexs.size();
    const int inRangDataCount = m_inRangOldData.size();

    for(int i=0;i<indexCount && i<inRangDataCount;++i)
    {
        int index = m_inRangIndexs[i];
        if(index < maxSize)
        {
            curveDatas[index] = m_inRangOldData[i];
        }
    }
    m_curve->setData(new TQwtSeries(curveDatas));
}


///
/// \brief 序列数据QPointF的按照索引替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceXYSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QPointF,QwtPointSeriesData>
{
public:
    using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};


///
/// \brief 序列数据QwtPoint3D的按照索引替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceXYZSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QwtPoint3D,QwtPoint3DSeriesData>
{
public:
    using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};


///
/// \brief 序列数据Interval的按照索引替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceIntervalSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QwtIntervalSample,QwtIntervalSeriesData>
{
public:
   using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};

///
/// \brief 序列数据QwtOHLCSample的按照索引替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceOHLCSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QwtOHLCSample,QwtTradingChartData>
{
public:
   using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};
///
/// \brief 序列数据QwtPlotMultiBarChart的按照索引替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceMultiBarSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QwtSetSample,QwtSetSeriesData>
{
public:
   using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};


/////////////////////////////////////////////////////////////////////
///
///  Series值追加
///
/////////////////////////////////////////////////////////////////////
template<typename T,typename TQwtSeries>
class SAFigureAppendSeriesDataCommand : public SAFigureReplaceSeriesDataCommand<T,TQwtSeries>
{
public:
    SAFigureAppendSeriesDataCommand(SAChart2D* chart
                                         ,QwtSeriesStore<T> *curve
                                         ,const QString &cmdName
                                         ,const QVector<T>& newData
                                         , QUndoCommand *parent = Q_NULLPTR);
};

template<typename T,typename TQwtSeries>
SAFigureAppendSeriesDataCommand<T,TQwtSeries>::SAFigureAppendSeriesDataCommand(
        SAChart2D *chart
        , QwtSeriesStore<T> *curve
        , const QString &cmdName
        , const QVector<T> &newData
        , QUndoCommand *parent)
    :SAFigureReplaceSeriesDataCommand<T,TQwtSeries>(chart,curve,cmdName,-1,newData,parent)
{

}


///
/// \brief 序列数据QPointF的追加
///
class SA_COMMON_UI_EXPORT SAFigureAppendXYSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QPointF,QwtPointSeriesData>
{
public:
    using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};



///
/// \brief 序列数据QwtPoint3D的追加
///
class SA_COMMON_UI_EXPORT SAFigureAppendXYZSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QwtPoint3D,QwtPoint3DSeriesData>
{
public:
    using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};

///
/// \brief 序列数据Interval的追加
///
class SA_COMMON_UI_EXPORT SAFigureAppendIntervalSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QwtIntervalSample,QwtIntervalSeriesData>
{
public:
   using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};
///
/// \brief 序列数据QwtOHLCSample的追加
///
class SA_COMMON_UI_EXPORT SAFigureAppendOHLCSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QwtOHLCSample,QwtTradingChartData>
{
public:
   using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};
///
/// \brief 序列数据QwtPlotMultiBarChart的追加
///
class SA_COMMON_UI_EXPORT SAFigureAppendMultiBarSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QwtSetSample,QwtSetSeriesData>
{
public:
   using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};



//==============================================
//把QwtSeriesStore的值按索引提取
template<typename T,typename IteIndex,typename IteData>
void copyInRangeData(IteIndex beginIndex,IteIndex endIndex,const QwtSeriesStore<T>* curve,IteData beginData)
{
    while(beginIndex != endIndex)
    {
        *beginData = curve->sample(*beginIndex);
        ++beginData;
        ++beginIndex;
    }
}

#endif // SAFIGUREOPTCOMMAND_H
