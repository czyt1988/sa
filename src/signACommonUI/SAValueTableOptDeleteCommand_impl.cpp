#include "SAValueTableOptCommands.h"
#include "SAData.h"
#include "czyAlgorithm.h"

///
/// \brief SAValueTableOptDeleteCommand的内部处理类，会根据参数类型不一样实例化不一样的内容
///
class SAValueTableOptDeleteCommandPrivateBase : public SAAbstractValueTableOptCommandPrivate
{
public:
    SAValueTableOptDeleteCommandPrivateBase():SAAbstractValueTableOptCommandPrivate(){}
    virtual ~SAValueTableOptDeleteCommandPrivateBase(){}
    virtual bool isValid() const = 0;
    ///
    /// \brief 获取索引表里有序不重复的行号
    /// \param input 以QVector<QPoint>组织的索引表
    /// \param index_begin 输出的有序行号迭代器起始地址，
    /// 注意，此迭代器为普通迭代器，在函数中会使用std::back_inserter自动插入，因此别传入std::back_inserter的迭代器
    ///
    template<typename Ite>
    static void getUniqueRows(const QVector<QPoint>& input,Ite index_begin)
    {
        QVector<int> rows;
        rows.resize(input.size());
        std::transform(input.begin(),input.end(),rows.begin()
                       ,[](const QPoint& p)->int
        {
            return p.y();
        });
        std::sort(rows.begin(),rows.end());
        std::unique_copy(rows.begin(),rows.end(),index_begin);
    }

};

/////delete 相关///////////////////////////////////////////////

template<typename T>
class SAValueTableOptVectorDeleteCommandPrivate : public SAValueTableOptDeleteCommandPrivateBase
{
public:
    SAValueTableOptVectorDeleteCommandPrivate(SAAbstractDatas* data
                                               ,const QVector<QPoint>& indexs);
    bool isValid() const;
    void init(const QVector<QPoint>& indexs);
    virtual void redo();
    virtual void undo();

private:
    SAVectorDatas<T>* m_data;
    bool m_isValid;
    bool m_isOldDirty;
    QVector<T> m_deleteDatas;
    QVector<int> m_deleteIndexs;
    int m_oldDataSize;
};
template<typename T>
class SAValueTableOptTableDeleteCommandPrivate : public SAValueTableOptDeleteCommandPrivateBase
{
public:
    SAValueTableOptTableDeleteCommandPrivate(SAAbstractDatas* data
                                               ,const QVector<QPoint>& indexs);
    bool isValid() const;
    void init(const QVector<QPoint>& indexs);
    virtual void redo();
    virtual void undo();

private:
    SATableData<T>* m_data;
    bool m_isValid;
    bool m_isOldDirty;
    QList<QPair<QPoint,T> > m_tableDeletedData;
};


////////SAValueTableOptVectorDeleteCommandPrivate//////////////////////////////////////////////////////////////////////

template<typename T>
SAValueTableOptVectorDeleteCommandPrivate<T>::SAValueTableOptVectorDeleteCommandPrivate(
        SAAbstractDatas* data
        ,const QVector<QPoint>& indexs)
    :SAValueTableOptDeleteCommandPrivateBase()
    ,m_isValid(false)
    ,m_isOldDirty(true)
    ,m_oldDataSize(0)
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
        init (indexs);
        break;
    }
    default:
        break;
    }
}


template<typename T>
bool SAValueTableOptVectorDeleteCommandPrivate<T>::isValid() const
{
    return m_isValid;
}

template<typename T>
void SAValueTableOptVectorDeleteCommandPrivate<T>::init(const QVector<QPoint> &indexs)
{
    const QVector<T>& datas = m_data->getValueDatas();
    m_oldDataSize = datas.size();
    QVector<int> rowIndexs;
    rowIndexs.reserve(indexs.size());
    m_deleteIndexs.reserve(rowIndexs.size());
    getUniqueRows(indexs,std::back_inserter(rowIndexs));
    for(int i=0;i<rowIndexs.size();++i)
    {
        if(rowIndexs[i] < m_oldDataSize)
        {
            m_deleteIndexs.push_back(rowIndexs[i]);
        }
    }

    m_deleteDatas.reserve(m_deleteIndexs.size());
    for(int i=0;i<m_deleteIndexs.size();++i)
    {
        if(m_deleteIndexs[i] <= datas.size())
        {
            m_deleteDatas.push_back(datas[m_deleteIndexs[i]]);
        }
    }
    if(m_deleteIndexs.size()>0)
    {
        m_isValid = true;
        m_isOldDirty = m_data->isDirty();
    }
}

template<typename T>
void SAValueTableOptVectorDeleteCommandPrivate<T>::redo()
{
    if(!isValid())
        return;
    QVector<T>& datas = m_data->getValueDatas();
    QVector<T> newDatas;
    newDatas.reserve(datas.size());
    czy::copy_out_of_indexs(datas.begin(),datas.end(),m_deleteIndexs.begin(),m_deleteIndexs.end(),std::back_inserter(newDatas));
    datas.swap(newDatas);
    m_data->setDirty(true);
}

