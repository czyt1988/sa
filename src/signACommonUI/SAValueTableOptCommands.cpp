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
#include <iterator>
#include "czyAlgorithm.h"
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
    SAValueTableOptEditValueCommandPrivateBase():SAValueTableOptCommandPrivateBase(){}
    virtual ~SAValueTableOptEditValueCommandPrivateBase(){}
    virtual bool isValid() const = 0;
};

///
/// \brief SAValueTableOptPasteCommand的内部处理类，会根据参数类型不一样实例化不一样的内容
///
class SAValueTableOptPasteCommandPrivateBase : public SAValueTableOptCommandPrivateBase
{
public:
    SAValueTableOptPasteCommandPrivateBase():SAValueTableOptCommandPrivateBase(){}
    virtual ~SAValueTableOptPasteCommandPrivateBase(){}
    virtual bool isValid() const = 0;
    static QSize getClipboardTableSize(const QList<QVariantList>& clipboardTable);
};
///
/// \brief SAValueTableOptDeleteCommand的内部处理类，会根据参数类型不一样实例化不一样的内容
///
class SAValueTableOptDeleteCommandPrivateBase : public SAValueTableOptCommandPrivateBase
{
public:
    SAValueTableOptDeleteCommandPrivateBase():SAValueTableOptCommandPrivateBase(){}
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

///
/// \brief The SAValueTableOptVectorPasteCommandPrivate class
///
/// 处理Vector的复制粘贴
///
template<typename T,typename FunMakeT>
class SAValueTableOptVectorPasteCommandPrivate : public SAValueTableOptPasteCommandPrivateBase
{
public:
    SAValueTableOptVectorPasteCommandPrivate(SAAbstractDatas* data
                                            , const QList<QVariantList>& clipboardTextTable
                                            , int startRow
                                            , int startCol
                                            ,FunMakeT fp);
    void init(const QList<QVariantList>* clipboardTable);
    virtual void redo();
    virtual void undo();
    virtual bool isValid() const;
private:
    FunMakeT m_funPtr;///< void fun(T& data,int col,QVariant val)
    bool m_isvalid;
    //新数据的区域定位
    int m_startRow;
    int m_startCol;
    //
    bool m_isOldDirty;
    //
    SAVectorDatas<T> *m_data;
    //索引，旧值，新值
    QVector<std::tuple<int,T,T> > m_replaceInfos;
    QVector<std::pair<int,T> > m_appendInfos;
    int m_oldDataSize;///< 记录原来的长度
    int m_newDataSize;///< 记录原来的长度
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
//////SAValueTableOptEditTableValueCommandPrivate/////////////////////////////////////////////////////////////////////////////
template<typename T>
SAValueTableOptEditTableValueCommandPrivate<T>::SAValueTableOptEditTableValueCommandPrivate(
        SAAbstractDatas *data
        , const QVariant &val
        , int startRow
        , int startCol)
    :SAValueTableOptEditValueCommandPrivateBase()
    ,m_isValid(false)
    ,m_data(nullptr)
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
    m_data->setTableData(m_startRow,m_startCol,m_newVal);
}
template<typename T>
void SAValueTableOptEditTableValueCommandPrivate<T>::undo()
{
    m_data->removeTableData(m_startRow,m_startCol);
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
        m_oldVal = m_data->getAt({(unsigned int)m_startRow,(unsigned int)m_startCol});
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
            m_oldVal.append(m_data->getAt({(unsigned int)m_startRow,(unsigned int)(m_startCol+i)}));
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



////////SAValueTableOptVectorPasteCommandPrivate/////////////////////////////////////////////////
template<typename T,typename FunMakeT>
SAValueTableOptVectorPasteCommandPrivate<T,FunMakeT>::SAValueTableOptVectorPasteCommandPrivate(
        SAAbstractDatas *data
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol
        , FunMakeT fp)
    :SAValueTableOptPasteCommandPrivateBase()
    ,m_startRow(startRow)
    ,m_startCol(startCol)
    ,m_funPtr(fp)
    ,m_isOldDirty(true)
    ,m_data(nullptr)
    ,m_oldDataSize(0)
    ,m_newDataSize(0)
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
        init(&clipboardTextTable);
        break;
    }
    default:
        break;
    }
}
template<typename T,typename FunMakeT>
void SAValueTableOptVectorPasteCommandPrivate<T,FunMakeT>::init(const QList<QVariantList> *clipboardTable)
{
    const int clipBoardRowCount = clipboardTable->size();
    int dim2 = m_data->getSize(SA::Dim2);
    const QVector<T>& datas = m_data->getValueDatas();
    m_newDataSize = m_oldDataSize = datas.size();
    //线提取旧的数据，生成新的数据
    for(int r=0;r<clipBoardRowCount;++r)
    {
        const QVariantList& rowValue = clipboardTable->at(r);
        const int realDataRowIndex = m_startRow + r;
        if(realDataRowIndex < datas.size())
        {
            //说明这是替换原来的数据
            if(rowValue.size() <= 0)
                continue;
            T val = datas.at(realDataRowIndex);
            for(int c=0;c<rowValue.size();++c)
            {
                int realDataCol = c + m_startCol;
                if(realDataCol < dim2)
                    m_funPtr(val,realDataCol,rowValue[c]);//此函数指针用来根据QVariant和对应的列设置T的值
            }
            m_replaceInfos.append(std::make_tuple(realDataRowIndex,datas[realDataRowIndex],val));
        }
        else
        {
            //说明插入新数据
            if(rowValue.size() > 0)
            {
                T val;
                for(int c=0;c<rowValue.size();++c)
                {
                    int realDataCol = c + m_startCol;
                    if(realDataCol < dim2)
                        m_funPtr(val,realDataCol,rowValue[c]);//此函数指针用来根据QVariant和对应的列设置T的值
                }
                m_appendInfos.append(std::make_pair(realDataRowIndex,val));
            }
            m_newDataSize = realDataRowIndex+1;
        }
    }
    m_isvalid = true;
    m_isOldDirty = m_data->isDirty();
}

