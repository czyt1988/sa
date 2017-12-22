#ifndef SAFIGUREOPTCOMMANDS_H
#define SAFIGUREOPTCOMMANDS_H
#include "SAFigureOptCommand.h"
#include <QList>
#include <QVector>
#include <QSharedPointer>
#include "qwt_plot_item.h"
#include "qwt_series_store.h"
#include "qwt_series_store.h"
#include <memory>
class SAChart2D;
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
///
/// \brief 曲线QwtPlotCurve值变更
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
/// \brief 移动某些序列的数据
///
class SA_COMMON_UI_EXPORT SAFigureMoveXYSeriesDataInIndexsCommand : public SAFigureOptCommand
{
public:
    SAFigureMoveXYSeriesDataInIndexsCommand(SAChart2D* chart
                                         ,QwtSeriesStore<QPointF> *curve
                                         ,const QString &cmdName
                                         ,const QVector<int>& inRangIndexs
                                         ,const QVector<QPointF>& inRangNewData
                                         , QUndoCommand *parent = Q_NULLPTR);
    SAFigureMoveXYSeriesDataInIndexsCommand(SAChart2D* chart
                                         ,QwtSeriesStore<QPointF> *curve
                                         ,const QString &cmdName
                                         ,const QVector<int>& inRangIndexs
                                         ,const QVector<QPointF>& inRangOldData
                                         ,const QVector<QPointF>& inRangNewData
                                         , QUndoCommand *parent = Q_NULLPTR);

    virtual void redo();
    virtual void undo();
private:
    QVector<QPointF> m_inRangOldData;
    QVector<int> m_inRangIndexs;
    QVector<QPointF> m_inRangNewData;
    QwtSeriesStore<QPointF> *m_curve;
};
#endif // SAFIGUREOPTCOMMAND_H
