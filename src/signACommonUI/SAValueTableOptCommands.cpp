#include "SAValueTableOptCommands.h"
#include "SAData.h"
#include "SADataTableModel.h"
#include "SAUIHelper.h"
#include "SAVectorDatas.h"
#include "SAVectorInt.h"
#include "SAVectorDouble.h"
#include "SAVectorInterval.h"
#include "SAVectorPointF.h"
#include "SAVectorVariant.h"
#include "SAVectorOHLCDatas.h"
class SAValueTableOptCommandPrivateBase
{
public:
    SAValueTableOptCommandPrivateBase()
    {

    }
    virtual ~SAValueTableOptCommandPrivateBase()
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
    virtual void redo() = 0;
    virtual void undo() = 0;
};


///
/// \brief Edit的内部处理类，会根据参数类型不一样实例化不一样的内容
///
class SAValueTableOptEditValueCommandPrivateBase : public SAValueTableOptCommandPrivateBase
{
public:
    SAValueTableOptEditValueCommandPrivateBase(){}
    virtual ~SAValueTableOptEditValueCommandPrivateBase(){}
    virtual bool isValid() const = 0;
};

///
/// \brief SAValueTableOptPasteCommand的内部处理类，会根据参数类型不一样实例化不一样的内容
///
class SAValueTableOptPasteCommandPrivateBase : public SAValueTableOptCommandPrivateBase
{
public:
    SAValueTableOptPasteCommandPrivateBase(){}
    virtual ~SAValueTableOptPasteCommandPrivateBase(){}
    virtual bool isValid() const = 0;
    static QSize getClipboardTableSize(const QList<QVariantList>& clipboardTable);
};


template<typename T>
class SAValueTableOptEditVectorValueCommandPrivate : public SAValueTableOptEditValueCommandPrivateBase
{
public:
    SAValueTableOptEditVectorValueCommandPrivate(SAAbstractDatas* data
                                                 ,const QVariant& val
                                                 , int startRow
                                                 , int startCol);
    ~SAValueTableOptEditVectorValueCommandPrivate();
    bool isValid() const;
    void init(const QVariant& val);
    virtual void redo();
    virtual void undo();
private:
    SAVectorDatas<T>* m_data;
    bool m_isValid;
    int m_startRow;
    int m_startCol;
    int m_oldSize;
    int m_newSize;
    QVariant m_oldVal;
    QVariant m_newVal;
};

template<typename T>
class SAValueTableOptEditVectorMultValuesCommandPrivate : public SAValueTableOptEditValueCommandPrivateBase
{
public:
    SAValueTableOptEditVectorMultValuesCommandPrivate(SAAbstractDatas* data
                                                 ,const QVariantList& vals
                                                 , int startRow
                                                 , int startCol);
    ~SAValueTableOptEditVectorMultValuesCommandPrivate();
    bool isValid() const;
    void init(const QVariantList& val);
    virtual void redo();
    virtual void undo();
private:
    SAVectorDatas<T>* m_data;
    bool m_isValid;
    int m_startRow;
    int m_startCol;
    int m_oldSize;
    int m_newSize;
    QVariantList m_oldVal;
    QVariantList m_newVal;
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
                                                   , const QList<QVariantList>& clipboardTextTable
                                                   , int startRow
                                                   , int startCol);
    void init(const QList<QVariantList>* clipboardTable);
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
                                                   , const QList<QVariantList>& clipboardTextTable
                                                   , int startRow
                                                   , int startCol);
    void init(const QList<QVariantList>* clipboardTable);
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
    int m_startCol;
    //
    bool m_isOldDirty;
    //
    SATableData<T> *m_data;
    //
    QList<QPair<QPoint,T> > m_tableNewData;
    QList<QPair<QPoint,T> > m_tableOldData;
    QList<QPoint> m_willRemovePos;///< 需要删除的索引
};


//////SAValueTableOptCommandPrivateBase/////////////////////////////////////////////////////
bool SAValueTableOptCommandPrivateBase::checkVarList(const QList<QVariantList> &varTable, int row, int col)
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

//////SAValueTableOptPasteCommandPrivateBase///////////////////////////////////////////////////////////

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


///////SAValueTableOptDoubleVectorPasteCommandPrivate///////////////////////////////////////////////////////////////

