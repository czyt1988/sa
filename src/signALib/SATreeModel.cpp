#include "SATreeModel.h"
#include "SATree.h"
#include "SAVariantCaster.h"
#include <QVector>
#include <iostream>
#include <memory>


class SATreeModelPrivate
{
    SA_IMPL_PUBLIC(SATreeModel)
public:
    SATreeModelPrivate(SATreeModel* p);
    bool isNull() const;
    void setColumnCount(int col);
    SATree* mTreePtr;
    int mColumnCount;
    QVector<int> mColumnRoles;
    QVector<std::shared_ptr<SAItem> > mHHeaderItem;///< 头所用的item，如果没有设置，则设置为nullptr
};

SATreeModelPrivate::SATreeModelPrivate(SATreeModel *p):q_ptr(p)
  ,mTreePtr(nullptr)
  ,mColumnCount(1)
{

}

bool SATreeModelPrivate::isNull() const
{
    return nullptr == mTreePtr;
}

void SATreeModelPrivate::setColumnCount(int col)
{
    mColumnCount = col;
    mHHeaderItem.resize(col);
    for(int i = 0;i<col;++i)
    {
        if(mHHeaderItem[i] == nullptr)
        {
            mHHeaderItem[i] = std::make_shared<SAItem>();
        }
    }
}

SATreeModel::SATreeModel(QObject *par, int colCount):QAbstractItemModel(par)
  ,d_ptr(new SATreeModelPrivate(this))
{
    d_ptr->setColumnCount(colCount);
}

SATreeModel::~SATreeModel()
{

}

QModelIndex SATreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0)
        return QModelIndex();
    if(d_ptr->isNull())
        return QModelIndex();
    if(!parent.isValid ())//说明是顶层
    {
        if((row >= d_ptr->mTreePtr->getItemCount())
            ||
            (column >= d_ptr->mColumnCount))
        {
            return QModelIndex();
        }
        return createIndex(row, column, d_ptr->mTreePtr->getItem(row));//顶层节点
    }
    SAItem* parItem = toItemPtr(parent);
    if ((nullptr == parItem)
        || (row >= parItem->childItemCount())
        || (column >= d_ptr->mColumnCount))
    {
        return QModelIndex();//不正常情况
    }
    return createIndex(row, column, parItem->childItem(row));
}

QModelIndex SATreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    SAItem* item =  toItemPtr(index);
    if(nullptr == item)
        return QModelIndex();
    SAItem* parItem =  item->parent();
    if(nullptr == parItem)
    {
        //父指针为0，说明是顶层item
        return QModelIndex();
    }
    SAItem* grandParItem = parItem->parent();//祖父指针，这个比较关键
    if(nullptr == grandParItem)
    {//如果祖父为0，说明它是第二层级，parent是1层，但不能用它自身的parItem->row(), parItem->column()
     //需要在QLsit里查找它的层次
        int row = d_ptr->mTreePtr->indexOfItem(parItem);
        if(row<0)
        {//说明没有在QList找到
            return QModelIndex();
        }
        return createIndex(row, 0, parItem);
    }
    return createIndex(parItem->fieldRow(), 0, parItem);//挂载parent的都是只有一列的
}

int SATreeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        return d_ptr->mTreePtr->getItemCount();
    }
    SAItem* parItem = toItemPtr(parent);
    return parItem ? parItem->childItemCount() : 0;

}

int SATreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d_ptr->mColumnCount;
}
/**
 * @brief 表头
 * @param section
 * @param orientation
 * @param role
 * @return
 */
QVariant SATreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {
        if(section < d_ptr->mHHeaderItem.size())
        {
            std::shared_ptr<SAItem> item = d_ptr->mHHeaderItem[section];
            if(item != nullptr)
            {
                return item->getName();
            }
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags SATreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant SATreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    SAItem* item = toItemPtr(index);
    if(nullptr == item)
    {
        return QVariant();
    }
    if(Qt::DisplayRole == role)
    {
        switch(index.column())
        {
        case 0:
        {
            return item->getName();
        }
        default:
            Q_ASSERT_X(index.column() < d_ptr->mColumnRoles.size(), "SATreeModel::data", "column is large than columnRoles");
            int propRole = d_ptr->mColumnRoles[index.column()];
            return SAVariantCaster::variantToString(item->property(propRole));
        }
    }
    else if(Qt::BackgroundRole == role)
    {
        return item->getProperty(Qt::BackgroundRole);
    }
    return QVariant();

}
/**
 * @brief 设置列的数量
 * @param col 列的数量
 */
void SATreeModel::setColumnCount(int col, const QVector<int> &roles)
{
    beginResetModel();
    d_ptr->setColumnCount(col);
    d_ptr->mColumnRoles = roles;
    endResetModel();
}
/**
 * @brief 设置除第一列的其他列索引，这个函数需要在列数不为1时设置，否则无法获知其余列的索引位置
 * @param roles
 */
void SATreeModel::setColumnsDisplayIDs(const QVector<int> &roles)
{
    beginResetModel();
    d_ptr->mColumnRoles = roles;
    endResetModel();
}
/**
 * @brief 设置列索引，用于显示saitem的property
 * 此操作不会刷新，需要收到刷新model
 * @param index
 * @param role
 */
void SATreeModel::setColumnDisplayID(int index, int role)
{
   d_ptr->mColumnRoles[index] = role;
}
/**
 * @brief 插入水平header对应的item，效仿QStandardItemModel
 * @param column 列号
 * @param item saitem，其生命周期由satreemodel来管理
 */
void SATreeModel::setHorizontalHeaderItem(int column, SAItem *item)
{
    const int size = d_ptr->mHHeaderItem.size();
    Q_ASSERT_X(column < size,"SATreeModel::setHorizontalHeaderItem","column set in setHorizontalHeaderItem is invalid");
    d_ptr->mHHeaderItem[column] = std::shared_ptr<SAItem>(item);
}

SAItem *SATreeModel::toItemPtr(const QModelIndex &index) const
{
    return static_cast<SAItem *>(index.internalPointer());
}


