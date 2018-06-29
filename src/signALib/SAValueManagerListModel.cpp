#include "SAValueManagerListModel.h"
#include <QStringList>
#include "SAValueManager.h"
#include <QDebug>
class SAValueManagerListModelPrivate
{
    SA_IMPL_PUBLIC(SAValueManagerListModel)
public:
    QList<int> valIndex;
    SAValueManagerListModel::FunPtrFilter funFiter;
    SAValueManagerListModelPrivate(SAValueManagerListModel* p)
    :q_ptr(p)
    ,funFiter(nullptr)
    {
        p->connect(saValueManager,&SAValueManager::dataAdded
                   ,p,&SAValueManagerListModel::onDataChanged);
        p->connect(saValueManager,&SAValueManager::dataRemoved
                   ,p,&SAValueManagerListModel::onDataChanged);
        p->connect(saValueManager,&SAValueManager::dataClear
                   ,p,&SAValueManagerListModel::onDataClear);
    }
};

SAValueManagerListModel::SAValueManagerListModel(QObject *parent)
    : QAbstractListModel(parent)
    ,d_ptr(new SAValueManagerListModelPrivate(this))
{
    updateData();
}

SAValueManagerListModel::~SAValueManagerListModel()
{

}

int SAValueManagerListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d_ptr->valIndex.size();
}


QVariant SAValueManagerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(Qt::DisplayRole == role)
    {
        if(index.row() < d_ptr->valIndex.size())
        {
            return saValueManager->nameAt(d_ptr->valIndex[index.row()]);
        }
    }
    else if(Qt::DecorationRole == role)
    {
        if(index.row() < d_ptr->valIndex.size())
        {
            int i = d_ptr->valIndex[index.row()];
            SAAbstractDatas* p = saValueManager->at(i);
            if(p)
            {
                return saValueManager->getDataIcon(p->getType());
            }
        }
    }
    return QVariant();
}
///
/// \brief 设置过滤器，过滤器会按照规则过滤不显示的条目
/// 过滤器是一个bool(const SAAbstractDatas*,int)的函数指针，返回true表示接受，false表示不接受
/// 参数const SAAbstractDatas*是遍历的变量指针，参数int是变量的索引号
/// \param filterFun
///
void SAValueManagerListModel::setFilter(SAValueManagerListModel::FunPtrFilter filterFun)
{
    d_ptr->funFiter = filterFun;
    updateData();
}

void SAValueManagerListModel::onDataChanged(const QList<SAAbstractDatas *> &datas)
{
    Q_UNUSED(datas);
    updateData();
}

void SAValueManagerListModel::onDataClear()
{
    d_ptr->valIndex.clear();
    updateData();
}

void SAValueManagerListModel::updateData()
{
    beginResetModel();
    const int size = saValueManager->count();
    d_ptr->valIndex.reserve(size);

    if(nullptr != d_ptr->funFiter)
    {
        for(int i=0;i<size;++i)
        {
            if(d_ptr->funFiter(saValueManager->at(i),i))
            {
                d_ptr->valIndex.append(i);
            }
        }
    }
    else
    {
        for(int i=0;i<size;++i)
        {
            d_ptr->valIndex.append(i);
        }
    }
    endResetModel();
}

SAAbstractDatas *SAValueManagerListModel::castToDataPtr(const QModelIndex &index) const
{
    if(index.row() < d_ptr->valIndex.size())
    {
        int vali = d_ptr->valIndex.at(index.row());
        return saValueManager->at(vali);
    }
    return nullptr;
}
///
/// \brief 只保留1维数据
/// \param p
/// \param index
/// \return
///
bool filterSAValueManagerListModel1Dim(const SAAbstractDatas *p, int index)
{
    Q_UNUSED(index);
    if(p)
    {
        if(SA::Dim1 == p->getDim())
        {
            return true;
        }
    }
    return false;
}
