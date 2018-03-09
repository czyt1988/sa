#ifndef SAVECTORDATAS_H
#define SAVECTORDATAS_H

#include "SAAbstractDatas.h"
#include <QVector>
///
/// \brief sa的vector变量接口
/// \author czy -> czy.t@163.com
/// \date
///
template<typename T>
class SALIB_EXPORT SAVectorDatas : public SAAbstractDatas
{
public:
    SAVectorDatas();
    SAVectorDatas(const QString & name);
    SAVectorDatas(const QString& name,const QVector<T>& datas);
    SAVectorDatas(const QVector<T>& datas);
    virtual ~SAVectorDatas();

    int getSize(int dim=SA::Dim1) const;
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const;
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
    int getDim() const;
    //判断该数据在上次write之后是否内存有变更
    virtual void read(QDataStream & in);
    virtual bool isDirty() const;
    //设置内存有变更
    virtual void setDirty(bool dirty);
    //根据类型判断是否是数据,如nan就返回true，如空的一维数据都返回true
    virtual bool isEmpty() const;
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    //设置数据
    void setValueDatas(const QVector<T>& datas);

    template<typename IT>
    void setValueDatas(IT begin,IT end)
    {
        m_datas.resize(std::distance(begin,end));
        std::copy(begin,end,m_datas.begin());
        setDirty(true);
    }

    const QVector<T>& getValueDatas() const;
    QVector<T>& getValueDatas();
    void getValueDatas(QVector<T>& dataBeGet) const;
    void getValueDatas (QVector<T>& dataBeGet,const QVector<int>& index) const;
    T getValue(int index) const;
    T& get(int index);
    const T& get(int index) const;
    void set(size_t index,const T& value);
    void append(const T& value);
    void resize(int size);
    void reserve(int size);
    void push_back(const T& value);

    typename QVector<T>::iterator begin();
    typename QVector<T>::iterator end();
    typename QVector<T>::const_iterator cbegin() const;
    typename QVector<T>::const_iterator cend() const;
    T &operator[](int i);
    const T &operator[](int i) const;
    void clear();

    bool setAt(const QVariant &val, const std::initializer_list<size_t> &index);
protected:
    QVector<T> m_datas;
    mutable bool m_isDirty;
};





template<typename T>
SAVectorDatas<T>::SAVectorDatas():SAAbstractDatas()
{

}

template<typename T>
SAVectorDatas<T>::SAVectorDatas(const QString &name):SAAbstractDatas(name)
{

}
template<typename T>
SAVectorDatas<T>::SAVectorDatas(const QString &name, const QVector<T> &datas):SAAbstractDatas(name)
{
    setValueDatas(datas);
}
template<typename T>
SAVectorDatas<T>::SAVectorDatas(const QVector<T> &datas)
{
    setValueDatas(datas);
}
template<typename T>
SAVectorDatas<T>::~SAVectorDatas()
{

}
///
/// \brief 设置数据
/// \param datas
///
template<typename T>
void SAVectorDatas<T>::setValueDatas(const QVector<T> &datas)
{
    this->m_datas = std::move(datas);
    setDirty(true);
}

template<typename T>
const QVector<T> &SAVectorDatas<T>::getValueDatas() const
{
    return this->m_datas;
}
template<typename T>
QVector<T> &SAVectorDatas<T>::getValueDatas()
{
    return this->m_datas;
}
template<typename T>
void SAVectorDatas<T>::getValueDatas(QVector<T> &dataBeGet) const
{
    dataBeGet = std::move(m_datas);
}
///
/// \brief 获取值
/// \param dataBeGet
/// \param index 需要获取的索引
///
template<typename T>
void SAVectorDatas<T>::getValueDatas(QVector<T> &dataBeGet, const QVector<int> &index) const
{
    dataBeGet.reserve (index.size ());
    auto end = index.end ();
    for(auto i=index.begin ();i!=end;++i)
    {
        dataBeGet.push_back (m_datas[*i]);
    }
}
///
/// \brief 线性数组，只返回1维的数据，其他维度返回0
/// \param dim 维度
/// \return 其他维度都是1
///
template<typename T>
int SAVectorDatas<T>::getSize(int dim) const {
    if(dim==SA::Dim1)
    {
        return this->m_datas.size();
    }
    return m_datas.isEmpty() ? 0 : 1;
}