template<typename T>
SAValueTableOptEditVectorValueCommandPrivate<T>::SAValueTableOptEditVectorValueCommandPrivate(
        SAAbstractDatas *data
        , const QVariant &val
        , int startRow
        , int startCol)
    :SAValueTableOptEditValueCommandPrivateBase()
    ,m_isValid(false)
    ,m_data(nullptr)
    ,m_startRow(startRow)
    ,m_startCol(startCol)
{
    switch(data->getType())
    {
    case SA::VectorInt:
    case SA::VectorDouble:
    case SA::VectorInterval:
    case SA::VectorOHLC:
    case SA::VectorPoint:
    case SA::VectorVariant:
    {
        m_data = static_cast<SAVectorDatas<T>*>(data);
        init(val);
        break;
    }
    default:
        break;
    }
}
template<typename T>
SAValueTableOptEditVectorValueCommandPrivate<T>::~SAValueTableOptEditVectorValueCommandPrivate()
{

}
template<typename T>
bool SAValueTableOptEditVectorValueCommandPrivate<T>::isValid() const
{
    return m_isValid;
}

template<typename T>
void SAValueTableOptEditVectorValueCommandPrivate<T>::init(const QVariant &val)
{
    //获取第二维长度
    int dim2 = m_data->getSize(SA::Dim2);

    if(m_startCol >= dim2)
    {
        if(0 != dim2)
        {
            return;
        }
        else
        {
            //0维度时列数为0也是正确的
            if(0 != m_startCol)
            {
                return;
            }
        }
    }
    m_newSize = m_oldSize = m_data->getValueDatas().size();
    if(m_startRow >= m_oldSize)
    {
        //说明是插入数据，需要记录新尺寸
        m_newSize = m_startRow+1;
    }
    else
    {
        //修改数据就记录原有的值
        m_oldVal = m_data->getAt({m_startRow,m_startCol});
    }
    m_newVal = val;
    m_isValid = true;
}
template<typename T>
void SAValueTableOptEditVectorValueCommandPrivate<T>::redo()
{
    QVector<T> & innerData = m_data->getValueDatas();
    while(innerData.size() < m_newSize)
    {
        //插入默认数据达到一样的长度
        innerData.append(T());
    }
    m_data->setAt(m_newVal,{(size_t)m_startRow,(size_t)m_startCol});
}
template<typename T>
void SAValueTableOptEditVectorValueCommandPrivate<T>::undo()
{
    QVector<T> & innerData = m_data->getValueDatas();
    if(innerData.size() != m_oldSize)
    {
        innerData.resize(m_oldSize);
    }
    else
    {
        m_data->setAt(m_oldVal,{(size_t)m_startRow,(size_t)m_startCol});
    }
}


//////////////////////////////////////////////////////////////////////////////////////

