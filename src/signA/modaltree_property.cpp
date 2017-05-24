#include "modaltree_property.h"

ModelTree_Property::ModelTree_Property(){
    m_pRootItem.reset(new TreeItem_Property(QStringLiteral("属性"),QStringLiteral("值")));
}

ModelTree_Property::ModelTree_Property(const QString& strhead1,const QString& strhead2){
    m_pRootItem.reset(new TreeItem_Property(strhead1,strhead2));
}

void ModelTree_Property::setHeaderString(const QString& strhead1,const QString& strhead2){
    m_pRootItem->setName(strhead1);
    m_pRootItem->setData(strhead2);
}


QVariant ModelTree_Property::data(const QModelIndex &index, int role)
const
{
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (nullptr == item)
        return QVariant();
    return item->data(index.column());
}

Qt::ItemFlags ModelTree_Property::flags(const QModelIndex &index)
const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

QVariant ModelTree_Property::headerData(int section
                                       , Qt::Orientation orientation
                                       , int role)
const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_pRootItem->data(section);
   return QVariant();
}

QModelIndex ModelTree_Property::index(int row
                                     ,int column
                                     ,const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
            return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())//没有定义父级，自动挂到顶层
        parentItem = m_pRootItem.get();
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    if (nullptr == parentItem)
        return QModelIndex();

    TreeItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ModelTree_Property::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());

    if(nullptr == childItem){
        q_check_ptr(childItem);
        return QModelIndex();
    }

    TreeItem *parentItem = childItem->parent();

    if (parentItem == m_pRootItem.get())
        return QModelIndex();
    if(nullptr == parentItem){
        q_check_ptr(parentItem);;
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

int ModelTree_Property::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_pRootItem.get();
    else
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    if(nullptr == parentItem){
        q_check_ptr(parentItem);
        return 0;
    }
    return parentItem->childCount();
}
int ModelTree_Property::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return m_pRootItem->columnCount();
}
