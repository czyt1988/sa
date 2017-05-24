#include "QTDMChannelTableModel.h"
#include "QTDMChannel.h"
QTDMChannelTableModel::QTDMChannelTableModel(QObject* parent):QAbstractTableModel(parent)
  ,m_rowCount(0)
{
}

int QTDMChannelTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_rowCount;
}

int QTDMChannelTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_datas.size ();
}

QVariant QTDMChannelTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        if (section < m_datas.size ())
        {
            return m_datas[section]->getName();
        }
    }
    else if(Qt::Vertical == orientation)
    {//垂直表头
        return section+1;
    }
    return QVariant();
}

QVariant QTDMChannelTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    { //返回的是对其方式
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole){ //返回的是现实内容
        int row = index.row();
        int col = index.column();
        if(col >= m_datas.size ())
            return QVariant();
        QTDMChannel* channel = m_datas[col];
        if(row>=channel->getDataNums ())
            return QVariant();
        return channel->getVariantData (row);
    }
    return QVariant();
}

bool QTDMChannelTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

    return false;
}

Qt::ItemFlags QTDMChannelTableModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void QTDMChannelTableModel::setChannels(const QList<QTDMChannel*>& channels)
{
    m_datas = channels;
    int size = m_datas.size ();
    m_rowCount = 0;
    for(int i=0;i<size;++i)
    {
        int num = m_datas[i]->getDataNums ();
        if(num>m_rowCount)
            m_rowCount = num;
    }
}
