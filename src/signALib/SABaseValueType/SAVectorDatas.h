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
    SAVectorDatas():SAAbstractDatas(){}
    SAVectorDatas(const QString & name):SAAbstractDatas(name){}
    SAVectorDatas(const QString& name,const QVector<T>& datas):SAAbstractDatas(name)
    {
        setValueDatas(datas);
    }
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    virtual ~SAVectorDatas(){}
    ///
    /// \brief 设置数据
    /// \param datas
    ///
    void setValueDatas(const QVector<T>& datas)    {this->m_datas = std::move(datas);}
    template<typename IT>
    void setValueDatas(IT begin,IT end)
    {
        m_datas.resize(std::distance(begin,end));
        std::copy(begin,end,m_datas.begin());
    }
    const QVector<T>& getValueDatas() const         {return this->m_datas;}
    QVector<T>& getValueDatas()         {return this->m_datas;}
    void getValueDatas(QVector<T>& dataBeGet) const {dataBeGet = std::move(m_datas);}
    ///
    /// \brief 获取值
    /// \param dataBeGet
    /// \param index 需要获取的索引
    ///
    void getValueDatas (QVector<T>& dataBeGet,const QVector<int>& index) const
    {
        dataBeGet.reserve (index.size ());
        auto end = index.end ();
        for(auto i=index.begin ();i!=end;++i)
        {
            dataBeGet.push_back (m_datas[*i]);
        }
    }

    virtual int getType() const   {return SA::UnknowType;}
    ///
    /// \brief 线性数组，只返回1维的数据，其他维度返回0
    /// \param dim 维度
    /// \return 其他维度都是1
    ///
    int getSize(int dim=SA::Dim1) const {
        if(dim==SA::Dim1)
        {
            return this->m_datas.size();
        }
        else if(dim==SA::Dim2)
        {
            return 1;
        }
        return 0;
    }
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const
    {
        if(1 == index.size())
            return QVariant::fromValue<T>(get(*index.begin()));
        return QVariant();
    }
    virtual QString displayAt(const std::initializer_list<size_t>& index) const
    {
        return getAt(index).toString();
    }
    int getDim() const{return 1;}

    T getValue(int index) const     {return m_datas[index];}
    T& get(int index) {return m_datas[index];}
    const T& get(int index) const {return m_datas[index];}
    void set(size_t index,const T& value){m_datas[index] = value;}
    void append(const T& value){m_datas.append (value);}
    void resize(int size){m_datas.resize (size);}
    void reserve(int size){m_datas.reserve (size);}
    void push_back(const T& value){m_datas.push_back (value);}
    ///
    /// \brief 写文件时，会额外加一个SADataTypeInfo，用于判断类型，在读取时，是不会读取这个SADataTypeInfo的，因为读取时需要
    /// 先读取SADataTypeInfo，根据信息再来进行其它操作
    /// \param in
    ///
    virtual void read(QDataStream & in){
        SAAbstractDatas::read(in);
        in >> getValueDatas();
    }
    typename QVector<T>::iterator begin() {
        return m_datas.begin();
    }
    typename QVector<T>::iterator end() {
        return m_datas.end();
    }
    typename QVector<T>::const_iterator cbegin() const{
        return m_datas.cbegin();
    }
    typename QVector<T>::const_iterator cend() const{
        return m_datas.cend();
    }
    //
    T &operator[](int i)
    {
        return m_datas[i];
    }

    const T &operator[](int i) const
    {
        return m_datas[i];
    }
    void clear()
    {
        m_datas.clear();
    }

protected:
    QVector<T> m_datas;
};

#endif // SAVECTORDATAS_H



