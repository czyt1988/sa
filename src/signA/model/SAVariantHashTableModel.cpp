#include "SAVariantHashTableModel.h"

SAVariantHashTableModel::SAVariantHashTableModel(QObject* parent):QAbstractTableModel(parent)
  ,m_data(nullptr)
{

}

SAVariantHashTableModel::~SAVariantHashTableModel()
{

}

int SAVariantHashTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    if(nullptr == m_data)
    {
        return 0;
    }
    return m_data->rowCount ();
}

int SAVariantHashTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    if(nullptr == m_data)
    {
        return 0;
    }
    return m_data->columnCount ();
}

QVariant SAVariantHashTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        return section+1;
    }
    else if(Qt::Vertical == orientation)
    {//垂直表头
        return section+1;
    }
    return QVariant();
}

Qt::ItemFlags SAVariantHashTableModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant SAVariantHashTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(nullptr == m_data)
    {
        return QVariant();
    }
    if (role == Qt::TextAlignmentRole)
    { //返回的是对其方式
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole) //返回的是现实内容
    {
        return m_data->at (index.row (),index.column ());
    }
    return QVariant();
}

bool SAVariantHashTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (Qt::EditRole == role)
    {
        if(nullptr == m_data)
        {
            return false;
        }
        m_data->setData (index.row (),index.column (),value);
    }
    else
    {
        return QAbstractTableModel::setData(index,value,role);
    }
    return true;
}

void SAVariantHashTableModel::update()
{
    beginResetModel();
    endResetModel();
}

void SAVariantHashTableModel::setHashTableData(SAVariantHashTableModel::Table* prt)
{
    m_data = prt;
}

const SAVariantHashTableModel::Table*SAVariantHashTableModel::getHashTableData() const
{
    return m_data;
}

SAVariantHashTableModel::Table*SAVariantHashTableModel::getHashTableData()
{
    return m_data;
}
