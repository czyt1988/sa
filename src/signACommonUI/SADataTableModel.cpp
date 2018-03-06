#include "SADataTableModel.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include "SAAbstractDatas.h"
#include "SAPropertySetDialog.h"
SADataTableModel::SADataTableModel(QObject* parent):QAbstractTableModel(parent)
  ,m_rowCount(0)
  ,m_columnCount(0)
  ,m_columnShowMin(15)
  ,m_rowShowMin(35)
  ,m_funSetData(nullptr)
{
}

void SADataTableModel::setSADataPtr(SAAbstractDatas *data)
{
    if(nullptr == data)
    {
        clear();
        return;
    }
    setSADataPtrs({data});
}

void SADataTableModel::setSADataPtrs(const QList<SAAbstractDatas *> &datas)
{
    if(datas.size() <= 0)
    {
        clear();
        return;
    }
	beginResetModel();
    m_datas.clear();
    for(auto i=datas.begin();i!=datas.end();++i)
    {
        if((*i)->getDim()<=2)
        {
            m_datas.append(*i);
        }
    }
    reCalcRowAndColumnCount();
    endResetModel();
}

void SADataTableModel::appendSADataPtr(SAAbstractDatas *data)
{
    if(nullptr == data)
    {
        return;
    }
    appendSADataPtrs({data});
}

void SADataTableModel::appendSADataPtrs(QList<SAAbstractDatas*> datas)
{
    if(datas.size() <= 0)
    {
        return;
    }
    beginResetModel();
    for(auto i=datas.begin();i!=datas.end();++i)
    {
        if((*i)->getDim()<=2)
        {
            m_datas.append(*i);
        }
    }
    reCalcRowAndColumnCount();
    endResetModel();
}
///
/// \brief 移除显示的数据
/// \param datas
///
void SADataTableModel::removeDatas(const QList<SAAbstractDatas *> &datas)
{
    QList<SAAbstractDatas *> newData;
    for(int i=0;i<m_datas.size();++i)
    {
        SAAbstractDatas * d = m_datas[i];
        if(!datas.contains(d))
        {
            newData.append(d);
        }
    }
    setSADataPtrs(newData);

}
void SADataTableModel::update()
{
    beginResetModel();
    reCalcRowAndColumnCount();
    endResetModel();
}

void SADataTableModel::clear()
{
    beginResetModel();
    m_rowCount = 0;
    m_columnCount = 0;
    m_datas.clear();
    m_col2Ptr.clear();
    m_ptr2Col.clear();
    m_ptr2ColMap.clear();
    endResetModel();
}


bool SADataTableModel::isEmpty() const
{
    return (0 == m_datas.size());
}

void SADataTableModel::setupSetDataFun(SADataTableModel::FUN_SET_DATA p)
{
    m_funSetData = p;
}
///
/// \brief 根据列号获取对应数据指针
/// \param c
/// \return
///
SAAbstractDatas *SADataTableModel::columnToData(int c)
{
    return m_col2Ptr.value(c,nullptr);
}
///
/// \brief 根据数据获取对应的列范围
/// \param p
/// \param start
/// \param end
/// \return
///
void SADataTableModel::dataColumnRange(SAAbstractDatas *p, int &start, int &end)
{
    QList<int> cols = m_ptr2Col.values(p);
    std::sort(cols.begin(),cols.end());
    if(cols.size() > 0)
    {
        start = *(cols.begin());
        end = *(cols.end()-1);
    }
}

const QList<SAAbstractDatas *> &SADataTableModel::getSADataPtrs() const
{
    return m_datas;
}

int SADataTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_rowCount > m_rowShowMin ? m_rowCount+m_rowShowMin : m_rowShowMin;
}

int SADataTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return (m_columnCount > m_columnShowMin) ? m_columnCount : m_columnShowMin;
}


QVariant SADataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        if(m_datas.size() <= 0)
        {
            return QVariant();
        }
        SAAbstractDatas* d = m_col2Ptr.value(section,nullptr);
        if(!d)
        {
            return QVariant();
        }
        switch (d->getDim()) {
        case 0:
            return horizontalHeaderToDim0(section,d);
            break;
        case 1:
            return horizontalHeaderToDim1(section,d);
            break;
        case 2:
            return horizontalHeaderToDim2(section,d);
            break;
        default:
            break;
        }
        return QVariant();
    }
    else if(Qt::Vertical == orientation)
    {//垂直表头
        return section+1;
    }
    return QVariant();
}

