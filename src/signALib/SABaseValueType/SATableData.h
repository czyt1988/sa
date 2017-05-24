#ifndef SATABLEDATA_H
#define SATABLEDATA_H

#include "SAAbstractDatas.h"
#include "czyQtApp.h"
#include "SADataHeader.h"
#include "SAVectorVariant.h"
#include "SAVectorDatas.h"
#ifndef SA_TABLE_WRITE
#define SA_TABLE_WRITE(className) \
    virtual void write(QDataStream &out) const\
    {\
        SADataHeader type(this);\
        out << type;\
        SAAbstractDatas::write(out);\
        out << getTable();\
    }
#endif

template<typename T>
class SALIB_EXPORT SATableData : public SAAbstractDatas
{
public:
    typedef czy::QtApp::QczyHashTable<T> Table;
    SATableData(){}
    SATableData(const QString& name):SAAbstractDatas(name){}
    SATableData(const QIcon & icon, const QString & name):SAAbstractDatas(icon,name){}
    virtual ~SATableData(){}
    virtual int getType() const   {return SA::UnknowType;}
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const
    {
        if(2 == index.size())
            return QVariant::fromValue<T>(getValue (*index.begin(),*(index.begin()+1)));
        return QVariant();
    }
    virtual QString displayAt(const std::initializer_list<size_t>& index) const
    {
        return getAt(index).toString();
    }
    ///
    /// \brief 表格的size
    /// \param sizeType SA_SIZE_TYPE_ROW时获取的是行数，SA_SIZE_TYPE_COLUMN为获取列数
    /// \return 行数
    ///
    int getSize(int dim=SA::Row) const{
        if(SA::Row == dim)
            return m_table.rowCount ();
        else if(SA::Column == dim)
            return m_table.columnCount ();
        return 0;
    }
    int getDim() const{return 2;}
    bool isHaveData(uint r,uint c) const{return m_table.isHaveData (r,c);}
    void setTableData(uint row,uint col,const T& d){m_table.setData (row,col,d);}
    const T getValue(uint r,uint c) const
    {
        return m_table.at (r,c);
    }
    T& getValue(uint r,uint c){return m_table.at (r,c);}

    int rowCount() const{return m_table.rowCount ();}
    int columnCount() const{return m_table.columnCount ();}

    Table& getTable(){return m_table;}
    const Table& getTable() const{return m_table;}
    void setTable(const Table& t){m_table = t;}
    QSize getTableSize() const{return QSize(m_table.rowCount (),m_table.columnCount ());}
    virtual void read(QDataStream & in){
        SAAbstractDatas::read(in);
        in >> m_table;
    }
    void clear()
    {
        m_table.clear();
    }

    //获取一列数据
    bool getColumnDatas(size_t col,SAVectorVariant* res);


    //获取一行数据
    bool getRowDatas(size_t row, SAVectorVariant* res);
    //
#if 0
    SATableData& operator =(const SATableData* data);
#endif
private:
    Table m_table;
};

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
}
#if 0
template<typename T>
SATableData<T> &SATableData<T>::operator =(const SATableData *data)
{
    this->m_table = data->m_table;
}
#endif






#endif // SATABLEDATA_H


