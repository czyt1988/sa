#include "DataFeatureTreeModel.h"
#include "SAVariantCaster.h"
DataFeatureTreeModel::DataFeatureTreeModel(QObject *parent) : QAbstractItemModel(parent)
{

}

QModelIndex DataFeatureTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0)
        return QModelIndex();

    if(!parent.isValid ())//说明是顶层
    {
        if((row >= m_items.size())
            ||
            (column >= 1))
        {
            return QModelIndex();
        }
        return createIndex(row, column, m_items[row]);//顶层节点
    }
    SADataFeatureItem* parItem = toItemPtr(parent);
    if ((nullptr == parItem)
        || (row < 0)
        || (column < 0)
        || (row >= parItem->getChildCount())
        || (column >= 2)) {
        return QModelIndex();//不正常情况
    }
    return createIndex(row, column, parItem->getChild(row));
}

QModelIndex DataFeatureTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    SADataFeatureItem* item =  toItemPtr(index);
    if(!item)
        return QModelIndex();
    SADataFeatureItem* parItem =  item->getParent();
    if(nullptr == parItem)
    {
        //父指针为0，说明是顶层item
        return QModelIndex();
    }
    SADataFeatureItem* grandParItem = parItem->getParent();//祖父指针，这个比较关键
    if(nullptr == grandParItem)
    {//如果祖父为0，说明它是第二层级，parent是1层，但不能用它自身的parItem->row(), parItem->column()
     //需要在QLsit里查找它的层次
        int row = m_items.indexOf(parItem);
        if(row<0)
        {//说明没有在QList找到
            return QModelIndex();
        }
        return createIndex(row, 0, parItem);
    }

    return createIndex(parItem->getCurrentRowIndex(), 0, parItem);//挂载parent的都是只有一列的
}

int DataFeatureTreeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return m_items.size();
    SADataFeatureItem* parItem = toItemPtr(parent);
    return parItem ? parItem->getCurrentRowIndex() : 0;
}

int DataFeatureTreeModel::columnCount(const QModelIndex &parent) const
{
    return 2;
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
    SADataFeatureItem* item = toItemPtr(index);
    if(nullptr == item)
        return QVariant();
    if(Qt::BackgroundRole == role)
    {
        return item->getBackground();
    }

    if(item)
    {
        switch(index.column())
        {
        case 0:return item->getName();
        case 1:
            return SAVariantCaster::variantToString(item->getValue());
        }
    }
    return QVariant();
}

void DataFeatureTreeModel::appendDataFeatureItem(SADataFeatureItem *item)
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

void DataFeatureTreeModel::setDataFeatureItems(const QList<SADataFeatureItem *> &items)
{
    beginResetModel();
    m_items = items;
    endResetModel();
}

SADataFeatureItem *DataFeatureTreeModel::toItemPtr(const QModelIndex &index) const
{
    return static_cast<SADataFeatureItem *>(index.internalPointer());
}