template<typename T,typename FunMakeT>
void SAValueTableOptVectorPasteCommandPrivate<T,FunMakeT>::redo()
{
    QVector<T>& datas = m_data->getValueDatas();
    if(m_newDataSize > m_oldDataSize)
    {
        datas.resize(m_newDataSize);
        for(auto i = m_appendInfos.begin();i!=m_appendInfos.end();++i)
        {
            int index = std::get<0>((*i));
            if(index < datas.size())
                datas[index] = std::get<1>((*i));
        }
    }
    for(auto i = m_replaceInfos.begin();i!=m_replaceInfos.end();++i)
    {
        int index = std::get<0>((*i));
        if(index < datas.size())
            datas[index] = std::get<2>((*i));
    }
    m_data->setDirty(true);
}

template<typename T,typename FunMakeT>
void SAValueTableOptVectorPasteCommandPrivate<T,FunMakeT>::undo()
{
    QVector<T>& datas = m_data->getValueDatas();
    if(m_newDataSize > m_oldDataSize)
    {
        datas.resize(m_oldDataSize);
    }
    for(auto i = m_replaceInfos.begin();i!=m_replaceInfos.end();++i)
    {
        int index = std::get<0>((*i));
        if(index < datas.size())
            datas[index] = std::get<1>((*i));
    }
    m_data->setDirty(m_isOldDirty);
}
template<typename T,typename FunMakeT>
bool SAValueTableOptVectorPasteCommandPrivate<T,FunMakeT>::isValid() const
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
    m_deleteIndexs.reserve(indexs.size());
    getUniqueRows(indexs,std::back_inserter(m_deleteIndexs));
    m_deleteDatas.reserve(m_deleteIndexs.size());
    const int size = m_deleteIndexs.size();
    for(int i=0;i<size;++i)
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

////////////////////////////////////////////////////////////////////


SAValueTableOptPasteCommand::SAValueTableOptPasteCommand(
        SAAbstractDatas *data
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol
        , QUndoCommand *par)
    :SAAbstractValueTableOptCommand(data,par)
    ,d_ptr(nullptr)
{
    switch(data->getType())
    {
    case SA::VectorDouble:
    {
        auto fp = [](double& d,int col,const QVariant& val)
        {
            if(0 == col && val.isValid())
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d = v;
                }
            }
        };
        d_ptr = new SAValueTableOptVectorPasteCommandPrivate<double,decltype(fp)>(data
                                                                     ,clipboardTextTable
                                                                     ,startRow
                                                                     ,startCol
                                                                     ,fp);
        break;
    }
    case SA::VectorPoint://VectorPoint在显示上比较特殊
    {
        auto fp = [](QPointF& d,int col,const QVariant& val){
            if(0 == col && val.isValid())
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.rx() = v;
                }
            }
            else if (1 == col && val.isValid())
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.ry() = v;
                }
            }
        };
        d_ptr = new SAValueTableOptVectorPasteCommandPrivate<QPointF,decltype(fp)>(data
                                                                     ,clipboardTextTable
                                                                     ,startRow
                                                                     ,startCol
                                                                     ,fp);
        break;
    }
    case SA::VectorInterval:
    {
        auto fp = [](QwtIntervalSample& d,int col,const QVariant& val){
            switch(col)
            {
            case 0:
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.value = v;
                }
                break;
            }
            case 1:
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.interval.setMinValue(v);
                }
                break;
            }
            case 2:
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.interval.setMaxValue(v);
                }
                break;
            }
            }
        };
        d_ptr = new SAValueTableOptVectorPasteCommandPrivate<QwtIntervalSample,decltype(fp)>(data
                                                                     ,clipboardTextTable
                                                                     ,startRow
                                                                     ,startCol
                                                                     ,fp);
        break;
    }
    case SA::VectorOHLC:
    {
        auto fp = [](QwtOHLCSample& d,int col,const QVariant& val){
            switch(col)
            {
            case 0:
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.time = v;
                }
                break;
            }
            case 1:
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.open=v;
                }
                break;
            }
            case 2:
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.high=v;
                }
                break;
            }
            case 3:
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.low=v;
                }
                break;
            }
            case 4:
            {
                bool isOK = false;
                double v = val.toDouble(&isOK);
                if(isOK)
                {
                    d.close=v;
                }
                break;
            }
            }
        };
        d_ptr = new SAValueTableOptVectorPasteCommandPrivate<QwtOHLCSample,decltype(fp)>(data
                                                                     ,clipboardTextTable
                                                                     ,startRow
                                                                     ,startCol
                                                                     ,fp);
        break;
    }
    case SA::VectorVariant:
    {
        auto fp = [](QVariant& d,int col,const QVariant& val)
        {
            Q_UNUSED(col);
            d = val;
        };
        d_ptr = new SAValueTableOptVectorPasteCommandPrivate<QVariant,decltype(fp)>(data
                                                                     ,clipboardTextTable
                                                                     ,startRow
                                                                     ,startCol
                                                                     ,fp);
        break;
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
        d_ptr = new SAValueTableOptVectorDeleteCommandPrivate<QPoint>(data,deleteIndexs);
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
        break;
    }
    case SA::TableVariant:
    {
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



