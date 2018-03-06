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
    SAAbstractDatas* data() const
    {
        return m_data;
    }
    SADataTableModel* model() const
    {
        return m_model;
    }
private:
    SAAbstractDatas* m_data;
    SADataTableModel* m_model;
};

///
/// \brief 数值表格编辑命令
///
class SA_COMMON_UI_EXPORT SAValueTableOptEditValueCommand : public SAValueTableOptBaseCommand
{
public:
    SAValueTableOptEditValueCommand(SAAbstractDatas* data
                                       ,SADataTableModel* model
                                       ,const QVariantList& oldDatas
                                       ,const QVariantList& newDatas
                                       ,int row
                                       ,int col
                                       ,QUndoCommand* par = Q_NULLPTR);
    void redo();
    void undo();
private:
    QVariantList m_oldDatas;
    QVariantList m_newDatas;
    int m_row;
    int m_col;
};

#endif // SAVALUETABLEOPTCOMMANDS_H
