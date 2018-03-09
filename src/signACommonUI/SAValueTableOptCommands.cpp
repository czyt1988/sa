#include "SAValueTableOptCommands.h"
#include "SAData.h"
#include "SADataTableModel.h"
#include "SAUIHelper.h"
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


SAValueTableOptAppendValueCommand::SAValueTableOptAppendValueCommand(
        SAAbstractDatas *data
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


//===============================================



SAValueTableOptPasteBaseCommand::SAValueTableOptPasteBaseCommand(SAAbstractDatas *data, SADataTableModel *model, QUndoCommand *par)
:SAValueTableOptBaseCommand(data,model,par)
{

}

bool SAValueTableOptPasteBaseCommand::checkVarList(const QList<QVariantList> &varTable, int row, int col)
{
    if(row < varTable.size())
    {
        if(col < varTable[row].size())
        {
            return true;
        }
    }
    return false;
}

//===============================================


SAValueTableOptPasteDoubleVectorCommand::SAValueTableOptPasteDoubleVectorCommand(SAAbstractDatas *data
        , SADataTableModel *model
        , const QList<QVariantList>& clipboardTextTable
        , int startRow
        , QUndoCommand *par
    )
    :SAValueTableOptPasteBaseCommand(data,model,par)
    ,m_isvalid(false)
    ,m_startRow(startRow)
    ,m_endRow(-1)
    ,m_oldEndRow(-1)
    ,m_oldStartRow(-1)
{
    if(data->getType() == SA::VectorDouble)
    {
        init(clipboardTextTable);
    }
}

bool SAValueTableOptPasteDoubleVectorCommand::isValid() const
{
    return m_isvalid;
}




void SAValueTableOptPasteDoubleVectorCommand::init(const QList<QVariantList>& clipboardTable)
{
    const int clipSize = clipboardTable.size();
    m_newData.resize(clipSize);
    for(int i=0;i<clipSize;++i)
    {
        if(!SAValueTableOptPasteBaseCommand::checkVarList(clipboardTable,i,0))
            return;
        QVariant v = clipboardTable[i][0];
        bool isOK = false;
        double d = v.toDouble(&isOK);
        if(!isOK)
            d = 0;
        m_newData[i] = d;
    }
    //记录行变换
    m_endRow = m_startRow + m_newData.size();
    //提取需要复制的数据
    SAVectorDouble *data = static_cast<SAVectorDouble*>(getDataPtr());
    const int rawRowCount = data->getValueDatas().size();
    m_oldStartRow = (m_startRow < rawRowCount) ? m_startRow : -1;
    m_oldEndRow = (m_endRow < rawRowCount)? m_endRow : rawRowCount-1;

    //提取原有数据
    if(m_oldStartRow >= 0)
    {
        m_oldData.reserve(m_oldEndRow-m_oldStartRow+1);
        for(int i=m_oldStartRow;i<m_oldEndRow;++i)
        {
            m_oldData.append(data->get(i));
        }
    }
    //完成处理
    m_isvalid = true;
}



void SAValueTableOptPasteDoubleVectorCommand::redo()
{
    if(!m_isvalid)
        return;
    SAVectorDouble *data = static_cast<SAVectorDouble*>(getDataPtr());
    QVector<double>& rawBuffer = data->getValueDatas();
    const int oldDataSize = rawBuffer.size();
    //1 替换数据
    if(m_endRow < oldDataSize)
    {
        //不会扩展
        for(int r=m_startRow;r<m_endRow;++r)
        {
            rawBuffer[r] = m_newData[r - m_startRow];
        }
    }
    else
    {
        //会扩展
        const int newDataSize = m_newData.size();
        int i= 0;
        for(i = 0;i<newDataSize && (m_startRow+i) < oldDataSize;++i)
        {
            rawBuffer[m_startRow+i] = m_newData[i];
        }
        //剩下插入
        for(;i < newDataSize;++i)
        {
            rawBuffer.append(m_newData[i]);
        }
    }
    m_isOldDirty = data->isDirty();
    data->setDirty(true);
}




void SAValueTableOptPasteDoubleVectorCommand::undo()
{
    if(!m_isvalid)
        return;
    SAVectorDouble *data = static_cast<SAVectorDouble*>(getDataPtr());
    QVector<double>& rawBuffer = data->getValueDatas();
    //判断是否要删除
    if(m_oldEndRow < m_endRow)
    {
        rawBuffer.resize(m_oldEndRow+1);
    }
    //1 替换数据只会少不会多
    if(m_oldStartRow >= 0)
    {
        //有需要替换的数据
        for(int r=m_oldStartRow;r<m_oldEndRow;++r)
        {
            rawBuffer[r] = m_oldData[r - m_oldStartRow];
        }
    }
    data->setDirty(m_isOldDirty);
}

