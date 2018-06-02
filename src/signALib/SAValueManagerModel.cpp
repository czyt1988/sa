#include <QtGlobal>
#include "SAValueManagerModel.h"
#include "SAValueManager.h"
#include "SAData.h"
#include "SALibResourDefine.h"
#define COL_COUNT 3
#define COL_INDEX_NAME 0
#define COL_INDEX_TYPE 1
#define COL_INDEX_DES 2


SAValueManagerModel::SAValueManagerModel(QObject *parent):QAbstractItemModel(parent)
  ,m_funBackgroundData(nullptr)
{
    connect(saValueManager,&SAValueManager::dataAdded
            ,this,&SAValueManagerModel::onDataAdded);
    connect(saValueManager,&SAValueManager::dataNameChanged
            ,this,&SAValueManagerModel::onDataNameChanged);
    connect(saValueManager,&SAValueManager::dataRemoved
            ,this,&SAValueManagerModel::onDataDeleted);
    connect(saValueManager,&SAValueManager::dataClear
            ,this,&SAValueManagerModel::onDataClear);
}

QModelIndex SAValueManagerModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0)
        return QModelIndex();
    if(!parent.isValid ())//说明是顶层
    {
        if((row >= saValueManager->count())
            ||(column >= COL_COUNT))
        {
            return QModelIndex();
        }
        return createIndex(row, column, saValueManager->at(row));//顶层节点
    }
    SAItem* parItem = toItemPtr(parent);
    if ((parItem == nullptr)
        || (row < 0)
        || (column < 0)
        || (row >= parItem->rowCount())
        || (column >= COL_COUNT)) {
        return QModelIndex();
    }
    return createIndex(row, column, parItem->child(row));
}

QModelIndexList SAValueManagerModel::datasToIndexs(const QSet<SAAbstractDatas *> &data)
{
    QModelIndexList indexList;
    ergodicAllItem([&](const QModelIndex& index)->bool
    {
        SAAbstractDatas * d = this->castToDataPtr(index);
        if(nullptr == d)
        {
            return true;//继续迭代
        }
        if(0 != index.column())
        {
            return true;//继续迭代
        }
        if(data.contains(d))
        {
            indexList.append(index);
            if(indexList.size() == data.size())
            {
                return false;//结束迭代
            }
        }
        return true;
    });
    return indexList;
}
///
/// \brief 迭代SAValueManagerModel下的所有条目
/// \param model
/// \param pFun 为回调函数，回调函数返回true代表继续迭代，回调函数返回false代表结束迭代
///
void SAValueManagerModel::ergodicAllItem( FunErgodicItemPtr pFun)
{
    int rows = rowCount();
    int column = columnCount();
    for (int i=0;i<rows;++i)
    {
        for(int j=0;j<column;++j)
        {
            QModelIndex item = index(i,j);
            if (item.isValid())
            {
                if(!ergodicItem(item,pFun))
                    return;
            }
        }
    }
}
///
/// \brief 从某一个QModelIndex开始向下迭代，如果迭代结束，返回false
/// \param item 开始迭代的QStandardItem
/// \param pFun 为回调函数，输入为当前迭代的QStandardItem指针，回调函数返回true代表继续迭代，回调函数返回false代表结束迭代
/// \return false 代表迭代结束
///
bool SAValueManagerModel::ergodicItem(const QModelIndex &index, SAValueManagerModel::FunErgodicItemPtr pFun)
{
    int rows = index.row();
    int column = index.column();
    for (int i=0;i<rows;++i)
    {
        for(int j=0;j<column;++j)
        {
            QModelIndex childItem = index.child(i,j);
            if (childItem.isValid())
            {
                if(!ergodicItem(childItem,pFun))
                    return false;
            }
        }
    }
    return pFun(index);
}

QModelIndex SAValueManagerModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    SAItem* item =  toItemPtr(index);
    if(!item)
        return QModelIndex();
    SAItem* parItem =  dynamic_cast<SAItem*>(item->parent());
    if(!parItem)
        return QModelIndex();
    SAItem* grandParItem = dynamic_cast<SAItem*>(parItem->parent());//祖父指针，这个比较关键
    if(!grandParItem)
    {//如果祖父为0，说明它是第二层级，这是的parent是1层，但不能用它自身的parItem->row(), parItem->column()
     //需要在QLsit里查找它的层次
        int row = saValueManager->indexOf(static_cast<SAAbstractDatas*>(parItem));
        return createIndex(row, 0, parItem);
    }
    return createIndex(parItem->row(), parItem->column(), parItem);

}

int SAValueManagerModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return saValueManager->count();
    SAItem* item =  toItemPtr(parent);
    return item ? item->rowCount() : 0;
}

int SAValueManagerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COL_COUNT;
}

QVariant SAValueManagerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        switch(section)
        {
        case COL_INDEX_NAME:return tr("name");
        case COL_INDEX_TYPE:return tr("type");
        case COL_INDEX_DES:return tr("describe");
        }
    }
    return QVariant();
}

Qt::ItemFlags SAValueManagerModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant SAValueManagerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    SAItem* item = toItemPtr(index);
    SAAbstractDatas* data = dynamic_cast<SAAbstractDatas*>(item);
    if(item)
    {
        if(Qt::DisplayRole == role)
        {
            switch(index.column())
            {
            case COL_INDEX_NAME:return item->getName();
            case COL_INDEX_TYPE:return item->getTypeName();
            case COL_INDEX_DES:
            {
                if(data)
                {
                    int dim = data->getDim();
                    if(1 == dim)
                    {
                        int r = data->getSize();
                        return QStringLiteral("[%1]").arg(r);
                    }
                    else if(2 == dim)
                    {
                        int r = data->getSize(SA::Dim1);
                        int c = data->getSize(SA::Dim2);
                        return QStringLiteral("[%1×%2]").arg(r).arg(c);
                    }
                    else if(3 == dim)
                    {
                        int r = data->getSize(SA::Dim1);
                        int c = data->getSize(SA::Dim2);
                        int z = data->getSize(SA::Dim3);
                        return QStringLiteral("[%1×%2×%3]").arg(r).arg(c).arg(z);
                    }
                    else if(dim > 3)
                    {
                        int r = data->getSize(SA::Dim1);
                        int c = data->getSize(SA::Dim2);
                        int z = data->getSize(SA::Dim3);
                        return QStringLiteral("[%1×%2×%3×...]").arg(r).arg(c).arg(z);
                    }
                    return QVariant();
                }
            }
            default:
                return QVariant();
            }
        }
        else if(Qt::DecorationRole == role)
        {
            if(!data)
                return QVariant();
            switch(index.column())
            {
            case COL_INDEX_NAME:return SAValueManager::getDataIcon(data->getType());
            default:
                return QVariant();
            }
        }
        else if(Qt::BackgroundRole == role)
        {
            if(m_funBackgroundData)
            {
                return m_funBackgroundData(index,data);
            }
        }
    }
    return QVariant();
}


bool SAValueManagerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //改名
    if(index.column() == COL_INDEX_NAME)
    {
        if(role == Qt::DisplayRole)
        {
            SAItem* item = toItemPtr(index);
            SAAbstractDatas* data = dynamic_cast<SAAbstractDatas*>(item);
            if(data)
            {
                return saValueManager->renameData(data,value.toString());
            }
        }
    }
    return false;
}

//Qt::DropActions SAValueManagerModel::supportedDropActions() const
//{
//    return Qt::CopyAction | Qt::MoveAction;
//}

SAAbstractDatas *SAValueManagerModel::castToDataPtr(const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        SAItem * item = SAValueManagerModel::toItemPtr(index);
        return dynamic_cast<SAAbstractDatas*>(item);
    }
    else
    {
        QModelIndex par = index.parent();
        QModelIndex dataIndex;
        if(par.isValid())
        {
            dataIndex = par.child(index.row(),0);
        }
        else
        {
            dataIndex = this->index(index.row(),0,par);
        }
        SAItem * item = SAValueManagerModel::toItemPtr(dataIndex);
        return dynamic_cast<SAAbstractDatas*>(item);
    }
    return nullptr;
}
///
/// \brief 根据modelindex获取其item指针
/// \param index
/// \return
///
SAItem *SAValueManagerModel::toItemPtr(const QModelIndex &index)
{
    return static_cast<SAItem *>(index.internalPointer());
}




void SAValueManagerModel::onDataAdded(const QList<SAAbstractDatas *> &datas)
{
    Q_UNUSED(datas);
    updateModel();
}

void SAValueManagerModel::onDataDeleted(const QList<SAAbstractDatas *> &dataBeDeletedPtr)
{
    Q_UNUSED(dataBeDeletedPtr);
    updateModel();
}

void SAValueManagerModel::onDataClear()
{
    updateModel();
}

void SAValueManagerModel::onDataNameChanged(SAAbstractDatas *data, const QString &oldName)
{
    Q_UNUSED(data);
    Q_UNUSED(oldName);
    updateModel();
}

void SAValueManagerModel::updateModel()
{
    beginResetModel();
    endResetModel();
}
///
/// \brief 添加背景颜色显示的回调函数
/// \param funBackgroundData 回调函数
///
void SAValueManagerModel::setFunBackgroundData(FunBackgroundShowPtr funBackgroundData)
{
    m_funBackgroundData = funBackgroundData;
}


