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


///////////////////////////////////////////////////////////////////////
///
/// \brief figure窗体中添加子窗体
///
class SA_COMMON_UI_EXPORT SAFigureCreateSubWidgetCommand : public SAFigureOptCommand
{
public:
    SAFigureCreateSubWidgetCommand(SAFigureWindow* fig,QWidget* w,float xPresent, float yPresent, float wPresent, float hPresent,const QString &cmdName,QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureCreateSubWidgetCommand();
    virtual void redo();
    virtual void undo();
private:
    QWidget* m_widget;
    bool m_isNeedToDelete;///< 记录是否是此命令对它进行隐藏
};

///////////////////////////////////////////////////////////////////////
///
/// \brief 子窗口位置变更
///
class SA_COMMON_UI_EXPORT SAFigureSubChartResizeCommand : public SAFigureOptCommand
{
public:
    SAFigureSubChartResizeCommand(SAFigureWindow* fig,QWidget* w,const QRect& size,const QString &cmdName,QUndoCommand *parent = Q_NULLPTR);
    SAFigureSubChartResizeCommand(SAFigureWindow* fig,QWidget* w,const QRect& oldSize,const QRect& newSize,const QString &cmdName,QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureSubChartResizeCommand();
    virtual void redo();
    virtual void undo();
private:
    QWidget* m_widget;
    QRect m_oldSize;
    QRect m_newSize;
};

///////////////////////////////////////////////////////////////////////
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
    QwtPlotItem* m_item;
};
///////////////////////////////////////////////////////////////////////
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
    QList<QwtPlotItem*> m_itemList;
};
///////////////////////////////////////////////////////////////////////
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
    QwtPlotItem* m_item;
};
///////////////////////////////////////////////////////////////////////
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
