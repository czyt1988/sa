#ifndef SAHASHTABLE_H
#define SAHASHTABLE_H
#include <QString>
#include <QVector>
#include <QHash>
#include <QPair>
#include <QDataStream>
template <typename T>
class SAArrayTable
{
public:
    typedef QVector<T> QTableRow;
    SAArrayTable():m_columns(0){}
    SAArrayTable(int rows,int columns);
    const T& at(int r,int c) const;
    T& at(int r,int c);
    bool appendRow(const QVector<T>& row);
    ///
    /// \brief 表的行数
    /// \return
    ///
    int rowCount() const;
    ///
    /// \brief 表的列数
    /// \return
    ///
    int columnCount() const;

private:
    QVector< QVector<T> > m_d;
    size_t m_columns;
};

template <typename T>
SAArrayTable<T>::SAArrayTable(int rows, int columns)
{
    m_d.clear ();
    m_d.reserve (rows);
    for(int i=0;i<rows;++i)
    {
        m_d.push_back (QVector<T>(columns));
    }
    m_columns = columns;
}

template <typename T>
const T &SAArrayTable<T>::at(int r, int c) const
{
    return m_d.at (r).at (c);
}

template <typename T>
T &SAArrayTable<T>::at(int r, int c)
{
    return m_d[r][c];
}

template <typename T>
bool SAArrayTable<T>::appendRow(const QVector<T> &row)
{
    size_t s = row.size ();
    bool isSuccess = false;
    if(s == m_columns || 0 == m_columns)
    {
        m_d.push_back (row);
        m_columns = s;
        isSuccess = true;
    }
    else if(s < m_columns)
    {//在结尾补充
        QVector<T> newRow(m_columns);
        std::copy(row.begin (),row.end (),newRow.begin ());
        m_d.push_back (newRow);
    }
    else
    {//s>m_columns
        size_t d = s - m_columns;
        for(auto i=m_d.begin ();i!=m_d.end ();++i)
        {
            i->insert(i->end(),d,T());
        }
        m_d.push_back (row);
        m_columns = s;
    }
    return isSuccess;
}

template <typename T>
int SAArrayTable<T>::rowCount() const
{
    return m_d.size ();
}

template <typename T>
int SAArrayTable<T>::columnCount() const
{
    return m_columns;
}




///
/// \brief 根据QHash来作为存储的数据结构
///
/// QHashTable是一个离散的表，不用预先分配好行和列。和QTable不同，QTable需要固定的行列，每个行列里理论上
/// 都需要有东西，即使没有东西，其模板参数也需要返回一个默认构造。
///
/// QHashTable的速度会稍微逊色于QTable，如果模板参数T()有默认构造参数，在获取值时不需要使用isHaveData函数进行判断是否存在内容
///
template <typename T>
class SAHashTable
{
public:
    typedef uint Index;
    typedef QHash<QPair<Index,Index>,T> Table;
public:
    SAHashTable():m_columns(0),m_rows(0){}
    ~SAHashTable(){}
    ///
    /// \brief 判断在某行某列里是否存在内容
    /// \param r 行
    /// \param c 列
    /// \return 如果有，返回true
    ///
    bool isHaveData(Index r,Index c) const;
    ///
    /// \brief 给某行某列设置内容
    /// \param row 行
    /// \param col 列
    /// \param d 数据内容
    ///
    void setData(Index row,Index col,const T& d);
    ///
    /// \brief 移除数据
    /// \param row 移除的行
    /// \param col 移除的列
    ///
    void removeData(Index row,Index col);

