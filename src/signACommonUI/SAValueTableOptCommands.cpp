#include "SAValueTableOptCommands.h"
#include "SAData.h"

SAValueTableOptEditValueCommand::SAValueTableOptEditValueCommand(SAAbstractDatas *data
                                                                 , SADataTableModel *model
                                                                 , const QVariant &newData
                                                                 , int row
                                                                 , int col
                                                                 , QUndoCommand *par)
    :SAValueTableOptBaseCommand(data,model,par)
    ,m_newDatas(newData)
    ,m_modelRow(row)
    ,m_modelCol(col)
    ,m_realRow(row)
{
    SAAbstractDatas* d = getDataPtr();
    SADataTableModel* m = getModel();

    if(d->getDim() < SA::Dim2)
    {
        //对于小于2维的直接设置
        m_oldDatas = d->getAt(row);
        m_realCol = 0;
    }
    else
    {
        int colStart,colEnd;
        model->dataColumnRange(d,colStart,colEnd);
        m_realCol = col - colStart;
        m_oldDatas = d->getAt(m_realRow,m_realCol);
    }
}

void SAValueTableOptEditValueCommand::redo()
{
    SAAbstractDatas* d = getDataPtr();
    SADataTableModel* m = getModel();
    d->setAt(m_newDatas,{m_realRow,m_realCol});
    m->update();
}

void SAValueTableOptEditValueCommand::undo()
{
    SAAbstractDatas* d = getDataPtr();
    SADataTableModel* m = getModel();
    d->setAt(m_oldDatas,{m_realRow,m_realCol});
    m->update();
}


SAValueTableOptAppendValueCommand::SAValueTableOptAppendValueCommand(SAAbstractDatas *data
                                                                       ,SADataTableModel* model
                                                                       , const QVariantList &newDatas
                                                                       ,int row
                                                                       ,int col
                                                                       , QUndoCommand *par
                                                                       )
    :SAValueTableOptBaseCommand(data,model,par)
    ,m_newDatas(newDatas)
    ,m_modelRow(row)
    ,m_modelCol(col)
    ,m_realRow(row)
{

}

void SAValueTableOptAppendValueCommand::redo()
{
    SAAbstractDatas* d = getDataPtr();
    SADataTableModel* m = getModel();
    switch(d->getType())
    {
    case SA::VectorPoint:
        appendInVectorPoint();
        break;
    case SA::VectorDouble:
    case SA::VectorInt:
    case SA::VectorVariant:
        appendInVectorVariant();
        break;
    case SA::TableDouble:
    case SA::TableVariant:
        appendInTable();
        break;
    default:
        break;
    }
    m->update();
}

void SAValueTableOptAppendValueCommand::undo()
{
    SAAbstractDatas* d = getDataPtr();
    SADataTableModel* m = getModel();
    switch(d->getType())
    {
    case SA::VectorPoint:
        popBackVectorPoint();
        break;
    case SA::VectorDouble:
    case SA::VectorInt:
    case SA::VectorVariant:
        popBackVectorVariant();
        break;
    case SA::TableDouble:
    case SA::TableVariant:
        popFromTable();
        break;
    default:
        break;
    }
    m->update();
}

void SAValueTableOptAppendValueCommand::appendInVectorPoint()
{
    QPointF f;
    if(2 != m_newDatas.size())
    {
        return;
    }
    f.setX(m_newDatas[0].toDouble());
    f.setY(m_newDatas[1].toDouble());
    SAAbstractDatas* d = getDataPtr();
    SAVectorPointF* vf = static_cast<SAVectorPointF*>(d);
    vf->append(f);
}

void SAValueTableOptAppendValueCommand::popBackVectorPoint()
{
    SAAbstractDatas* d = getDataPtr();
    SAVectorPointF* vf = static_cast<SAVectorPointF*>(d);
    vf->getValueDatas().pop_back();
}

void SAValueTableOptAppendValueCommand::appendInVectorVariant()
{
    if(1 != m_newDatas.size())
    {
        return;
    }
    SAAbstractDatas* d = getDataPtr();
    switch(d->getType())
    {
    case SA::VectorDouble:
    {
        bool isOK = false;
        double v = m_newDatas[0].toDouble(&isOK);
        if(isOK)
            static_cast<SAVectorDouble*>(d)->append(v);
        break;
    }
    case SA::VectorInt:
    {
        bool isOK = false;
        int v = m_newDatas[0].toInt(&isOK);
        if(isOK)
        static_cast<SAVectorInt*>(d)->append(v);
        break;
    }
    case SA::VectorVariant:
    {
        static_cast<SAVectorVariant*>(d)->append(m_newDatas[0]);
        break;
    }
    default:
        break;
    }
}

void SAValueTableOptAppendValueCommand::popBackVectorVariant()
{
    SAAbstractDatas* d = getDataPtr();
    switch(d->getType())
    {
    case SA::VectorDouble:
    {
        bool isOK = false;
        double v = m_newDatas[0].toDouble(&isOK);
        if(isOK)
            static_cast<SAVectorDouble*>(d)->append(v);
        break;
    }
    case SA::VectorInt:
    {
        bool isOK = false;
        int v = m_newDatas[0].toInt(&isOK);
        if(isOK)
        static_cast<SAVectorInt*>(d)->append(v);
        break;
    }
    case SA::VectorVariant:
    {
        static_cast<SAVectorVariant*>(d)->append(m_newDatas[0]);
        break;
    }
    default:
        break;
    }
}

void SAValueTableOptAppendValueCommand::appendInTable()
{
    if(1 != m_newDatas.size())
    {
        return;
    }
    SAAbstractDatas* d = getDataPtr();
    SADataTableModel* m = getModel();
    int colStart,colEnd;
    m->dataColumnRange(d,colStart,colEnd);
    int realCol = m_modelCol - colStart;
    switch(d->getType())
    {
    case SA::TableDouble:
    {
        bool isOK = false;
        double v = m_newDatas[0].toDouble(&isOK);
        static_cast<SATableDouble*>(d)->setTableData(m_realRow ,realCol,v);
        break;
    }
    case SA::TableVariant:
    {
        static_cast<SATableVariant*>(d)->setTableData(m_realRow ,realCol,m_newDatas[0]);
        break;
    }
    default:
        break;
    }
}

void SAValueTableOptAppendValueCommand::popFromTable()
{
    SAAbstractDatas* d = getDataPtr();
    SADataTableModel* m = getModel();
    int colStart,colEnd;
    m->dataColumnRange(d,colStart,colEnd);
    int realCol = m_modelCol - colStart;
    switch(d->getType())
    {
    case SA::TableDouble:
    {
        static_cast<SATableDouble*>(d)->removeTableData(m_realRow,realCol);
        break;
    }
    case SA::TableVariant:
    {
        static_cast<SATableVariant*>(d)->removeTableData(m_realRow ,realCol);
        break;
    }
    default:
        break;
    }
}