template<typename T>
QVariant SAVectorDatas<T>::getAt(const std::initializer_list<size_t> &index) const
{
    if(0 == index.size())
        return QVariant();
    if(1 == index.size())
        return QVariant::fromValue<T>(get(*index.begin()));
    for(auto i=(index.begin()+1);i!=index.end();++i)
    {
        if(0!=*i)
        {
            return QVariant();
        }
    }
    return QVariant::fromValue<T>(get(*index.begin()));
}

template<typename T>
QString SAVectorDatas<T>::displayAt(const std::initializer_list<size_t> &index) const
{
    return getAt(index).toString();
}

template<typename T>
int SAVectorDatas<T>::getDim() const
{
    return SA::Dim1;
}
template<typename T>
bool SAVectorDatas<T>::isDirty() const
{
    return m_isDirty;
}

template<typename T>
void SAVectorDatas<T>::setDirty(bool dirty)
{
    m_isDirty = dirty;
}
template<typename T>
bool SAVectorDatas<T>::isEmpty() const
{
    return (0 == m_datas.size());
}

template<typename T>
T SAVectorDatas<T>::getValue(int index) const
{
    return m_datas[index];
}

template<typename T>
T &SAVectorDatas<T>::get(int index)
{
    return m_datas[index];
}

template<typename T>
const T &SAVectorDatas<T>::get(int index) const
{
    return m_datas[index];
}

template<typename T>
void SAVectorDatas<T>::set(size_t index, const T &value)
{
    m_datas[index] = value;
    setDirty(true);
}
template<typename T>
void SAVectorDatas<T>::append(const T &value)
{
    m_datas.append (value);
    setDirty(true);
}
template<typename T>
void SAVectorDatas<T>::resize(int size)
{
    m_datas.resize (size);
    setDirty(true);
}

template<typename T>
void SAVectorDatas<T>::reserve(int size)
{
    m_datas.reserve (size);
}

template<typename T>
void SAVectorDatas<T>::push_back(const T &value)
{
    m_datas.push_back (value);
    setDirty(true);
}
///
/// \brief 写文件时，会额外加一个SADataTypeInfo，用于判断类型，在读取时，是不会读取这个SADataTypeInfo的，因为读取时需要
/// 先读取SADataTypeInfo，根据信息再来进行其它操作
/// \param in
///
template<typename T>
void SAVectorDatas<T>::read(QDataStream &in)
{
    SAAbstractDatas::read(in);
    in >> getValueDatas();
    setDirty(false);
}

template<typename T>
typename QVector<T>::iterator SAVectorDatas<T>::begin()
{
    return m_datas.begin();
}

template<typename T>
typename QVector<T>::iterator SAVectorDatas<T>::end()
{
    return m_datas.end();
}

template<typename T>
typename QVector<T>::const_iterator SAVectorDatas<T>::cbegin() const
{
    return m_datas.cbegin();
}

template<typename T>
typename QVector<T>::const_iterator SAVectorDatas<T>::cend() const
{
    return m_datas.cend();
}

template<typename T>
T &SAVectorDatas<T>::operator[](int i)
{
    return m_datas[i];
}

template<typename T>
const T &SAVectorDatas<T>::operator[](int i) const
{
    return m_datas[i];
}

template<typename T>
void SAVectorDatas<T>::clear()
{
    m_datas.clear();
    setDirty(true);
}

template<typename T>
bool SAVectorDatas<T>::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    const int dimsize = index.size();
    if(dimsize > 2)
    {
        return false;
    }
    if(2 == dimsize)
    {
        if(0 != *(index.begin()+1))
        {
            return false;
        }
    }
    int r = *(index.begin());
    if(r < 0 || r >  getValueDatas().size())
    {
        return false;
    }
    if(val.canConvert<T>())
    {
        T v = val.value<T>();
        if(r != getValueDatas().size())
        {
            getValueDatas()[r] = v;
        }
        else
        {
            getValueDatas().append(v);
        }
        setDirty(true);
        return true;
    }
    return false;
}




#endif // SAVECTORDATAS_H