    ///
    /// \brief at 获取某行某列的内容
    /// \param r 行
    /// \param c 列
    /// \return 返回内容
    ///
    const T at(Index r,Index c) const;
    ///
    /// \brief at 获取某行某列内容的引用
    /// \param r 行
    /// \param c 列
    /// \return  引用
    ///
    T& at(Index r,Index c);
    ///
    /// \brief 表的行数
    /// \return
    ///
    Index rowCount() const{return m_rows;}
    ///
    /// \brief 表的列数
    /// \return
    ///
    Index columnCount() const{return m_columns;}
    ///
    /// \brief 插入一行
    /// \return 返回插入的行数
    ///
    Index appendRow(const QList<T>& rows);
    ///
    /// \brief 在给定的位置插入一行
    /// \param row 给定的行
    /// \param col 给定的列
    /// \param rows 返回最终插入的列索引
    /// \return
    ///
    Index setRowDatas(Index row,Index col, const QList<T>& rows);
    ///
    /// \brief 在指定的位置插入一列
    /// \param row
    /// \param col
    /// \param cols
    /// \return
    ///
    Index setColumnDatas(Index row,Index col, const QList<T>& cols);
    ///
    /// \brief 插入一列
    /// \return 返回插入的列数
    ///
    Index appendColumn(const QList<T>& cols);
    ///
    /// \brief 清空内容
    ///
    void clear();
    ///
    /// \brief 提取列
    /// \param index 列索引
    /// \param ifGetNullData 对于表格里没有的数据也或取，这样list的长度和行数一致，且效率更高
    /// 如果为false会先判断是否存在值，不存在就会接着获取下一个，获取的都是当前列有用的数据
    /// \return 列
    ///
    QList<T> getColumn(Index index,bool ifGetNullData = false) const;
    ///
    /// \brief getRow
    /// \param index
    /// \param ifGetNullData
    /// \return
    ///
    QList<T> getRow(Index index,bool ifGetNullData = false) const;
    Table& data(){return m_data;}
    const Table& data() const{return m_data;}
    ///
    /// \brief 设置所有内容
    /// \param datas Table
    ///
    void setDatas(const Table& datas);

//            friend QDataStream &operator<<(QDataStream & out, const Table & item);
//            friend QDataStream &operator>>(QDataStream & in, Table & item);

private:
    Table m_data;//表格的数据存放
    Index m_columns;
    Index m_rows;
};

template <typename T>
bool SAHashTable<T>::isHaveData(typename SAHashTable<T>::Index r,typename SAHashTable<T>::Index c) const
{
    return (m_data.find(qMakePair(r,c)) != m_data.end ());
}

template <typename T>
void SAHashTable<T>::setData(typename SAHashTable<T>::Index row,typename SAHashTable<T>::Index col, const T &d)
{
    if(col >= m_columns )
        m_columns = (col+1);
    if(row >= m_rows)
        m_rows = (row+1);
    m_data.insert (qMakePair(row,col),d);
}

template <typename T>
void SAHashTable<T>::removeData(typename SAHashTable<T>::Index row,typename SAHashTable<T>::Index col)
{
    int i = m_data.remove(qMakePair(row,col));
    if(i <= 0)
        return;
    if(col == m_columns || row == m_rows)
    {
        //说明需要更新
        typename Table::const_iterator i = m_data.begin();
        typename Table::const_iterator end = m_data.end();
        Index maxRow(0),maxCol(0);
        if(i == end)
            return;
        while(i != end)
        {
            if( maxRow  < i.key().first )
                maxRow = i.key().first;
            if(maxCol < i.key().second)
                maxCol = i.key().second;
            ++i;
        }
    }
}

template <typename T>
const T SAHashTable<T>::at(typename SAHashTable<T>::Index r,typename SAHashTable<T>::Index c) const
{
    return m_data[qMakePair(r,c)];
}

template <typename T>
T &SAHashTable<T>::at(typename SAHashTable<T>::Index r,typename SAHashTable<T>::Index c)
{
    return m_data[qMakePair(r,c)];
}

template <typename T>
typename SAHashTable<T>::Index SAHashTable<T>::appendRow(const QList<T> &rows)
{
    int rowIndex = m_rows;
    ++m_rows;
    int size = rows.size ();
    if(size > m_columns )
        m_columns = size;
    for(int i=0;i<size;++i)
    {
        m_data.insert (qMakePair(rowIndex,i),rows[i]);
    }
    return m_rows;
}

