#ifndef SAFIGUREOPTCOMMANDS_H
#define SAFIGUREOPTCOMMANDS_H
#include "SAFigureOptCommand.h"
#include <QList>
#include <QVector>
#include <QSharedPointer>
#include "qwt_plot_item.h"
#include "qwt_series_store.h"
#include "qwt_series_store.h"
#include "SAChart.h"
#include <memory>

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
    SAFigureChartItemDeleteCommand(SAChart2D* chart, QwtPlotItem *ser, const QString &cmdName, QUndoCommand *parent = Q_NULLPTR);
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
///  Series值变更
///
/////////////////////////////////////////////////////////////////////


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
///  Series值替换
///
/////////////////////////////////////////////////////////////////////
template<typename T,typename TQwtSeries>
class SA_COMMON_UI_EXPORT SAFigureReplaceSeriesDataInIndexsCommand : public SAFigureOptCommand
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
/// \brief 序列数据QPointF的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceXYSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QPointF,QwtPointSeriesData>
{
public:
    using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};


///
/// \brief 序列数据QwtPoint3D的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceXYZSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QwtPoint3D,QwtPoint3DSeriesData>
{
public:
    using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};

///
/// \brief 序列数据Interval的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceIntervalSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QwtIntervalSample,QwtIntervalSeriesData>
{
public:
   using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};
///
/// \brief 序列数据QwtOHLCSample的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceOHLCSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QwtOHLCSample,QwtTradingChartData>
{
public:
   using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};
///
/// \brief 序列数据QwtPlotMultiBarChart的替换
///
class SA_COMMON_UI_EXPORT SAFigureReplaceMultiBarSeriesDataInIndexsCommand
        : public SAFigureReplaceSeriesDataInIndexsCommand<QwtSetSample,QwtSetSeriesData>
{
public:
   using SAFigureReplaceSeriesDataInIndexsCommand::SAFigureReplaceSeriesDataInIndexsCommand;
};


/////////////////////////////////////////////////////////////////////
///
///  Series值插入
///
/////////////////////////////////////////////////////////////////////
template<typename T,typename TQwtSeries>
class SA_COMMON_UI_EXPORT SAFigureAppendSeriesDataCommand : public SAFigureOptCommand
{
public:
    SAFigureAppendSeriesDataCommand(SAChart2D* chart
                                         ,QwtSeriesStore<T> *curve
                                         ,const QString &cmdName
                                         ,const QVector<T>& newData
                                         , QUndoCommand *parent = Q_NULLPTR);

    virtual void redo();
    virtual void undo();
private:
    QVector<T> m_newData;
    QwtSeriesStore<T> *m_curve;
};

template<typename T,typename TQwtSeries>
SAFigureAppendSeriesDataCommand<T,TQwtSeries>::SAFigureAppendSeriesDataCommand(
        SAChart2D *chart
        , QwtSeriesStore<T> *curve
        , const QString &cmdName
        , const QVector<T> &newData
        , QUndoCommand *parent)
    :SAFigureOptCommand(chart,cmdName,parent)
    ,m_newData(newData)
    ,m_curve(curve)
{

}

template<typename T,typename TQwtSeries>
void SAFigureAppendSeriesDataCommand<T,TQwtSeries>::redo()
{
    QVector<T> curveDatas;
    SAChart::getSeriesData(curveDatas,m_curve);
    curveDatas.append(m_newData);
    m_curve->setData(new TQwtSeries(curveDatas));
}

template<typename T,typename TQwtSeries>
void SAFigureAppendSeriesDataCommand<T,TQwtSeries>::undo()
{
    QVector<T> curveDatas;
    SAChart::getSeriesData(curveDatas,m_curve);
    curveDatas.resize(curveDatas.size() - m_newData.size());
    m_curve->setData(new TQwtSeries(curveDatas));
}


///
/// \brief 序列数据QPointF的替换
///
class SA_COMMON_UI_EXPORT SAFigureAppendXYSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QPointF,QwtPointSeriesData>
{
public:
    using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};


///
/// \brief 序列数据QwtPoint3D的替换
///
class SA_COMMON_UI_EXPORT SAFigureAppendXYZSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QwtPoint3D,QwtPoint3DSeriesData>
{
public:
    using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};

///
/// \brief 序列数据Interval的替换
///
class SA_COMMON_UI_EXPORT SAFigureAppendIntervalSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QwtIntervalSample,QwtIntervalSeriesData>
{
public:
   using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};
///
/// \brief 序列数据QwtOHLCSample的替换
///
class SA_COMMON_UI_EXPORT SAFigureAppendOHLCSeriesDataCommand
        : public SAFigureAppendSeriesDataCommand<QwtOHLCSample,QwtTradingChartData>
{
public:
   using SAFigureAppendSeriesDataCommand::SAFigureAppendSeriesDataCommand;
};
///
/// \brief 序列数据QwtPlotMultiBarChart的替换
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
