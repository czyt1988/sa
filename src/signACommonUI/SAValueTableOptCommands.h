#ifndef SAVALUETABLEOPTCOMMANDS_H
#define SAVALUETABLEOPTCOMMANDS_H
#include "SACommonUIGlobal.h"
#include <QUndoCommand>
#include <QItemSelectionModel>
#include "SADataTableModel.h"
#include "SAAbstractDatas.h"
///
/// \brief sa 数值表格操作类命令基类
///
class SA_COMMON_UI_EXPORT SAValueTableOptBaseCommand : public QUndoCommand
{
public:
    SAValueTableOptBaseCommand(SAAbstractDatas* data,SADataTableModel* model,QUndoCommand* par = Q_NULLPTR)
        :m_data(data)
        ,m_model(model)
    {
    }
    SAAbstractDatas* getDataPtr() const
    {
        return m_data;
    }
    SADataTableModel* getModel() const
    {
        return m_model;
    }
private:
    SAAbstractDatas* m_data;
    SADataTableModel* m_model;
};
///
/// \brief 数值编辑命令
///
class SA_COMMON_UI_EXPORT SAValueTableOptEditValueCommand : public SAValueTableOptBaseCommand
{
public:
    SAValueTableOptEditValueCommand(SAAbstractDatas* data
                                      , SADataTableModel* model
                                       , const QVariant& newData
                                       , int row
                                       , int col
                                       , QUndoCommand* par = Q_NULLPTR);
    void redo();
    void undo();
private:
    QVariant m_oldDatas;
    QVariant m_newDatas;
    int m_modelRow;
    int m_modelCol;
    int m_realRow;
    int m_realCol;
};

///
/// \brief 数值追加命令
///
class SA_COMMON_UI_EXPORT SAValueTableOptAppendValueCommand : public SAValueTableOptBaseCommand
{
public:
    SAValueTableOptAppendValueCommand(SAAbstractDatas* data
                                       ,SADataTableModel* model
                                       ,const QVariantList& newDatas
                                       ,int row
                                       ,int col
                                       ,QUndoCommand* par = Q_NULLPTR);
    void redo();
    void undo();
private:
    void appendInVectorPoint();
    void popBackVectorPoint();
    void appendInVectorVariant();
    void popBackVectorVariant();
    void appendInTable();
    void popFromTable();
private:
    QVariantList m_newDatas;
    int m_modelRow;
    int m_modelCol;
    int m_realRow;
    int m_startCol;
    int m_endCol;
};


///
/// \brief 处理文本粘贴功能
/// 此命令需要进行isValid判断，因为有可能不符合粘贴要求
///
class SA_COMMON_UI_EXPORT SAValueTableOptPasteValueCommand : public SAValueTableOptBaseCommand
{
public:
    SAValueTableOptPasteValueCommand(SAAbstractDatas* data
                                     ,SADataTableModel* model
                                     ,const QList<QVariantList>& clipboardTextTable
                                     , const QSize& tableSize
                                     , int startCol
                                     , int endCol
                                     , int startRow
                                     , int endRow
                                     ,QUndoCommand* par = Q_NULLPTR);
    void redo();
    void undo();
    bool isValid() const;
private:
    void initVectorDouble(const QList<QVariantList> &clipboardTable, const QSize &tableSize);
    static bool checkVarList(const QList<QVariantList> &varTable,int row,int col);
private:
    bool m_isvalid;
    //新数据的区域定位
    int m_startCol;
    int m_endCol;
    int m_startRow;
    int m_endRow;
    //旧数据区域定位
    int m_oldEndRow;
    int m_oldStartRow;
    int m_oldEndCol;
    int m_oldStartCol;
    QList<QVariantList>& m_oldData;
    QList<QVariantList>& m_newData;
};

#endif // SAVALUETABLEOPTCOMMANDS_H
