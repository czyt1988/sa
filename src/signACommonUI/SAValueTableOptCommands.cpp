#include "SAValueTableOptCommands.h"
#include "SAData.h"
#include "SADataTableModel.h"
#include "SAUIHelper.h"

///
/// \brief SAValueTableOptPasteCommand的内部处理类，会根据参数类型不一样实例化不一样的内容
///
class SAValueTableOptPasteCommandPrivateBase
{
public:
    SAValueTableOptPasteCommandPrivateBase()
    {

    }
    virtual ~SAValueTableOptPasteCommandPrivateBase()
    {

    }
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
    virtual void init(const QList<QVariantList>* clipboardTable) = 0;
    virtual void redo() = 0;
    virtual void undo() = 0;
    virtual bool isValid() const = 0;
    static QSize getClipboardTableSize(const QList<QVariantList>& clipboardTable);
};
///
/// \brief The SAValueTableOptDoubleVectorPasteCommandPrivate class
///
/// 处理DoubleVector的复制粘贴
///
class SAValueTableOptDoubleVectorPasteCommandPrivate : public SAValueTableOptPasteCommandPrivateBase
{
public:
    SAValueTableOptDoubleVectorPasteCommandPrivate(SAAbstractDatas* data
                                                   ,SADataTableModel* model
                                                   , const QList<QVariantList>& clipboardTextTable
                                                   , int startRow
                                                   , int startCol);
    virtual void init(const QList<QVariantList>* clipboardTable);
    virtual void redo();
    virtual void undo();
    virtual bool isValid() const;
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
    //
    SADataTableModel *m_model;
    SAVectorDouble *m_data;
    //
};

///
/// \brief The SAValueTableOptDoubleVectorPasteCommandPrivate class
///
/// 处理DoubleVector的复制粘贴
///
class SAValueTableOptPointFVectorPasteCommandPrivate : public SAValueTableOptPasteCommandPrivateBase
{
public:
    SAValueTableOptPointFVectorPasteCommandPrivate(SAAbstractDatas* data
                                                   , SADataTableModel* model
                                                   , const QList<QVariantList>& clipboardTextTable
                                                   , int startRow
                                                   , int startCol);
    virtual void init(const QList<QVariantList>* clipboardTable);
    virtual void redo();
    virtual void undo();
    virtual bool isValid() const;
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
    //
    SADataTableModel *m_model;
    SAVectorPointF *m_data;
};

///
/// \brief The SAValueTableOptTablePasteCommandPrivate class
///
/// 处理Table的复制粘贴
///
template<typename T>
class SAValueTableOptTablePasteCommandPrivate : public SAValueTableOptPasteCommandPrivateBase
{
public:
    SAValueTableOptTablePasteCommandPrivate(SAAbstractDatas* data
                                                   , SADataTableModel* model
                                                   , const QList<QVariantList>& clipboardTextTable
                                                   , int startRow
                                                   , int startCol);
    virtual void init(const QList<QVariantList>* clipboardTable);
    virtual void redo();
    virtual void undo();
    virtual bool isValid() const;
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
    typename SATableData<T>::Table m_oldData;
    typename SATableData<T>::Table m_newData;
    //
    SADataTableModel *m_model;
    SATableVariant *m_data;
};
/////////////////////////////////////////////////////////////////
bool SAValueTableOptPasteCommandPrivateBase::checkVarList(const QList<QVariantList> &varTable, int row, int col)
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

QSize SAValueTableOptPasteCommandPrivateBase::getClipboardTableSize(const QList<QVariantList> &clipboardTable)
{
    int maxColumn = 0;
    const int row = clipboardTable.size();
    for(int i=0;i<row;++i)
    {
        const QVariantList& colVar = clipboardTable.at(i);
        if(colVar.size() > maxColumn)
        {
            maxColumn = colVar.size();
        }
    }
    return QSize(maxColumn,row);
}


//////////////////////////////////////////////////////////////////////

SAValueTableOptDoubleVectorPasteCommandPrivate::SAValueTableOptDoubleVectorPasteCommandPrivate(
        SAAbstractDatas *data
        , SADataTableModel *model
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol
        )
    :SAValueTableOptPasteCommandPrivateBase()
    ,m_model(model)
    ,m_isvalid(false)
    ,m_startRow(startRow)
    ,m_endRow(-1)
    ,m_oldEndRow(-1)
    ,m_oldStartRow(-1)
{
    if(data->getType() == SA::VectorDouble)
    {
        m_data = static_cast<SAVectorDouble*>(data);
        init(&clipboardTextTable);
    }
}

