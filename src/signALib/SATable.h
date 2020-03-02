#ifndef SATABLE_H
#define SATABLE_H
#include <QVector>
#include "SALibGlobal.h"

template <typename T>
class SALIB_EXPORT SATable
{
public:
    typedef QVector<T> TableRow;
    SATable();
    SATable(int rows,int columns);
    const T& at(int r,int c) const;
    T& at(int r,int c);
    bool appendRow(const TableRow& row);
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


template<typename T>
SATable<T>::SATable():m_columns(0)
{

}

template<typename T>
SATable<T>::SATable(int rows, int columns)
{
    m_d.clear ();
    m_d.reserve (rows);
    for(int i=0;i<rows;++i)
    {
        m_d.push_back (QVector<T>(columns));
    }
    m_columns = columns;
}

template<typename T>
const T &SATable<T>::at(int r, int c) const
{
    return m_d.at (r).at (c);
}

template<typename T>
T &SATable<T>::at(int r, int c)
{
    return m_d[r][c];
}

template<typename T>
bool SATable<T>::appendRow(const TableRow &row)
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

template<typename T>
int SATable<T>::rowCount() const
{
    return m_d.size ();
}

template<typename T>
int SATable<T>::columnCount() const
{
   return m_columns;
}


#endif // SATABLE_H
