#include "SADataFeatureTreeModel.h"
#include "SAFigureWindow.h"
#include "SAVariantCaster.h"
#include "SAChart2D.h"
//#define DEBUG_OUTPUT__
//#define DEBUG_OUTPUT__INDEX_
//#define DEBUG_OUTPUT__PARENT_
//#define DEBUG_OUTPUT__ROW_COUNT_
#ifdef DEBUG_OUTPUT__
#include <QDebug>
#endif
#ifndef ROLE_FIGURE_PTR
#define ROLE_FIGURE_PTR (Qt::UserRole+2)
#endif
#ifndef ROLE_PLOT_ITEM_PTR
#define ROLE_PLOT_ITEM_PTR (Qt::UserRole+3)
#endif
SADataFeatureTreeModel::SADataFeatureTreeModel(QObject *parent) : QAbstractItemModel(parent)
{

}

QModelIndex SADataFeatureTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0)
        return QModelIndex();

    if(!parent.isValid ())//说明是顶层
    {
        if((row >= m_items.size())
            ||
            (column >= 2))
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
        || (column >= 2))
    {
        return QModelIndex();//不正常情况
    }
    return createIndex(row, column, parItem->getChild(row));
}

QModelIndex SADataFeatureTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QModelIndex();
    }
    SADataFeatureItem* item =  toItemPtr(index);
    if(!item)
    {
        return QModelIndex();
    }
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

int SADataFeatureTreeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        return m_items.size();
    }
    SADataFeatureItem* parItem = toItemPtr(parent);
    return parItem ? parItem->getChildCount() : 0;
}

int SADataFeatureTreeModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SADataFeatureTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags SADataFeatureTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant SADataFeatureTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    SADataFeatureItem* item = toItemPtr(index);
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
            const QString& tmp = item->getName();
            if(tmp.isEmpty())
            {
                if(!index.parent().isValid())
                {
                    return tr("chart %1").arg(index.row()+1);//如果图没有命名，默认按照顺序命名
                }
            }
            return tmp;
        }
        case 1:
            return SAVariantCaster::variantToString(item->getValue());
        }
    }
    else if(Qt::BackgroundRole == role)
    {
        return item->getBackground();
    }

    return QVariant();
}



void SADataFeatureTreeModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

bool SADataFeatureTreeModel::takeRootItem(SADataFeatureItem *item)
{
    bool ret = false;
    beginResetModel();
    ret = m_items.removeOne(item);
    SAChart2D* chart = getChartPtrFromItem(item);
    m_fig2item.remove(chart);
    endResetModel();
    return ret;
}

void SADataFeatureTreeModel::setPlotItem(SAChart2D *chartPtr, QwtPlotItem *itemPtr, SADataFeatureItem *items)
{
    SADataFeatureItem *itemChart = findChartItem(chartPtr,true);
    if(nullptr == itemChart)
    {
        return;
    }
    beginResetModel();
    QVariant var = QVariant::fromValue<quintptr>((quintptr)itemPtr);
    items->setData(var,ROLE_PLOT_ITEM_PTR);
    const int childCount = itemChart->getChildCount();
    int replaceItemIndex = -1;
    for(int i=0;i<childCount;++i)
    {
        QVariant v = itemChart->getChild(i)->getData(ROLE_PLOT_ITEM_PTR);
        if(v.isValid())
        {
            if(v.canConvert<quintptr>())
            {
                quintptr ip = v.value<quintptr>();
                if(ip == (quintptr)itemPtr)
                {
                    replaceItemIndex = i;
                }
            }
        }
    }
    if(replaceItemIndex >= 0)
    {
        SADataFeatureItem * oldItem = itemChart->setChild(replaceItemIndex,items);
        if(oldItem)
        {
            delete oldItem;
        }
    }
    else
    {
        itemChart->appendItem(items);
    }
    endResetModel();
}

