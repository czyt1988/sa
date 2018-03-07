#ifndef SAVALUETABLEOPTCOMMANDS_H
#define SAVALUETABLEOPTCOMMANDS_H
#include "SACommonUIGlobal.h"
#include <QUndoCommand>
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
private:
    QVariantList m_newDatas;
    int m_modelRow;
    int m_modelCol;
    int m_realRow;
    int m_startCol;
    int m_endCol;
};

#endif // SAVALUETABLEOPTCOMMANDS_H
