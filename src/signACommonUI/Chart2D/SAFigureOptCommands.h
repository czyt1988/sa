#ifndef SAFIGUREOPTCOMMANDS_H
#define SAFIGUREOPTCOMMANDS_H
#include "SAFigureOptCommand.h"
#include <QList>
#include <QVector>
#include <QSharedPointer>
#include "qwt_plot_item.h"
#include "qwt_series_store.h"
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
class SAFigureMoveSeriseDataInfoBase
{
public:
    SAFigureMoveSeriseDataInfoBase(QwtPlotItem* it = nullptr):plotItem(it)
    {

    }
    virtual ~SAFigureMoveSeriseDataInfoBase()
    {

    }
    virtual int rtti() const = 0;
    enum RTTI
    {
        RTTI_PointInfo
    };
    const QVector<int>& indexs() const{return inRangIndexs;}
    QVector<int>& indexs() {return inRangIndexs;}
    void setIndexs(const QVector<int>& v){inRangIndexs = v;}

    QwtPlotItem* item(){return plotItem;}
    void setItem(QwtPlotItem* v){plotItem  =v;}
protected:
    QwtPlotItem* plotItem;
    QVector<int> inRangIndexs;
};

class SAFigureMoveSerisePointDataInfo : public SAFigureMoveSeriseDataInfoBase
{
public:
    SAFigureMoveSerisePointDataInfo(QwtPlotItem* it = nullptr):SAFigureMoveSeriseDataInfoBase(it)
    {

    }
    ~SAFigureMoveSerisePointDataInfo(){

    }
    virtual int rtti() const{return RTTI_PointInfo;}
    void setOriginPoints(const QVector<QPointF>& p){
        inRangOriginData = p;
    }
    const QVector<QPointF>& originPoints() const{
        return inRangOriginData;
    }
    QVector<QPointF>& originPoints(){
        return inRangOriginData;
    }

    void setNewPoints(const QVector<QPointF>& p){
        inRangNewData = p;
    }
    const QVector<QPointF>& newPoints() const{
        return inRangNewData;
    }
    QVector<QPointF>& newPoints(){
        return inRangNewData;
    }
protected:
    QVector<QPointF> inRangOriginData;
    QVector<QPointF> inRangNewData;
};

///
/// \brief 移动某些序号的数据
///
class SAFigureMovePointDataInIndexsCommand : public SAFigureOptCommand
{
public:
    SAFigureMovePointDataInIndexsCommand(SAChart2D* chart
                                         ,SAFigureMoveSeriseDataInfoBase* baseInfo
                                         ,const QString &cmdName
                                         , QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureMovePointDataInIndexsCommand();
    virtual void redo();
    virtual void undo();
private:
    SAFigureMoveSeriseDataInfoBase* m_baseInfo;
};

#endif // SAFIGUREOPTCOMMAND_H