void SAValueTableOptDoubleVectorPasteCommandPrivate::init(const QList<QVariantList>* clipboardTable)
{
    const int clipSize = clipboardTable->size();
    m_newData.reserve(clipSize);
    for(int i=0;i<clipSize;++i)
    {
        if(!checkVarList((*clipboardTable),i,0))
            return;
        QVariant v = (*clipboardTable)[i][0];
        double d = v.toDouble();//没转换成功就设置为0
        m_newData.append(d);
    }
    //记录行变换
    m_endRow = m_startRow + m_newData.size();
    //提取需要复制的数据
    const int rawRowCount = m_data->getValueDatas().size();
    m_oldStartRow = (m_startRow < rawRowCount) ? m_startRow : -1;
    m_oldEndRow = (m_endRow < rawRowCount)? m_endRow : rawRowCount-1;

    //提取原有数据
    if(m_oldStartRow >= 0)
    {
        m_oldData.reserve(m_oldEndRow-m_oldStartRow+1);
        for(int i=m_oldStartRow;i<m_oldEndRow;++i)
        {
            m_oldData.append(m_data->get(i));
        }
    }
    //完成处理
    m_isvalid = true;
}

void SAValueTableOptDoubleVectorPasteCommandPrivate::redo()
{
    if(!m_isvalid)
        return;
    QVector<double>& rawBuffer = m_data->getValueDatas();
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
    m_isOldDirty = m_data->isDirty();
    m_data->setDirty(true);
}

void SAValueTableOptDoubleVectorPasteCommandPrivate::undo()
{
    if(!m_isvalid)
        return;
    QVector<double>& rawBuffer = m_data->getValueDatas();
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
    m_data->setDirty(m_isOldDirty);
}

bool SAValueTableOptDoubleVectorPasteCommandPrivate::isValid() const
{
    return m_isvalid;
}

/////////////////////////////////////////////////////////////////

SAValueTableOptPointFVectorPasteCommandPrivate::SAValueTableOptPointFVectorPasteCommandPrivate(
        SAAbstractDatas *data
        , SADataTableModel *model
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol)
    :SAValueTableOptPasteCommandPrivateBase()
    ,m_model(model)
    ,m_isvalid(false)
    ,m_startRow(startRow)
    ,m_endRow(-1)
    ,m_startCol(startCol)
    ,m_endCol(-1)
    ,m_oldEndRow(-1)
    ,m_oldStartRow(-1)
{
    QSize tableSize = getClipboardTableSize(clipboardTextTable);
    if(1 == tableSize.width())
    {
        m_endCol = startCol;
    }
    if(2 == tableSize.width())
    {
        m_endCol = startCol+1;
    }
    if(m_startCol == m_endCol || (0 == m_startCol && 1 == m_endCol))
    {
        //只有两种情况:
        //1.开始列和结束列相等
        //2.开始列为0，结束列为1
        if(SA::VectorPoint == data->getType())
        {
            m_data = static_cast<SAVectorPointF*>(data);
            init(&clipboardTextTable);
        }
    }
}