template <typename T>
typename SAHashTable<T>::Index SAHashTable<T>::setRowDatas(typename SAHashTable<T>::Index row,typename SAHashTable<T>::Index col, const QList<T> &rows)
{
    int size = rows.size ();
    Index endCol = col + size-1;
    if(row >= m_rows )
        m_rows = (row+1);
    if(endCol+1 > m_columns )
        m_columns = endCol+1;
    for(int i=0;i<size;++i)
    {
        m_data.insert (qMakePair(row,i+col),rows[i]);
    }
    return endCol;
}

template <typename T>
typename SAHashTable<T>::Index SAHashTable<T>::setColumnDatas(typename SAHashTable<T>::Index row,typename SAHashTable<T>::Index col, const QList<T> &cols)
{
    int size = cols.size ();
    Index endrow = row + size-1;
    if(col >= m_columns )
    {
        m_columns = (col+1);
    }
    if(endrow+1 > m_rows )
        m_rows = endrow+1;
    for(int i=0;i<size;++i)
    {
        m_data.insert (qMakePair(row+i,col),cols[i]);
    }
    return endrow;
}

template <typename T>
typename SAHashTable<T>::Index SAHashTable<T>::appendColumn(const QList<T> &cols)
{
    int size = cols.size ();
    int colIndex = m_columns;
    ++m_columns;
    if(size > m_rows )
        m_rows = size;
    for(int i=0;i<size;++i)
    {
        m_data.insert (qMakePair(i,colIndex),cols[i]);
    }
    return m_columns;
}

template <typename T>
void SAHashTable<T>::clear()
{
    m_data.clear ();
    m_columns = 0;
    m_rows = 0;
}

template <typename T>
QList<T> SAHashTable<T>::getColumn(typename SAHashTable<T>::Index index, bool ifGetNullData) const
{
    QList<T> res;
    if(index >= m_columns)
        return QList<T>();
    if(ifGetNullData)
    {
        for(int i=0;i<m_rows;++i)
        {
            res.push_back (at(i,index));
        }
    }
    else
    {
        for(int i=0;i<m_rows;++i)
        {
            if(!isHaveData (i,index))
                continue;
            res.push_back (at(i,index));
        }
    }
    return res;
}

template <typename T>
QList<T> SAHashTable<T>::getRow(typename SAHashTable<T>::Index index, bool ifGetNullData) const
{
    QList<T> res;
    if(index >= m_rows)
        return QList<T>();
    if(ifGetNullData)
    {
        for(int i=0;i<m_columns;++i)
        {
            res.push_back (at(index,i));
        }
    }
    else
    {
        for(int i=0;i<m_columns;++i)
        {
            if(!isHaveData (index,i))
                continue;
            res.push_back (at(index,i));
        }
    }
    return res;
}

template <typename T>
void SAHashTable<T>::setDatas(const typename SAHashTable<T>::Table &datas)
{
    typename Table::const_iterator i = datas.begin();
    typename Table::const_iterator end = datas.end();
    Index maxRow(0),maxCol(0);
    if(i == end)
        return;
    while(i != end)
    {
        if( maxRow  < i.key().first )
            maxRow = i.key().first;
        if(maxCol < i.key().second)
            maxCol = i.key().second;
        ++i;
    }
    m_data = datas;
    m_columns = maxCol+1;
    m_rows = maxRow+1;
}




template<typename T>
QDataStream &operator<<(QDataStream & out, const SAHashTable<T> & item);
template<typename T>
QDataStream &operator>>(QDataStream & in, SAHashTable<T> & item);

template<typename T>
QDataStream &operator<<(QDataStream & out, const SAHashTable<T> & item)
{
    out << item.data();
    return out;
}

template<typename T>
QDataStream &operator>>(QDataStream & in, SAHashTable<T> & item)
{
    typename SAHashTable<T>::Table table;
    in >> table;
    item.setDatas(table);
    return in;
}






#endif // SAHASHTABLE_H


