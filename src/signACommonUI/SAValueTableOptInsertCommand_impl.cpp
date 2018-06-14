#include "SAValueTableOptCommands.h"
#include "SAData.h"


///
/// \brief The SAValueTableOptInsertCommand的内部处理类，会根据不一样的情况实例化不同的内容
///
class SAValueTableOptInsertCommandPrivateBase : public SAAbstractValueTableOptCommandPrivate
{
public:
    SAValueTableOptInsertCommandPrivateBase():SAAbstractValueTableOptCommandPrivate(){}
    virtual ~SAValueTableOptInsertCommandPrivateBase(){}
    virtual bool isValid() const = 0;
    static void indexRange(const QVector<QPoint>& p,int *pRowMin = nullptr,int *pRowMax = nullptr,int *pColMin = nullptr,int *pColMax = nullptr)
    {
        if(p.isEmpty())
            return;
        int xmin,xmax=0,ymin=0,ymax=0;
        xmin = xmax = p[0].x();
        ymin = ymax = p[0].y();
        for(int i=1;i<p.size();++i)
        {
            if(p[i].x() < xmin)
                xmin = p[i].x();
            else if(p[i].x() > xmax)
                xmax = p[i].x();

            if(p[i].y() < ymin)
                ymin = p[i].y();
            else if(p[i].y() > ymax)
                ymax = p[i].y();
        }
        if(pRowMin)
            *pRowMin = ymin;
        if(pRowMax)
            *pRowMax = ymax;
        if(pColMin)
            *pColMin = xmin;
        if(pColMax)
            *pColMax = xmax;
    }
};



///////insert相关////////////////////////////////////////////////////

template<typename T>
class SAValueTableOptVectorInsertCommandPrivate : public SAValueTableOptInsertCommandPrivateBase
{
public:
    SAValueTableOptVectorInsertCommandPrivate(SAAbstractDatas* data
                                              ,const QVector<QPoint>& indexs);
    bool isValid() const;
    void init(const QVector<QPoint>& indexs);
    virtual void redo();
    virtual void undo();
private:
    SAVectorDatas<T>* m_data;
    bool m_isValid;
    bool m_isOldDirty;
    int m_startInsertRow;///< 从此行开始向上插入
};

//////////////////////////////////////////////////////////////////////////

template<typename T>
SAValueTableOptVectorInsertCommandPrivate<T>::SAValueTableOptVectorInsertCommandPrivate(
        SAAbstractDatas *data
        , const QVector<QPoint> &indexs)
    :SAValueTableOptInsertCommandPrivateBase()
    ,m_isValid(false)
    ,m_isOldDirty(false)
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
bool SAValueTableOptVectorInsertCommandPrivate<T>::isValid() const
{
    return m_isValid;
}
template<typename T>
void SAValueTableOptVectorInsertCommandPrivate<T>::init(const QVector<QPoint> &indexs)
{
    indexRange(indexs,&m_startInsertRow);
    if(m_startInsertRow<0 || m_startInsertRow>= m_data->getValueDatas().size())
    {
        return;
    }
    m_isOldDirty = m_data->isDirty();
    m_isValid = true;
}
template<typename T>
void SAValueTableOptVectorInsertCommandPrivate<T>::redo()
{
    QVector<T>& raw = m_data->getValueDatas();
    raw.insert(m_startInsertRow,T());
    m_data->setDirty(true);
}
template<typename T>
void SAValueTableOptVectorInsertCommandPrivate<T>::undo()
{
    QVector<T>& raw = m_data->getValueDatas();
    raw.remove(m_startInsertRow);
    m_data->setDirty(m_isOldDirty);
}



//////////////////////////////////////////////////////////
//SAValueTableOptInsertCommand
///////////////////////////////////////////////////////////

SAValueTableOptInsertCommand::SAValueTableOptInsertCommand(
        SAAbstractDatas *data
        , const QVector<QPoint> &selectIndex
        , QUndoCommand *par)
    :SAAbstractValueTableOptCommand(data,par)
    ,d_ptr(nullptr)
{
    switch(data->getType())
    {
    case SA::VectorInt:
        d_ptr = new SAValueTableOptVectorInsertCommandPrivate<int>(data,selectIndex);
        break;
    case SA::VectorDouble:
        d_ptr = new SAValueTableOptVectorInsertCommandPrivate<double>(data,selectIndex);
        break;
    case SA::VectorInterval:
        d_ptr = new SAValueTableOptVectorInsertCommandPrivate<QwtIntervalSample>(data,selectIndex);
        break;
    case SA::VectorOHLC:
        d_ptr = new SAValueTableOptVectorInsertCommandPrivate<QwtOHLCSample>(data,selectIndex);
        break;
    case SA::VectorPoint:
        d_ptr = new SAValueTableOptVectorInsertCommandPrivate<QPointF>(data,selectIndex);
        break;
    case SA::VectorVariant:
        d_ptr = new SAValueTableOptVectorInsertCommandPrivate<QVariant>(data,selectIndex);
        break;
    case SA::TableDouble:
        break;
    case SA::TableVariant:
        break;
    default:
        break;
    }
}

SAValueTableOptInsertCommand::~SAValueTableOptInsertCommand()
{
    if(d_ptr)
    {
        delete d_ptr;
    }
}

bool SAValueTableOptInsertCommand::isValid() const
{
    if(d_ptr)
    {
        return d_ptr->isValid();
    }
    return false;
}

void SAValueTableOptInsertCommand::redo()
{
    if(d_ptr)
    {
        return d_ptr->redo();
    }
}

void SAValueTableOptInsertCommand::undo()
{
    if(d_ptr)
    {
        return d_ptr->undo();
    }
}


