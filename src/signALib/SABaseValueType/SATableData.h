#ifndef SATABLEDATA_H
#define SATABLEDATA_H

#include "SAAbstractDatas.h"
#include "SAHashTable.h"
#include "SADataHeader.h"
#include "SAVectorVariant.h"
#include "SAVectorDatas.h"
#include <QDebug>

#ifndef SA_TABLE_WRITE
#define SA_TABLE_WRITE(className) \
    virtual void write(QDataStream &out) const\
    {\
        SADataHeader type(this);\
        out << type;\
        SAAbstractDatas::write(out);\
        out << getTable();\
        m_isDirty = false;\
    }
#endif

template<typename T>
class SATableData : public SAAbstractDatas
{
public:
    typedef SAHashTable<T> Table;
    SATableData();
    SATableData(const QString& name);
    virtual ~SATableData();
    virtual int getType() const;
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const;
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
    int getSize(int dim=SA::Row) const;
    int getDim() const;
    virtual void read(QDataStream & in);
    //判断该数据在上次write之后是否内存有变更
    virtual bool isDirty() const;
    //设置内存有变更
    virtual void setDirty(bool dirty);
    //根据类型判断是否是数据,如nan就返回true，如空的一维数据都返回true
    virtual bool isEmpty() const;
    //设置数据
    virtual bool setAt(const QVariant& val,const std::initializer_list<size_t>& index);
public:
    bool isHaveData(uint r,uint c) const;
    void setTableData(uint row,uint col,const T& d);
    const T getValue(uint r,uint c) const;
    T& getValue(uint r,uint c);
    //删除一个数据
    void removeTableData(uint r,uint c);
    int rowCount() const;
    int columnCount() const{return m_table.columnCount ();}
    //get
    const Table& getTable() const{return m_table;}
    Table& getTable(){return m_table;}
    void setTable(const Table& t){m_table = t;}
    //获取表格的尺寸
    QSize getTableSize() const;
    //清除所有数据
    void clear();
    //获取一列数据
    bool getColumnDatas(size_t col,SAVectorVariant* res);
    //获取一行数据
    bool getRowDatas(size_t row, SAVectorVariant* res);
#if 0
    SATableData& operator =(const SATableData* data);
#endif
private:
    Table m_table;
protected:
    mutable bool m_isDirty;
};



template<typename T>
SATableData<T>::SATableData():SAAbstractDatas()
  ,m_isDirty(true)
{

}
template<typename T>
SATableData<T>::SATableData(const QString &name):SAAbstractDatas(name)
  ,m_isDirty(true)
{

}


template<typename T>
SATableData<T>::~SATableData()
{

}

template<typename T>
int SATableData<T>::getType() const
{
    return SA::UnknowType;
}

template<typename T>
QVariant SATableData<T>::getAt(const std::initializer_list<size_t> &index) const
{
    if(2 == index.size())
        return QVariant::fromValue<T>(getValue (*index.begin(),*(index.begin()+1)));
    return QVariant();
}

template<typename T>
QString SATableData<T>::displayAt(const std::initializer_list<size_t> &index) const
{
    return getAt(index).toString();
}
///
/// \brief 表格的size
/// \param sizeType SA_SIZE_TYPE_ROW时获取的是行数，SA_SIZE_TYPE_COLUMN为获取列数
/// \return 行数
///
template<typename T>
int SATableData<T>::getSize(int dim) const
{
    if(SA::Row == dim)
        return m_table.rowCount ();
    else if(SA::Column == dim)
        return m_table.columnCount ();
    return 0;
}

template<typename T>
int SATableData<T>::getDim() const
{
    return SA::Dim2;
}
///
/// \brief 表格对应的行列是否有内容
/// \param r 行索引
/// \param c 列索引
/// \return 存在值返回true
///
template<typename T>
bool SATableData<T>::isHaveData(uint r, uint c) const
{
    return m_table.isHaveData (r,c);
}
///
/// \brief 设置表格数据
/// \param row 行索引
/// \param col 列索引
/// \param d 表格内容
///
template<typename T>
void SATableData<T>::setTableData(uint row, uint col, const T &d)
{
    m_table.setData (row,col,d);
    setDirty(true);
}
///
/// \brief 获取表格数据
/// \param r 行索引
/// \param c 列索引
/// \return 数据
///
template<typename T>
const T SATableData<T>::getValue(uint r, uint c) const
{
    return m_table.at (r,c);
}
///
/// \brief 获取表格数据
/// \param r 行索引
/// \param c 列索引
/// \return
///
template<typename T>
T &SATableData<T>::getValue(uint r, uint c)
{
    return m_table.at (r,c);
}
///
/// \brief 删除一个数据
/// \param r
/// \param c
///
template<typename T>
void SATableData<T>::removeTableData(uint r, uint c)
{
    m_table.removeData(r,c);
}
///
/// \brief 行数
/// \return
///
template<typename T>
int SATableData<T>::rowCount() const
{
    return m_table.rowCount ();
}
///
/// \brief 返回表格的尺寸
/// \return
///
template<typename T>
QSize SATableData<T>::getTableSize() const
{
    return QSize(m_table.rowCount (),m_table.columnCount ());
}
template<typename T>
void SATableData<T>::read(QDataStream &in)
{
    SAAbstractDatas::read(in);
    in >> m_table;
    setDirty(false);
}
template<typename T>
void SATableData<T>::clear()
{
    m_table.clear();
    setDirty(true);
}

template<typename T>
bool SATableData<T>::getColumnDatas(size_t col, SAVectorVariant *res)
{
    int row = rowCount();
    if(col >= columnCount())
    {
        return false;
    }
    for(int i=0;i<row;++i)
    {
        res->append(m_table.at(i,col));
    }
    return true;
}
template<typename T>
bool SATableData<T>::getRowDatas(size_t row, SAVectorVariant *res)
{
    int col = columnCount();
    if(row >= rowCount())
    {
        return false;
    }
    for(int i=0;i<col;++i)
    {
        res->append(m_table.at(row,i));
    }
    return true;
}
template<typename T>
bool SATableData<T>::isDirty() const
{
    return m_isDirty;
}
template<typename T>
void SATableData<T>::setDirty(bool dirty)
{
    m_isDirty = dirty;
}
template<typename T>
bool SATableData<T>::isEmpty() const
{
    return ((0 == m_table.columnCount()) && (0 == m_table.rowCount()));
}

template<typename T>
bool SATableData<T>::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    if(index.size() < 2)
    {
        return false;
    }
    for(auto i=index.begin()+2;i!=index.end();++i)
    {
        if(0 != *i)
            return false;
    }
    if(!val.canConvert<T>())
    {
        qDebug() << "SATableData<T> setAt: variant can not cover type";
        return false;
    }
    int r = *(index.begin());
    int c = *(index.begin()+1);
    setTableData(r,c,val.value<T>());
    setDirty(true);
    return true;
}
#if 0
template<typename T>
SATableData<T> &SATableData<T>::operator =(const SATableData *data)
{
    this->m_table = data->m_table;
}
#endif






#endif // SATABLEDATA_H


