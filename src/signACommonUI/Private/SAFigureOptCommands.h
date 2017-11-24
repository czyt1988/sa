#ifndef SAFIGUREOPTCOMMANDS_H
#define SAFIGUREOPTCOMMANDS_H
#include "SAFigureOptCommand.h"
#include <QList>
#include <QVector>
#include <QSharedPointer>
#include "qwt_plot_item.h"
#include <memory>
class SAChart2D;
class SAAbstractDatas;
class QwtPlotCurve;


///
/// \brief 绘图元素的添加
///
class SAFigureChartItemAddCommand : public SAFigureOptCommand
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
class SAFigureChartItemListAddCommand : public SAFigureOptCommand
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
class SAFigureChartItemDeleteCommand : public SAFigureOptCommand
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
class SAFigureChartSelectionRegionAddCommand : public SAFigureOptCommand
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
/// \brief 移除曲线范围内的数据
///
class SAFigureRemoveCurveDataInRangCommand : public SAFigureOptCommand
{
public:
    SAFigureRemoveCurveDataInRangCommand(SAChart2D* chart,const QList<QwtPlotCurve *>& curves,const QString &cmdName, QUndoCommand *parent = Q_NULLPTR);
    virtual void redo();
    virtual void undo();
private:
    void recordPlotCureData(QList<QSharedPointer<QVector<QPointF> > >& recorder);
    void recover();
private:
    int m_redoCount;
    QList<QwtPlotCurve*> m_curveList;
    QList<QSharedPointer<QVector<QPointF> > > m_backupData;///< 保存曲线原来的数据
};
///
/// \brief 适用于SAFigureMoveCurveDataInIndexsCommand的数据结构
///
class SAFigureMoveCurveDataInIndexsCommandCurveInfo
{
public:
    QwtPlotCurve* curve;
    QVector<int> inRangIndexs;
    QVector<QPointF> inRangOriginData;
    QVector<QPointF> inRangNewData;
};

///
/// \brief 移动某些序号的数据
///
class SAFigureMoveCurveDataInIndexsCommand : public SAFigureOptCommand
{
public:
    SAFigureMoveCurveDataInIndexsCommand(SAChart2D* chart
                                         ,const QList<SAFigureMoveCurveDataInIndexsCommandCurveInfo >& curveInfoList
                                         ,const QString &cmdName
                                         , QUndoCommand *parent = Q_NULLPTR);
    virtual void redo();
    virtual void undo();
private:
    QList<SAFigureMoveCurveDataInIndexsCommandCurveInfo > m_curveInfoList;
};

#endif // SAFIGUREOPTCOMMAND_H
