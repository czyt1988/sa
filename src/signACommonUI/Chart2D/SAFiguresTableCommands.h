#ifndef SAFIGURESTABLECOMMANDS_H
#define SAFIGURESTABLECOMMANDS_H
#include "SAFigureOptCommand.h"




/////////////////////////////////////////////////////////////////////
///
///  Series值的编辑，这个编辑是对应表格的编辑，需要提供indexRow和indexCol信息
///
/////////////////////////////////////////////////////////////////////
class SAFiguresTableEditSeriesCommandPrivate;
///
/// \brief 编辑序列数值的命令在编辑的位置超过原有位置时，会把长度扩展到指定的长度，并初始化为默认的数据
///
class SA_COMMON_UI_EXPORT SAFiguresTableEditSeriesCommand : public SAFigureOptCommand
{
public:
    SAFiguresTableEditSeriesCommand(SAChart2D* chart
                                  , QwtPlotItem* item
                                  , double val
                                  , int indexRow
                                  , int indexCol
                                  , const QString &cmdName = QString()
                                  , QUndoCommand *parent = Q_NULLPTR);
    ~SAFiguresTableEditSeriesCommand();
    virtual void redo();
    virtual void undo();
    //判断是否有效
    bool isValid() const;
    //判断是否尺寸有变化
    bool isSizeChanged() const;
private:
    SAFiguresTableEditSeriesCommandPrivate* d_ptr;
};


/////////////////////////////////////////////////////////////////////
///
///  Series值的粘贴命令，这个编辑是对应表格的编辑，需要提供indexRow和indexCol信息
///
/////////////////////////////////////////////////////////////////////
class SAFiguresTablePasteInSeriesCommandPrivate;
///
/// \brief 粘贴内容到figure的命令 处理ctrl+v
///
class SA_COMMON_UI_EXPORT SAFiguresTablePasteInSeriesCommand : public SAFigureOptCommand
{
public:
    SAFiguresTablePasteInSeriesCommand(SAChart2D* chart
                                  , QwtPlotItem* item
                                  , const QList<QStringList>& stringClipboardTable
                                  , int indexRow = 0
                                  , int indexCol = 0
                                  , const QString &cmdName = QString()
                                  , QUndoCommand *parent = Q_NULLPTR);
    ~SAFiguresTablePasteInSeriesCommand();
    virtual void redo();
    virtual void undo();
    //判断是否有效
    bool isValid() const;
    //判断是否尺寸有变化
    bool isSizeChanged() const;
private:
    SAFiguresTablePasteInSeriesCommandPrivate* d_ptr;
};







#endif // SAFIGURESTABLECOMMANDS_H