template<typename T>
void SAValueTableOptVectorDeleteCommandPrivate<T>::undo()
{
    if(!isValid())
        return;
    QVector<T>& datas = m_data->getValueDatas();
    QVector<T> newDatas;
    newDatas.reserve(m_oldDataSize);
    czy::insert_inner_indexs(m_deleteIndexs.begin(),m_deleteIndexs.end()
                             ,m_deleteDatas.begin()
                             ,datas.begin(),datas.end()
                             ,std::back_inserter(newDatas));
    datas.swap(newDatas);
    m_data->setDirty(m_isOldDirty);
}




//////////////////////////////////////////////////////////////////////////////
//////SAValueTableOptTableDeleteCommandPrivate/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

template<typename T>
SAValueTableOptTableDeleteCommandPrivate<T>::SAValueTableOptTableDeleteCommandPrivate(
        SAAbstractDatas *data
        , const QVector<QPoint> &indexs)
    :SAValueTableOptDeleteCommandPrivateBase()
    ,m_isValid(false)
    ,m_isOldDirty(true)
{
    switch(data->getType())
    {
    case SA::TableDouble:
    case SA::TableVariant:
    {
        m_data = static_cast<SATableData<T>*>(data);
        init(indexs);
        break;
    }
    default:
        break;
    }
}
template<typename T>
bool SAValueTableOptTableDeleteCommandPrivate<T>::isValid() const
{
    return m_isValid;

}
template<typename T>
void SAValueTableOptTableDeleteCommandPrivate<T>::init(const QVector<QPoint> &indexs)
{
    int size = indexs.size();
    for(int i=0;i<size;++i)
    {
        int r = indexs[i].y();
        int c = indexs[i].x();
        if(m_data->isHaveData(r,c))
        {
            m_tableDeletedData.append(qMakePair(indexs[i],m_data->getValue(r,c)));
        }
    }
    if(m_tableDeletedData.size()>0)
    {
        m_isValid = true;
        m_isOldDirty = m_data->isDirty();
    }
}
template<typename T>
void SAValueTableOptTableDeleteCommandPrivate<T>::redo()
{
    if(!isValid())
        return;
    int size = m_tableDeletedData.size();
    for(int i=0;i<size;++i)
    {
        int r = m_tableDeletedData[i].first.y();
        int c = m_tableDeletedData[i].first.x();
        m_data->removeTableData(r,c);
    }
}
template<typename T>
void SAValueTableOptTableDeleteCommandPrivate<T>::undo()
{
    if(!isValid())
        return;
    int size = m_tableDeletedData.size();
    for(int i=0;i<size;++i)
    {
        int r = m_tableDeletedData[i].first.y();
        int c = m_tableDeletedData[i].first.x();
        m_data->setTableData(r,c,m_tableDeletedData[i].second);
    }
}



///////////SAValueTableOptDeleteCommand////////////////////////////


SAValueTableOptDeleteCommand::SAValueTableOptDeleteCommand(
        SAAbstractDatas *data
        , const QVector<QPoint> &deleteIndexs
        , QUndoCommand *par)
    :SAAbstractValueTableOptCommand(data,par)
    ,d_ptr(nullptr)
{
    switch(data->getType())
    {
    case SA::VectorDouble:
    {
        d_ptr = new SAValueTableOptVectorDeleteCommandPrivate<double>(data,deleteIndexs);
        break;
    }
    case SA::VectorPoint://VectorPoint在显示上比较特殊
    {
        d_ptr = new SAValueTableOptVectorDeleteCommandPrivate<QPointF>(data,deleteIndexs);
        break;
    }
    case SA::VectorInterval:
    {
        d_ptr = new SAValueTableOptVectorDeleteCommandPrivate<QwtIntervalSample>(data,deleteIndexs);
        break;
    }
    case SA::VectorOHLC:
    {
        d_ptr = new SAValueTableOptVectorDeleteCommandPrivate<QwtOHLCSample>(data,deleteIndexs);
        break;
    }
    case SA::VectorVariant:
    {
        d_ptr = new SAValueTableOptVectorDeleteCommandPrivate<QVariant>(data,deleteIndexs);
        break;
    }
    case SA::TableDouble:
    {
        d_ptr = new SAValueTableOptTableDeleteCommandPrivate<double>(data,deleteIndexs);
        break;
    }
    case SA::TableVariant:
    {
        d_ptr = new SAValueTableOptTableDeleteCommandPrivate<QVariant>(data,deleteIndexs);
        break;
    }
    default:
        break;
    }
}

SAValueTableOptDeleteCommand::~SAValueTableOptDeleteCommand()
{
    if(d_ptr)
    {
        delete d_ptr;
    }
}

bool SAValueTableOptDeleteCommand::isValid() const
{
    if(d_ptr)
    {
        return d_ptr->isValid();
    }
    return false;
}

void SAValueTableOptDeleteCommand::redo()
{
    if(d_ptr)
    {
        return d_ptr->redo();
    }
}

void SAValueTableOptDeleteCommand::undo()
{
    if(d_ptr)
    {
        return d_ptr->undo();
    }
}