void SAValueTableOptPointFVectorPasteCommandPrivate::init(const QList<QVariantList> *clipboardTable)
{
    const int clipSize = clipboardTable->size();
    const int oldDataSize = m_data->getValueDatas().size();
    m_newData.reserve(clipSize);

    //只有两种情况:
    //1.开始列和结束列相等
    //2.开始列为0，结束列为1
    if(m_startCol==m_endCol)
    {
        //只复制了一列是不允许超过原有的长度的
        if((clipSize + m_startRow) > oldDataSize)
        {
            return;
        }
        int successReadCount = 0;
        m_oldData.reserve(clipSize);
        for(int i=0;i<clipSize;++i)
        {
            double d;
            if(!checkVarList(*clipboardTable,i,0))
                return;
            if(!safeGetValue((*clipboardTable)[i][0],d))
                continue;

            QPointF value = m_data->get(m_startRow+successReadCount);
            m_oldData.append(value);
            if(0 == m_startCol)
            {
                value.setX(d);
            }
            else
            {
                value.setY(d);
            }
            m_newData.append(value);
            ++successReadCount;
        }
        //记录行变换
        m_endRow = m_startRow + m_newData.size();
        //提取需要复制的数据
        m_oldStartRow = m_startRow;//由于单行不能增长，因此不需要特殊处理
        m_oldEndRow = m_endRow;
    }
    else
    {
        for(int i=0;i<clipSize;++i)
        {
            double x;
            if(!checkVarList(*clipboardTable,i,0))
                return;
            if(!safeGetValue((*clipboardTable)[i][0],x))
                continue;

            double y;
            if(!checkVarList(*clipboardTable,i,1))
                return;
            if(!safeGetValue((*clipboardTable)[i][1],y))
                continue;

            m_newData.append(QPointF(x,y));
        }
        //记录行变换
        m_endRow = m_startRow + m_newData.size();
        m_oldStartRow = (m_startRow < oldDataSize) ? m_startRow : -1;
        m_oldEndRow = (m_endRow < oldDataSize)? m_endRow : oldDataSize-1;
        //提取原有数据
        if(m_oldStartRow >= 0)
        {
            m_oldData.reserve(m_oldEndRow-m_oldStartRow+1);
            for(int i=m_oldStartRow;i<m_oldEndRow;++i)
            {
                m_oldData.append(m_data->get(i));
            }
        }
    }
    //完成处理
    m_isvalid = true;
}

void SAValueTableOptPointFVectorPasteCommandPrivate::redo()
{
    if(!m_isvalid)
        return;
    QVector<QPointF>& rawBuffer = m_data->getValueDatas();
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
    m_isOldDirty = m_data->isDirty();
    m_data->setDirty(true);
}

void SAValueTableOptPointFVectorPasteCommandPrivate::undo()
{
    if(!m_isvalid)
        return;
    QVector<QPointF>& rawBuffer = m_data->getValueDatas();
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
    m_data->setDirty(m_isOldDirty);
}

bool SAValueTableOptPointFVectorPasteCommandPrivate::isValid() const
{
    return m_isvalid;
}


///////////////////////////////////////////////

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
    if(m_realRow < d->getSize(SA::Dim1) && m_realCol < d->getSize(SA::Dim2))
    {
        d->setAt(m_newDatas,{(size_t)m_realRow,(size_t)m_realCol});
    }
    else
    {
        qDebug() << "set row or col out of range";
    }
    m->update();
}

void SAValueTableOptEditValueCommand::undo()
{
    SAAbstractDatas* d = getDataPtr();
    SADataTableModel* m = getModel();
    d->setAt(m_oldDatas,{(size_t)m_realRow,(size_t)m_realCol});
    m->update();
}

////////////////////////////////////////////////////////////////////






/////////////////////////////////////////////////////////////////////////

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


SAValueTableOptPasteCommand::SAValueTableOptPasteCommand(
        SAAbstractDatas *data
        , SADataTableModel *model
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol
        , QUndoCommand *par)
    :SAValueTableOptBaseCommand(data,model,par)
    ,d_ptr(nullptr)
{
    switch(data->getType())
    {
    case SA::VectorDouble:
    {
        d_ptr = new SAValueTableOptDoubleVectorPasteCommandPrivate(data,model,clipboardTextTable,startRow,startCol);
        break;
    }
    case SA::VectorPoint:
    {
        d_ptr = new SAValueTableOptPointFVectorPasteCommandPrivate(data,model,clipboardTextTable,startRow,startCol);
        break;
    }
    case SA::TableDouble:
    {
        break;
    }
    default:
        break;
    }
}

SAValueTableOptPasteCommand::~SAValueTableOptPasteCommand()
{
    if(d_ptr)
    {
        delete d_ptr;
    }
}


bool SAValueTableOptPasteCommand::isValid() const
{
    if(d_ptr)
    {
        return d_ptr->isValid();
    }
    return false;
}

void SAValueTableOptPasteCommand::redo()
{
    if(d_ptr)
    {
        return d_ptr->redo();
    }
}

void SAValueTableOptPasteCommand::undo()
{
    if(d_ptr)
    {
        return d_ptr->undo();
    }
}


