#include "SAValueTableOptCommands.h"

SAValueTableOptEditValueCommand::SAValueTableOptEditValueCommand(SAAbstractDatas *data
                                                                       ,SADataTableModel* model
                                                                       , const QVariantList &oldDatas
                                                                       , const QVariantList &newDatas
                                                                       ,int row
                                                                       ,int col
                                                                       , QUndoCommand *par
                                                                       )
    :SAValueTableOptBaseCommand(data,model,par)
    ,m_oldDatas(oldDatas)
    ,m_newDatas(newDatas)
    ,m_row(row)
    ,m_col(col)
{

}

void SAValueTableOptEditValueCommand::redo()
{
    SAAbstractDatas* d = data();
    SADataTableModel* m = model();
    if(SA::TableVariant == d->getType() || SA::TableDouble == d->getType())
    {
        bool isOK = d->setAt(m_newDatas[0],{m_row,m_col});
        if(isOK && (m_row>=m->dataRowCount() || m_col>=m->dataColumnCount()) )
        {
            m->update();
        }
    }
    else if(d->getDim() < SA::Dim2)
    {
        d->setAt(m_newDatas[0],{m_row});
        if(m_row==m->dataRowCount()-1)
        {
            m->update();
        }
    }
}

void SAValueTableOptEditValueCommand::undo()
{
    SAAbstractDatas* d = data();
    SADataTableModel* m = model();
    if(SA::TableVariant == d->getType() || SA::TableDouble == d->getType())
    {
        bool isOK = d->setAt(m_oldDatas[0],{m_row,m_col});
        if(isOK && (m_row>=m->dataRowCount() || m_col>=m->dataColumnCount()) )
        {
            m->update();
        }
    }
    if(d->getDim() < SA::Dim2)
    {
        d->setAt(m_oldDatas[0],{m_row});
        if(m_row==m->dataRowCount()-1)
        {
            m->update();
        }
    }
}

