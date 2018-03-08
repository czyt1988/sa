#include "SAValueTableOptCommands.h"
#include "SAData.h"
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


SAValueTableOptPasteValueCommand::SAValueTableOptPasteValueCommand(
        SAAbstractDatas *data
        , SADataTableModel *model
        , const QList<QVariantList> &clipboardTextTable
        , const QSize &tableSize
        , int startCol
        , int endCol
        , int startRow
        , int endRow
        , QUndoCommand *par
    )
    :SAValueTableOptBaseCommand(data,model,par)
    ,m_isvalid(false)
    ,m_startCol(startCol)
    ,m_endCol(endCol)
    ,m_startRow(startRow)
    ,m_endCol(endRow)
    ,m_oldEndRow(-1)
    ,m_oldStartRow(-1)
    ,m_oldEndCol(-1)
    ,m_oldStartCol(-1)
{
    switch(data->getType())
    {
    case SA::VectorDouble:
        initVectorDouble(clipboardTextTable,tableSize);
        break;
    default:
        break;
    }

}




void SAValueTableOptPasteValueCommand::initVectorDouble(const QList<QVariantList>& clipboardTable,const QSize &tableSize)
{
    if(0 != (m_endCol - m_startCol))
        return;
    if(1 != tableSize.width())
        return;
    SAVectorDouble *data = static_cast<SAVectorDouble*>(getDataPtr());
    const int rowCount = m_endRow - m_startRow + 1;
    const int rawRowCount = data->getValueDatas().size();
    m_oldEndCol = m_oldStartCol = 0;
    m_oldStartRow = (m_startRow < rawRowCount) ? m_startRow : -1;
    m_oldEndRow = (m_endRow < rawRowCount)? m_endRow : rawRowCount-1;
    //首先判断是否会追加，如果要追加但列数不正确需要退出
    if()
    //提取需要复制的数据
    for(int r=0;r<rowCount;++r)
    {
        QVariantList varCol;

        if(!checkVarList(clipboardTable,r,0))
            return;
        QVariant v = clipboardTable[r][0];
        bool isOK = false;
        double d = v.toDouble(&isOK);
        if(!isOK)
        {
            varCol.append(QVariant());
        }
        else
        {
            varCol.append(d);
        }
        m_newData.append(varCol);
    }
    //提取原有数据

    for(int r=m_startRow;r<rawRowCount;++r)
    {
        QVariantList varCol;
        varCol.append(data->get(r));
        m_oldData.append(varCol);
    }
    //完成处理
    m_isvalid = true;
}

bool SAValueTableOptPasteValueCommand::checkVarList(const QList<QVariantList> &varTable, int row, int col)
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

void SAValueTableOptPasteValueCommand::redo()
{
    if(!m_isvalid)
        return;
    SAAbstractDatas *data = getDataPtr();
    const int dataDim1 = data->getSize(SA::Dim1);
    const int dataDim2 = data->getSize(SA::Dim2);
    //1 替换数据
    for(int r=m_startRow;i<dataDim1;++r)
    {
        for(int c = m_startCol;c<dataDim2;++c)
        {
            if(!checkVarList(m_newData,r-m_startRow,c-m_startCol))
                continue;
            data->setAt(m_newData[r-m_startRow][c-m_startCol],{r,c});
        }
    }
    //2 追加数据
    //先判断是否需要追加
    if(m_endRow != m_oldEndRow)
    {
        //新的结尾行和旧的结尾行不一样就需要追加
    }
}

void SAValueTableOptPasteValueCommand::undo()
{

}
