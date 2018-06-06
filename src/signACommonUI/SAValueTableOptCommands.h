#ifndef SAVALUETABLEOPTCOMMANDS_H
#define SAVALUETABLEOPTCOMMANDS_H
#include "SACommonUIGlobal.h"
#include <QUndoCommand>
#include <QItemSelectionModel>
#include <QVariant>
#include <QList>
class SADataTableModel;
class SAAbstractDatas;
class SAVectorDouble;
class SAValueTableOptEditValueCommandPrivateBase;
class SAValueTableOptPasteCommandPrivateBase;
class SAValueTableOptDeleteCommandPrivateBase;
class SAValueTableOptInsertCommandPrivateBase;
///
/// \brief sa 数值表格操作类命令基类
///
class SA_COMMON_UI_EXPORT SAAbstractValueTableOptCommand : public QUndoCommand
{
public:
    SAAbstractValueTableOptCommand(SAAbstractDatas* data,QUndoCommand* par = Q_NULLPTR)
        :QUndoCommand(par)
        ,m_data(data)
    {
    }

    SAAbstractDatas* getDataPtr() const
    {
        return m_data;
    }
private:
    SAAbstractDatas* m_data;
};



///
/// \brief 数值编辑命令
///
/// 数据编辑可以实现数据的修改，以及数据的添加，使用\sa isValid 进行判断
///
class SA_COMMON_UI_EXPORT SAValueTableOptEditValueCommand : public SAAbstractValueTableOptCommand
{
public:
    SAValueTableOptEditValueCommand(SAAbstractDatas* data
                                      , const QVariant& newData
                                       , int row
                                       , int col
                                       , QUndoCommand* par = Q_NULLPTR);
    SAValueTableOptEditValueCommand(SAAbstractDatas* data
                                      , const QVariantList& newDatas
                                       , int row
                                       , int col
                                       , QUndoCommand* par = Q_NULLPTR);
    ~SAValueTableOptEditValueCommand();
    void redo();
    void undo();
    bool isValid() const;
private:
    SAValueTableOptEditValueCommandPrivateBase* d_ptr;
};



///
/// \brief 处理数据表格的复制&粘贴命令
///
/// 并非所有数据都能支持复制粘贴，因此需要使用\sa isValid 进行判断
///
class SA_COMMON_UI_EXPORT SAValueTableOptPasteCommand : public SAAbstractValueTableOptCommand
{
public:
    SAValueTableOptPasteCommand(SAAbstractDatas* data
                                     , const QList<QVariantList>& clipboardTextTable
                                     , int startRow
                                     , int startCol
                                     ,QUndoCommand* par = Q_NULLPTR);
    ~SAValueTableOptPasteCommand();
    bool isValid() const;
    void redo();
    void undo();

private:
    SAValueTableOptPasteCommandPrivateBase* d_ptr;
};

///
/// \brief 处理数据表格删除命令
///
class SA_COMMON_UI_EXPORT SAValueTableOptDeleteCommand : public SAAbstractValueTableOptCommand
{
public:
    SAValueTableOptDeleteCommand(SAAbstractDatas* data
                                     , const QVector<QPoint>& deleteIndexs
                                     ,QUndoCommand* par = Q_NULLPTR);
    ~SAValueTableOptDeleteCommand();
    bool isValid() const;
    void redo();
    void undo();

private:
    SAValueTableOptDeleteCommandPrivateBase* d_ptr;
};

///
/// \brief 数据表格的插入命令
///
class SA_COMMON_UI_EXPORT SAValueTableOptInsertCommand : public SAAbstractValueTableOptCommand
{
public:
    SAValueTableOptInsertCommand(SAAbstractDatas* data
                                     , const QVector<QPoint>& selectIndex
                                     ,QUndoCommand* par = Q_NULLPTR);
    ~SAValueTableOptInsertCommand();
    bool isValid() const;
    void redo();
    void undo();

private:
    SAValueTableOptInsertCommandPrivateBase* d_ptr;
};
#endif // SAVALUETABLEOPTCOMMANDS_H
