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

///
/// \brief 数据表格Edit的内部处理类，会根据参数类型不一样实例化不一样的内容
///
class SAValueTableOptEditValueCommandPrivateBase : public SAAbstractValueTableOptCommandPrivate
{
public:
    SAValueTableOptEditValueCommandPrivateBase():SAAbstractValueTableOptCommandPrivate(){}
    virtual ~SAValueTableOptEditValueCommandPrivateBase(){}
    virtual bool isValid() const = 0;
};
/////Edit 相关/////////////////////////////////////////////////

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
    bool m_isValid;
    SAVectorDatas<T>* m_data;
    int m_startRow;
    int m_startCol;
    int m_oldSize;
    int m_newSize;
    QVariant m_oldVal;
    QVariant m_newVal;
};

template<typename T>
class SAValueTableOptEditTableValueCommandPrivate : public SAValueTableOptEditValueCommandPrivateBase
{
public:
    SAValueTableOptEditTableValueCommandPrivate(SAAbstractDatas* data
                                                 ,const QVariant& val
                                                 , int startRow
                                                 , int startCol);
    ~SAValueTableOptEditTableValueCommandPrivate();
    bool isValid() const;
    void init(const QVariant& val);
    virtual void redo();
    virtual void undo();
private:
    SATableData<T>* m_data;
    bool m_isValid;
    int m_startRow;
    int m_startCol;
    QSize m_oldSize;
    int m_newSize;
    T m_oldVal;
    T m_newVal;
    bool m_isOldDirty;
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


//////SAValueTableOptEditTableValueCommandPrivate/////////////////////////////////////////////////////////////////////////////

template<typename T>
SAValueTableOptEditTableValueCommandPrivate<T>::SAValueTableOptEditTableValueCommandPrivate(
        SAAbstractDatas *data
        , const QVariant &val
        , int startRow
        , int startCol)
    :SAValueTableOptEditValueCommandPrivateBase()
    ,m_data(nullptr)
    ,m_isValid(false)
    ,m_startRow(startRow)
    ,m_startCol(startCol)
    ,m_isOldDirty(false)
{
    switch(data->getType())
    {
    case SA::TableDouble:
    case SA::TableVariant:
    {
        m_data = static_cast<SATableData<T>*>(data);
        init(val);
        break;
    }
    default:
        break;
    }
}
template<typename T>
SAValueTableOptEditTableValueCommandPrivate<T>::~SAValueTableOptEditTableValueCommandPrivate()
{

}
template<typename T>
bool SAValueTableOptEditTableValueCommandPrivate<T>::isValid() const
{
    return m_isValid;
}
template<typename T>
void SAValueTableOptEditTableValueCommandPrivate<T>::init(const QVariant &val)
{
    //获取第二维长度
    m_oldSize.setWidth(m_data->columnCount());
    m_oldSize.setHeight(m_data->rowCount());
    //
    m_newVal = val.value<T>();
    if(m_data->isHaveData(m_startRow,m_startCol))
    {
        //有数据，就保留原来数据
        m_oldVal = m_data->getValue(m_startRow,m_startCol);
    }
    m_isOldDirty = m_data->isDirty();
    m_isValid = true;

}
template<typename T>
void SAValueTableOptEditTableValueCommandPrivate<T>::redo()
{
    if(!isValid())
        return;
    m_data->setTableData(m_startRow,m_startCol,m_newVal);
}
template<typename T>
void SAValueTableOptEditTableValueCommandPrivate<T>::undo()
{
    if(!isValid())
        return;
    m_data->removeTableData(m_startRow,m_startCol);
}


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
        m_oldVal = m_data->getAt({(unsigned int)m_startRow,(unsigned int)m_startCol});
    }
    m_newVal = val;
    m_isValid = true;
}
template<typename T>
void SAValueTableOptEditVectorValueCommandPrivate<T>::redo()
{
    if(!isValid())
        return;
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
    if(!isValid())
        return;
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
    ,m_data(nullptr)
    ,m_isValid(false)
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
            m_oldVal.append(m_data->getAt({(unsigned int)m_startRow,(unsigned int)(m_startCol+i)}));
        }
    }
    m_newVal = val;
    m_isValid = true;
}
template<typename T>
void SAValueTableOptEditVectorMultValuesCommandPrivate<T>::redo()
{
    if(!isValid())
        return;
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
    if(!isValid())
        return;
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





//////////////////////////////////////////////////////////////////////////////
//////SAValueTableOptEditValueCommand/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

SAValueTableOptEditValueCommand::SAValueTableOptEditValueCommand(
        SAAbstractDatas *data
         , const QVariant &newData
         , int row
         , int col
         , QUndoCommand *par)
    :SAAbstractValueTableOptCommand(data,par)
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
        d_ptr = new SAValueTableOptEditVectorValueCommandPrivate<QwtIntervalSample>(data,newData,row,col);
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
    case SA::TableDouble:
        d_ptr = new SAValueTableOptEditTableValueCommandPrivate<double>(data,newData,row,col);
        break;
    case SA::TableVariant:
        d_ptr = new SAValueTableOptEditTableValueCommandPrivate<QVariant>(data,newData,row,col);
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
    :SAAbstractValueTableOptCommand(data,par)
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
        d_ptr = new SAValueTableOptEditVectorMultValuesCommandPrivate<QwtIntervalSample>(data,newDatas,row,col);
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
    {
        delete d_ptr;
    }
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
