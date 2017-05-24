#include "DataFeatureTreeModel.h"

DataFeatureTreeModel::DataFeatureTreeModel(QObject *parent) : QAbstractItemModel(parent)
{

}

QModelIndex DataFeatureTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0)
        return QModelIndex();

    if(!parent.isValid ())//说明不是顶层
    {
        if((row >= m_items.size())
            ||
            (column >= 1))
        {
            return QModelIndex();
        }
        return createIndex(row, column, m_items[row]);//顶层节点
    }
    AbstractDataFeatureItem* parItem = toItemPtr(parent);
    if ((nullptr == parItem)
        || (row < 0)
        || (column < 0)
        || (row >= parItem->rowCount())
        || (column >= parItem->columnCount())) {
        return QModelIndex();//不正常情况
    }
    return createIndex(row, column, parItem->child(row,column));
}

QModelIndex DataFeatureTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    AbstractDataFeatureItem* item =  toItemPtr(index);
    if(!item)
        return QModelIndex();
    QStandardItem* parItem =  item->parent();
    if(nullptr == parItem)
    {
        //父指针为0，说明是顶层item
        return QModelIndex();
    }
    QStandardItem* grandParItem = parItem->parent();//祖父指针，这个比较关键
    if(nullptr == grandParItem)
    {//如果祖父为0，说明它是第二层级，parent是1层，但不能用它自身的parItem->row(), parItem->column()
     //需要在QLsit里查找它的层次
        int row = m_items.indexOf(static_cast<AbstractDataFeatureItem*>(parItem));
        if(row<0)
        {//说明没有在QList找到
            return QModelIndex();
        }
        return createIndex(row, 0, parItem);
    }

    return createIndex(parItem->row(), parItem->column(), parItem);
}

int DataFeatureTreeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return m_items.size();
    AbstractDataFeatureItem* parItem = toItemPtr(parent);
    return parItem ? parItem->rowCount() : 0;
}

int DataFeatureTreeModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return 2;
    AbstractDataFeatureItem* parItem = toItemPtr(parent);
    return parItem ? parItem->columnCount() : 0;
}

QVariant DataFeatureTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        switch(section)
        {
        case 0:return tr("property");
        case 1:return tr("value");//值
        }
    }
    return QVariant();
}

Qt::ItemFlags DataFeatureTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant DataFeatureTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    AbstractDataFeatureItem* item = toItemPtr(index);
    if(Qt::BackgroundRole == role)
    {
        if(nullptr == item)
            return QVariant();
        if(item->rtti() == AbstractDataFeatureItem::CurveDataFeatureItem)
        {
            CurveDataFeatureItem* cur = static_cast<CurveDataFeatureItem*>(item);
            QColor clr = cur->plotCurvePtr()->pen().color();
            clr.setAlpha(30);
            return clr;
        }
    }
    if(item)
        return item->data(role);
    return QVariant();
}

void DataFeatureTreeModel::appendDataFeatureItem(AbstractDataFeatureItem *item)
{
    beginResetModel();
    m_items.append(item);
    endResetModel();
}

void DataFeatureTreeModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

void DataFeatureTreeModel::setDataFeatureItems(const QList<AbstractDataFeatureItem *> &items)
{
    beginResetModel();
    m_items = items;
    endResetModel();
}

AbstractDataFeatureItem *DataFeatureTreeModel::toItemPtr(const QModelIndex &index) const
{
    return static_cast<AbstractDataFeatureItem *>(index.internalPointer());
}

