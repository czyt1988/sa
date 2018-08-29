#include "SAValueTableOptCommands.h"
#include "SAData.h"
#include <iterator>
#include "czyAlgorithm.h"
///
/// \brief SAValueTableOptPasteCommand的内部处理类，会根据参数类型不一样实例化不一样的内容
///
class SAAbstractValueTableOptPasteCommandPrivate : public SAAbstractValueTableOptCommandPrivate
{
public:
    SAAbstractValueTableOptPasteCommandPrivate():SAAbstractValueTableOptCommandPrivate(){}
    virtual ~SAAbstractValueTableOptPasteCommandPrivate(){}
    virtual bool isValid() const = 0;
    static QSize getClipboardTableSize(const QList<QVariantList>& clipboardTable);
};

///
/// \brief The SAValueTableOptVectorPasteCommandPrivate class
///
/// 处理Vector的复制粘贴
///
template<typename T,typename FunMakeT>
class SAValueTableOptVectorPasteCommandPrivate : public SAAbstractValueTableOptPasteCommandPrivate
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
class SAValueTableOptTablePasteCommandPrivate : public SAAbstractValueTableOptPasteCommandPrivate
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

//////SAValueTableOptPasteCommandPrivateBase///////////////////////////////////////////////////////////

QSize SAAbstractValueTableOptPasteCommandPrivate::getClipboardTableSize(const QList<QVariantList> &clipboardTable)
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

////////SAValueTableOptVectorPasteCommandPrivate/////////////////////////////////////////////////


template<typename T,typename FunMakeT>
SAValueTableOptVectorPasteCommandPrivate<T,FunMakeT>::SAValueTableOptVectorPasteCommandPrivate(
        SAAbstractDatas *data
        , const QList<QVariantList> &clipboardTextTable
        , int startRow
        , int startCol
        , FunMakeT fp)
    :SAAbstractValueTableOptPasteCommandPrivate()
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
    qDebug() << "SAValueTableOptVectorPasteCommandPrivate init";
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
            //此句在if(rowValue.size() <= 0)之前的意义是允许插入空行，空行会以默认数据替代
            m_newDataSize = realDataRowIndex+1;
qDebug() << m_newDataSize;
            if(rowValue.size() <= 0)
                continue;
            T val;
            for(int c=0;c<rowValue.size();++c)
            {
                int realDataCol = c + m_startCol;
                if(realDataCol < dim2)
                    m_funPtr(val,realDataCol,rowValue[c]);//此函数指针用来根据QVariant和对应的列设置T的值
            }
            m_appendInfos.append(std::make_pair(realDataRowIndex,val));
        }
    }
    m_isvalid = true;
    m_isOldDirty = m_data->isDirty();
}

template<typename T,typename FunMakeT>
void SAValueTableOptVectorPasteCommandPrivate<T,FunMakeT>::redo()
{
    if(!isValid())
        return;
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
    if(!isValid())
        return;
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
    :SAAbstractValueTableOptPasteCommandPrivate()
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
    if(!isValid())
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
    if(!isValid())
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


//////////////////////////////////////////////////

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