template<typename T>
SAValueTableOptEditVectorMultValuesCommandPrivate<T>::SAValueTableOptEditVectorMultValuesCommandPrivate(
        SAAbstractDatas *data
        , const QVariantList &vals
        , int startRow
        , int startCol)
    :SAValueTableOptEditValueCommandPrivateBase()
    ,m_isValid(false)
    ,m_data(nullptr)
    ,m_startRow(startRow)
    ,m_startCol(startCol)
{
    switch(data->getType())
    {
    case SA::VectorInt:
    case SA::VectorDouble:
    case SA::VectorInterval:
    case SA::VectorOHLC:
    case SA::VectorPoint:
    case SA::VectorVariant:
    {
        m_data = static_cast<SAVectorDatas<T>*>(data);
        init(vals);
        break;
    }
    default:
        break;
    }
}
template<typename T>
SAValueTableOptEditVectorMultValuesCommandPrivate<T>::~SAValueTableOptEditVectorMultValuesCommandPrivate()
{

}
template<typename T>
bool SAValueTableOptEditVectorMultValuesCommandPrivate<T>::isValid() const
{
    return m_isValid;
}
template<typename T>
void SAValueTableOptEditVectorMultValuesCommandPrivate<T>::init(const QVariantList &val)
{
    //获取第二维长度
    int editColSize = val.size();
    int dim2 = m_data->getSize(SA::Dim2);

    if(m_startCol >= dim2 || m_startCol+editColSize >= dim2)
    {
        if(0 != dim2)
        {
            return;
        }
        else
        {
            //0维度时列数为0也是正确的
            if(0 != m_startCol || 1 != editColSize)
            {
                return;
            }
        }
    }
    m_newSize = m_oldSize = m_data->getValueDatas().size();
    if(m_startRow >= m_oldSize)
    {
        //说明是插入数据，需要记录新尺寸
        m_newSize = m_startRow+1;
    }
    else
    {
        //修改数据就记录原有的值
        for(int i=0;i<editColSize;++i)
        {
            m_oldVal.append(m_data->getAt({m_startRow,m_startCol+i}));
        }
    }
    m_newVal = val;
    m_isValid = true;
}
template<typename T>
void SAValueTableOptEditVectorMultValuesCommandPrivate<T>::redo()
{
    QVector<T> & innerData = m_data->getValueDatas();
    while(innerData.size() < m_newSize)
    {
        //插入默认数据达到一样的长度
        innerData.append(T());
    }
    int editColSize = m_newVal.size();
    for(int i=0;i<editColSize;++i)
        m_data->setAt(m_newVal[i],{(size_t)m_startRow,(size_t)m_startCol+i});
}
template<typename T>
void SAValueTableOptEditVectorMultValuesCommandPrivate<T>::undo()
{
    QVector<T> & innerData = m_data->getValueDatas();
    if(innerData.size() != m_oldSize)
    {
        innerData.resize(m_oldSize);
    }
    else
    {
        int editColSize = m_oldVal.size();
        for(int i=0;i<editColSize;++i)
            m_data->setAt(m_oldVal[i],{(size_t)m_startRow,(size_t)m_startCol+i});
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////



SAValueTableOptDoubleVectorPasteCommandPrivate::SAValueTableOptDoubleVectorPasteCommandPrivate(
        SAAbstractDatas *data
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol
        )
    :SAValueTableOptPasteCommandPrivateBase()
    ,m_isvalid(false)
    ,m_startRow(startRow)
    ,m_endRow(-1)
    ,m_oldEndRow(-1)
    ,m_oldStartRow(-1)
{
    Q_UNUSED(startCol);

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

///////SAValueTableOptPointFVectorPasteCommandPrivate//////////////////////////////////////////////////////////

SAValueTableOptPointFVectorPasteCommandPrivate::SAValueTableOptPointFVectorPasteCommandPrivate(
        SAAbstractDatas *data
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol)
    :SAValueTableOptPasteCommandPrivateBase()
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

////////SAValueTableOptTablePasteCommandPrivate///////////////////////////////////////////////

template<typename T>
SAValueTableOptTablePasteCommandPrivate<T>::SAValueTableOptTablePasteCommandPrivate(
        SAAbstractDatas *data
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol)
    :SAValueTableOptPasteCommandPrivateBase()
    ,m_isvalid(false)
    ,m_startRow(startRow)
    ,m_startCol(startCol)
{
    int type = data->getType();
    if(SA::TableDouble == type || SA::TableVariant == type)
    {
        m_data = static_cast<SATableData<T>*>(data);
        init(&clipboardTextTable);
    }
}

template<typename T>
void SAValueTableOptTablePasteCommandPrivate<T>::init(const QList<QVariantList> *clipboardTable)
{
    const int rowCount = clipboardTable->size();
    const typename SATableData<T>::Table& table = m_data->getTable();
    for(int r=0;r<rowCount;++r)
    {
        const QVariantList& rowValue = clipboardTable->at(r);
        const int colCount = rowValue.size();
        for(int c=0;c<colCount;++c)
        {
            int rr = r+m_startRow;
            int rc = c+m_startCol;
            bool isHaveData = table.isHaveData(rr,rc);

            if(isHaveData)
            {
                //先把旧数据提取
                m_tableOldData.append(qMakePair(QPoint(rr,rc)
                                                ,table.at(rr,rc)));
            }

            if(rowValue.at(c).isValid())
            {
                //新加入的数据
                m_tableNewData.append(qMakePair(QPoint(rr,rc)
                                                ,rowValue.at(c).value<T>()));
            }
            else
            {
                //剪切板空，但原来有，说明要删除的数据
                if(isHaveData)
                {
                    m_willRemovePos.append(QPoint(rr,rc));
                }
            }
        }
    }
    //完成处理
    if(m_tableNewData.size() > 0 || m_tableOldData.size()>0 || m_willRemovePos.size()>0)
    {
        m_isvalid = true;
    }
}

template<typename T>
void SAValueTableOptTablePasteCommandPrivate<T>::redo()
{
    if(!m_isvalid)
        return;
    typename SATableData<T>::Table& table = m_data->getTable();
    //! 1先删除
    for(int i=0;i<m_willRemovePos.size();++i)
    {
        const QPoint& pos = m_willRemovePos.at(i);
        table.removeData(pos.x(),pos.y());
    }
    //! 2插入新数据
    for(int i=0;i<m_tableNewData.size();++i)
    {
        const QPair<QPoint,T>& pair = m_tableNewData.at(i);
        table.setData(pair.first.x(),pair.first.y(),pair.second);
    }
    m_isOldDirty = m_data->isDirty();
    m_data->setDirty(true);
}

template<typename T>
void SAValueTableOptTablePasteCommandPrivate<T>::undo()
{
    if(!m_isvalid)
        return;
    typename SATableData<T>::Table& table = m_data->getTable();
    //! 1删除数据
    for(int i=0;i<m_tableNewData.size();++i)
    {
        const QPair<QPoint,T>& pair = m_tableNewData.at(i);
        table.removeData(pair.first.x(),pair.first.y());
    }
    //! 2插入旧数据
    for(int i=0;i<m_tableOldData.size();++i)
    {
        const QPair<QPoint,T>& pair = m_tableOldData.at(i);
        table.setData(pair.first.x(),pair.first.y(),pair.second);
    }
    m_data->setDirty(m_isOldDirty);
}

template<typename T>
bool SAValueTableOptTablePasteCommandPrivate<T>::isValid() const
{
    return m_isvalid;
}

//////////////////////////////////////////////////////////////////////////////
//////SAValueTableOptEditValueCommand/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

SAValueTableOptEditValueCommand::SAValueTableOptEditValueCommand(
        SAAbstractDatas *data
         , const QVariant &newData
         , int row
         , int col
         , QUndoCommand *par)
    :SAValueTableOptBaseCommand(data,par)
    ,d_ptr(nullptr)
{
    switch(data->getType())
    {
    case SA::VectorInt:
        d_ptr = new SAValueTableOptEditVectorValueCommandPrivate<int>(data,newData,row,col);
        break;
    case SA::VectorDouble:
        d_ptr = new SAValueTableOptEditVectorValueCommandPrivate<double>(data,newData,row,col);
        break;
    case SA::VectorInterval:
        d_ptr = new SAValueTableOptEditVectorValueCommandPrivate<QwtInterval>(data,newData,row,col);
        break;
    case SA::VectorOHLC:
        d_ptr = new SAValueTableOptEditVectorValueCommandPrivate<QwtOHLCSample>(data,newData,row,col);
        break;
    case SA::VectorPoint:
        d_ptr = new SAValueTableOptEditVectorValueCommandPrivate<QPointF>(data,newData,row,col);
        break;
    case SA::VectorVariant:
        d_ptr = new SAValueTableOptEditVectorValueCommandPrivate<QVariant>(data,newData,row,col);
        break;
    default:
        break;
    }
}

SAValueTableOptEditValueCommand::SAValueTableOptEditValueCommand(
        SAAbstractDatas *data
        , const QVariantList &newDatas
        , int row
        , int col
        , QUndoCommand *par)
    :SAValueTableOptBaseCommand(data,par)
    ,d_ptr(nullptr)
{
    switch(data->getType())
    {
    case SA::VectorInt:
        d_ptr = new SAValueTableOptEditVectorMultValuesCommandPrivate<int>(data,newDatas,row,col);
        break;
    case SA::VectorDouble:
        d_ptr = new SAValueTableOptEditVectorMultValuesCommandPrivate<double>(data,newDatas,row,col);
        break;
    case SA::VectorInterval:
        d_ptr = new SAValueTableOptEditVectorMultValuesCommandPrivate<QwtInterval>(data,newDatas,row,col);
        break;
    case SA::VectorOHLC:
        d_ptr = new SAValueTableOptEditVectorMultValuesCommandPrivate<QwtOHLCSample>(data,newDatas,row,col);
        break;
    case SA::VectorPoint:
        d_ptr = new SAValueTableOptEditVectorMultValuesCommandPrivate<QPointF>(data,newDatas,row,col);
        break;
    case SA::VectorVariant:
        d_ptr = new SAValueTableOptEditVectorMultValuesCommandPrivate<QVariant>(data,newDatas,row,col);
        break;
    default:
        break;
    }
}

SAValueTableOptEditValueCommand::~SAValueTableOptEditValueCommand()
{
    if(d_ptr)
        delete d_ptr;
}

void SAValueTableOptEditValueCommand::redo()
{
    if(d_ptr)
    {
        return d_ptr->redo();
    }
}

void SAValueTableOptEditValueCommand::undo()
{
    if(d_ptr)
    {
        return d_ptr->undo();
    }
}

bool SAValueTableOptEditValueCommand::isValid() const
{
    if(d_ptr)
    {
        return d_ptr->isValid();
    }
    return false;
}

////////////////////////////////////////////////////////////////////






/////////////////////////////////////////////////////////////////////////
#if 0
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

#endif
//===============================================


SAValueTableOptPasteCommand::SAValueTableOptPasteCommand(
        SAAbstractDatas *data
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol
        , QUndoCommand *par)
    :SAValueTableOptBaseCommand(data,par)
    ,d_ptr(nullptr)
{
    switch(data->getType())
    {
    case SA::VectorDouble:
    {
        d_ptr = new SAValueTableOptDoubleVectorPasteCommandPrivate(data,clipboardTextTable,startRow,startCol);
        break;
    }
    case SA::VectorPoint://VectorPoint在显示上比较特殊
    {
        d_ptr = new SAValueTableOptPointFVectorPasteCommandPrivate(data,clipboardTextTable,startRow,startCol);
        break;
    }
    case SA::TableDouble:
    {
        d_ptr = new SAValueTableOptTablePasteCommandPrivate<double>(data,clipboardTextTable,startRow,startCol);
        break;
    }
    case SA::TableVariant:
    {
        d_ptr = new SAValueTableOptTablePasteCommandPrivate<QVariant>(data,clipboardTextTable,startRow,startCol);
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