QVariant SADataTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    { //返回的是对其方式
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    { //返回的是显示内容
        int col = index.column();
        int row = index.row();
        if(col >= m_col2Ptr.size ())
        {
            return QVariant();
        }
        SAAbstractDatas* d = m_col2Ptr.value(col,nullptr);
        if(!d)
        {
            return QVariant();
        }
        switch (d->getDim()) {
        case 0:
            return dataToDim0(row,col,d);
            break;
        case 1:
            return dataToDim1(row,col,d);
            break;
        case 2:
            return dataToDim2(row,col,d);
            break;
        default:
            break;
        }
        return QVariant();
    }
    return QVariant();
}

QVariant SADataTableModel::dataToDim0(int row, int col, SAAbstractDatas *d) const
{
    Q_UNUSED(col);
    if(row > 0)
        return QVariant();
    return d->displayAt(0);
}

QVariant SADataTableModel::dataToDim1(int row, int col, SAAbstractDatas *d) const
{
    Q_UNUSED(col);
    if(row >= d->getSize(SA::Dim1))
        return QVariant();
    return d->displayAt(row);
}

QVariant SADataTableModel::dataToDim2(int row, int col, SAAbstractDatas *d) const
{
    if(row >= d->getSize(SA::Dim1))
    {
        return QVariant();
    }
    auto ite = m_ptr2ColMap.find(d);
    if(ite == m_ptr2ColMap.end())
    {
        return QVariant();
    }
    auto iteSize = ite->find(col);
    if(iteSize == ite->end())
    {
        return QVariant();
    }
    return d->displayAt(row,*iteSize);
}

QVariant SADataTableModel::horizontalHeaderToDim0(int section, SAAbstractDatas *d) const
{
    Q_UNUSED(section);
    return d->getName();
}

QVariant SADataTableModel::horizontalHeaderToDim1(int section, SAAbstractDatas *d) const
{
    Q_UNUSED(section);
    return d->getName();
}

QVariant SADataTableModel::horizontalHeaderToDim2(int section, SAAbstractDatas *d) const
{
    auto ite = m_ptr2ColMap.find(d);
    if(ite == m_ptr2ColMap.end())
    {
        return QVariant();
    }
    auto iteSize = ite->find(section);
    if(iteSize == ite->end())
    {
        return QVariant();
    }
    return QString("%1(%2)").arg(d->getName()).arg(*iteSize+1);
}

bool SADataTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid() || nullptr == m_funSetData || role != Qt::EditRole)
    {
        return false;
    }
    const int col = index.column();
    const int row = index.row();
    return m_funSetData(row,col,value);

    if(col >= m_col2Ptr.size ())
    {
        return false;
    }

}

Qt::ItemFlags SADataTableModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
///
/// \brief 数据区的行号 区别于rowCount是显示区的
/// \return
///
int SADataTableModel::dataRowCount() const
{
    return m_rowCount;
}
///
/// \brief 数据区的列号 区别于columnCount是显示区的
/// \return
///
int SADataTableModel::dataColumnCount() const
{
    return m_columnCount;
}

void SADataTableModel::reCalcRowAndColumnCount()
{
    //计算最大行数
    m_rowCount = 0;
    m_columnCount = 0;
    m_col2Ptr.clear();
    m_ptr2ColMap.clear();
    m_ptr2Col.clear();
    int col = 0;
    for(auto i = m_datas.begin ();i!=m_datas.end ();++i)
    {
        if(SA::Dim0 == (*i)->getDim())
        {
            //0维也就是单一数字
            QHash<int,int> col2ptrCol;
            if(1 > m_rowCount)
                m_rowCount = 1;
            col2ptrCol[col] = 0;
            m_col2Ptr[col] = (*i);
            m_ptr2Col.insert(*i,col);
            m_ptr2ColMap[(*i)] = col2ptrCol;
            ++col;
        }
        else if(SA::Dim1 == (*i)->getDim())
        {
            //1维也就是向量
            QHash<int,int> col2ptrCol;
            int size = (*i)->getSize(SA::Dim1);
            if(size<0)
                continue;
            if(size > m_rowCount)
                m_rowCount = size;
            col2ptrCol[col] = 0;
            m_col2Ptr[col] = (*i);
            m_ptr2Col.insert(*i,col);
            m_ptr2ColMap[(*i)] = col2ptrCol;
            ++col;
        }
        else if(SA::Dim2 == (*i)->getDim())
        {
            //2维也就是表
            QHash<int,int> col2ptrCol;
            int size = (*i)->getSize(SA::Dim1);
            if(size<0)
                continue;
            if(size > m_rowCount)
                m_rowCount = size;
            size = (*i)->getSize(SA::Dim2);
            for(int k=0;k<size;++k)
            {
                m_col2Ptr[col] = (*i);
                m_ptr2Col.insert(*i,col);
                col2ptrCol[col] = k;
                ++col;
            }
            m_ptr2ColMap[(*i)] = col2ptrCol;
        }

    }
    m_columnCount = col;

}







