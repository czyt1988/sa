#ifndef SAFIGURESTABLECOMMANDS_H
#define SAFIGURESTABLECOMMANDS_H
#include "SAFigureOptCommand.h"




/////////////////////////////////////////////////////////////////////
///
///  Series值的编辑，这个编辑是对应表格的编辑，需要提供indexRow和indexCol信息
///
/////////////////////////////////////////////////////////////////////
class SAFigureTableEditSeriesCommandPrivate;
///
/// \brief 编辑序列数值的命令在编辑的位置超过原有位置时，会把长度扩展到指定的长度，并初始化为默认的数据
///
class SA_COMMON_UI_EXPORT SAFigureTableEditSeriesCommand : public SAFigureOptCommand
{
public:
    SAFigureTableEditSeriesCommand(SAChart2D* chart
                                  , QwtPlotItem* item
                                  , double val
                                  , int indexRow
                                  , int indexCol
                                  , const QString &cmdName = QString()
                                  , QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureTableEditSeriesCommand();
    virtual void redo();
    virtual void undo();
    //判断是否有效
    bool isValid() const;
    //判断是否尺寸有变化
    bool isSizeChanged() const;
private:
    SAFigureTableEditSeriesCommandPrivate* d_ptr;
};


/////////////////////////////////////////////////////////////////////
///
///  Series值的粘贴命令，这个编辑是对应表格的编辑，需要提供indexRow和indexCol信息
///
/////////////////////////////////////////////////////////////////////
class SAFigureTablePasteInSeriesCommandPrivate;
///
/// \brief 粘贴内容到figure的命令 处理ctrl+v
///
class SA_COMMON_UI_EXPORT SAFigureTablePasteInSeriesCommand : public SAFigureOptCommand
{
public:
    SAFigureTablePasteInSeriesCommand(SAChart2D* chart
                                  , QwtPlotItem* item
                                  , const QList<QStringList>& stringClipboardTable
                                  , int indexRow = 0
                                  , int indexCol = 0
                                  , const QString &cmdName = QString()
                                  , QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureTablePasteInSeriesCommand();
    virtual void redo();
    virtual void undo();
    //判断是否有效
    bool isValid() const;
    //判断是否尺寸有变化
    bool isSizeChanged() const;
private:
    SAFigureTablePasteInSeriesCommandPrivate* d_ptr;
};


/////////////////////////////////////////////////////////////////////
///
///  Series值的删除命令，这个编辑是对应表格的编辑，需要提供indexRow和indexCol信息
///
/////////////////////////////////////////////////////////////////////
class SAFigureTableDeleteCommandPrivate;
///
/// \brief figure表格的delete 处理delete
///
class SA_COMMON_UI_EXPORT SAFigureTableDeleteCommand : public SAFigureOptCommand
{
public:
    SAFigureTableDeleteCommand(SAChart2D* chart
                                  , QwtPlotItem* item
                                  , const QVector<QPoint> &deleteIndexs
                                  , const QString &cmdName = QString()
                                  , QUndoCommand *parent = Q_NULLPTR);
    ~SAFigureTableDeleteCommand();
    virtual void redo();
    virtual void undo();
    //判断是否有效
    bool isValid() const;
private:
    SAFigureTableDeleteCommandPrivate* d_ptr;
};



#endif // SAFIGURESTABLECOMMANDS_H




