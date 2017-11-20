#ifndef SAFIGUREOPTCOMMAND_H
#define SAFIGUREOPTCOMMAND_H
#include <QUndoCommand>
#include <QList>
#include <QVector>
#include <QSharedPointer>
#include "qwt_plot_item.h"
class SAChart2D;
class SAAbstractDatas;
class QwtPlotCurve;

class SAFigureOptCommand : public QUndoCommand
{
public:
    SAFigureOptCommand(SAChart2D* chart,const QString &cmdName);
    SAChart2D* plot();
protected:
    SAChart2D* m_chart;
};

///
/// \brief 绘图元素的添加
///
class SAFigureChartItemAddCommand : public SAFigureOptCommand
{
public:
    SAFigureChartItemAddCommand(SAChart2D* chart,QwtPlotItem *ser,const QString &cmdName);
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
    SAFigureChartItemListAddCommand(SAChart2D* chart,const QList<QwtPlotItem*>& itemList,const QString &cmdName);
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
    SAFigureChartItemDeleteCommand(SAChart2D* chart,QwtPlotItem *ser,const QString &cmdName);
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
    SAFigureChartSelectionRegionAddCommand(SAChart2D* chart,const QPainterPath& newRegion,const QString &cmdName);
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
    SAFigureRemoveCurveDataInRangCommand(SAChart2D* chart,const QList<QwtPlotCurve *>& curves,const QString &cmdName);
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


#endif // SAFIGUREOPTCOMMAND_H