SADataFeatureItem *SADataFeatureTreeModel::createChartItems(SAChart2D *chart)
{
    beginResetModel();
    SADataFeatureItem* figItem = new SADataFeatureItem(chart->title().text());
    figItem->setData(QVariant::fromValue<quintptr>((quintptr)chart),ROLE_FIGURE_PTR);
    m_fig2item[chart] = figItem;
    m_items.append(figItem);
    endResetModel();
    return figItem;
}

///
/// \brief 通过figure指针查找到对应的item条目，figure指针对应以第一层item
/// \param p
/// \param autoCreate 如果没找到对应的item，自动创建一个，默认为false
/// \return 如果没有对应的item，返回nullptr
///
SADataFeatureItem *SADataFeatureTreeModel::findChartItem(SAChart2D *p, bool autoCreate)
{
    SADataFeatureItem *item = m_fig2item.value(p,nullptr);
    if(autoCreate)
    {
       if(nullptr == item)
       {
           item = createChartItems(p);
       }
    }
    return item;
}
///
/// \brief 通过figure指针查找到对应的item条目，figure指针对应以第一层item
/// \param p
/// \return
///
SADataFeatureItem *SADataFeatureTreeModel::findChartItem(SAChart2D *p)
{
    SADataFeatureItem *item = m_fig2item.value(p,nullptr);
    return item;
}
///
/// \brief 获取顶层节点
/// \return
///
QList<SADataFeatureItem *> SADataFeatureTreeModel::getRootItems() const
{
    return m_items;
}
///
/// \brief 通过节点获取图形指针
/// \param item
/// \return
///
SAChart2D *SADataFeatureTreeModel::getChartPtrFromItem(const SADataFeatureItem *item)
{
    QVariant v = item->getData(ROLE_FIGURE_PTR);
    if(v.isValid())
    {
        if(v.canConvert<quintptr>())
        {
            quintptr p = v.value<quintptr>();
            return (SAChart2D *)p;
        }
    }
    return nullptr;
}
///
/// \brief 通过节点获取对应的itemlist
/// \param item 必须是getRootItems 获取的顶层节点或者findChartItem查找的顶层节点
/// \return
///
QList<QwtPlotItem *> SADataFeatureTreeModel::getItemListFromItem(const SADataFeatureItem *item)
{
    QList<QwtPlotItem *> res;
    QVariant var = item->getData(ROLE_FIGURE_PTR);
    if(!var.isValid())
    {
        return res;
    }
    if(!var.canConvert<quintptr>())
    {
        return res;
    }
    int childCount = item->getChildCount();
    for(int i=0;i<childCount;++i)
    {
        QVariant v = item->getChild(i)->getData(ROLE_PLOT_ITEM_PTR);
        if(v.isValid())
        {
            if(v.canConvert<quintptr>())
            {
                quintptr p = v.value<quintptr>();
                res.append((QwtPlotItem *)p);
            }
        }
    }
    return res;
}

QSet<QwtPlotItem *> SADataFeatureTreeModel::getItemSetFromItem(const SADataFeatureItem *item)
{
    QSet<QwtPlotItem *> res;
    QVariant var = item->getData(ROLE_FIGURE_PTR);
    if(!var.isValid())
    {
        return res;
    }
    if(!var.canConvert<quintptr>())
    {
        return res;
    }
    int childCount = item->getChildCount();
    for(int i=0;i<childCount;++i)
    {
        QVariant v = item->getChild(i)->getData(ROLE_PLOT_ITEM_PTR);
        if(v.isValid())
        {
            if(v.canConvert<quintptr>())
            {
                quintptr p = v.value<quintptr>();
                res.insert((QwtPlotItem *)p);
            }
        }
    }
    return res;
}


SADataFeatureItem *SADataFeatureTreeModel::toItemPtr(const QModelIndex &index) const
{
    return static_cast<SADataFeatureItem *>(index.internalPointer());
}

