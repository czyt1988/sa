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


class SA_COMMON_UI_EXPORT SAValueTableOptPasteBaseCommand : public SAValueTableOptBaseCommand
{
public:
    SAValueTableOptPasteBaseCommand(SAAbstractDatas* data
                                     ,SADataTableModel* model
                                     ,QUndoCommand* par = Q_NULLPTR);
    virtual bool isValid() const = 0;
    static bool checkVarList(const QList<QVariantList> &varTable,int row,int col);
    template<typename T>
    static bool safeGetValue(const QVariant &v,T& val)
    {
        if(!v.canConvert<T>())
        {
            return false;
        }
        val = v.value<T>();
        return true;
    }

protected:
    virtual void init(const QList<QVariantList> &clipboardTable) = 0;

};
///
/// \brief 处理文本粘贴功能
/// 此命令需要进行isValid判断，因为有可能不符合粘贴要求
///
class SA_COMMON_UI_EXPORT SAValueTableOptPasteDoubleVectorCommand : public SAValueTableOptPasteBaseCommand
{
public:
    SAValueTableOptPasteDoubleVectorCommand(SAAbstractDatas* data
                                     , SADataTableModel* model
                                     , const QList<QVariantList>& clipboardTextTable
                                     , int startRow
                                     , QUndoCommand* par = Q_NULLPTR);
    bool isValid() const;
    void redo();
    void undo();
private:
    void init(const QList<QVariantList> &clipboardTable);
private:
    bool m_isvalid;
    //新数据的区域定位
    int m_startRow;
    int m_endRow;
    //旧数据区域定位
    int m_oldEndRow;
    int m_oldStartRow;
    //
    bool m_isOldDirty;
    QVector<double> m_oldData;
    QVector<double> m_newData;
};
///
/// \brief 处理文本粘贴功能
/// 此命令需要进行isValid判断，因为有可能不符合粘贴要求
///
class SA_COMMON_UI_EXPORT SAValueTableOptPastePointFVectorCommand : public SAValueTableOptPasteBaseCommand
{
public:
    SAValueTableOptPastePointFVectorCommand(SAAbstractDatas* data
                                     , SADataTableModel* model
                                     , const QList<QVariantList>& clipboardTextTable
                                     , const QSize& tableSize
                                     , int startRow
                                     , int startCol
                                     , QUndoCommand* par = Q_NULLPTR);
    bool isValid() const;
    void redo();
    void undo();
private:
    void init(const QList<QVariantList> &clipboardTable);
private:
    bool m_isvalid;
    //新数据的区域定位
    int m_startRow;
    int m_endRow;
    int m_startCol;
    int m_endCol;
    //旧数据区域定位
    int m_oldEndRow;
    int m_oldStartRow;
    //
    bool m_isOldDirty;
    QVector<QPointF> m_oldData;
    QVector<QPointF> m_newData;
};

//TODO其他的粘贴操作以及返回选中的范围功能的添加

#endif // SAVALUETABLEOPTCOMMANDS_H
