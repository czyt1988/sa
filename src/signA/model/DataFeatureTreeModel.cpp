#include "DataFeatureTreeModel.h"
#include "SAFigureWindow.h"
#include "SAVariantCaster.h"
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
DataFeatureTreeModel::DataFeatureTreeModel(QObject *parent) : QAbstractItemModel(parent)
{

}

QModelIndex DataFeatureTreeModel::index(int row, int column, const QModelIndex &parent) const
{
#ifdef DEBUG_OUTPUT__INDEX_
    qDebug() << "index:("<<row<<","<<column<<"),parent.valid:" << parent.isValid();
#endif
    if (row < 0 || column < 0)
        return QModelIndex();

    if(!parent.isValid ())//说明是顶层
    {
#ifdef DEBUG_OUTPUT__INDEX_
    qDebug() << "    root index";
#endif
        if((row >= m_items.size())
            ||
            (column >= 2))
        {
            return QModelIndex();
        }
#ifdef DEBUG_OUTPUT__INDEX_
    qDebug() << "    createIndex("<<row<<","<<column<<")";
#endif
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
#ifdef DEBUG_OUTPUT__INDEX_
    qDebug() << "    createIndex("<<row<<","<<column<<"):parItem->getChild(row)";
#endif
    return createIndex(row, column, parItem->getChild(row));
}

QModelIndex DataFeatureTreeModel::parent(const QModelIndex &index) const
{
#ifdef DEBUG_OUTPUT__PARENT_
    qDebug() << "parent:index.isvalid:"<<index.isValid();
#endif
    if(!index.isValid())
    {
        return QModelIndex();
    }
    SADataFeatureItem* item =  toItemPtr(index);
    if(!item)
    {
#ifdef DEBUG_OUTPUT__PARENT_
    qDebug() << "   toItemPtr:null";
#endif
        return QModelIndex();
    }
    SADataFeatureItem* parItem =  item->getParent();
    if(nullptr == parItem)
    {
        //父指针为0，说明是顶层item
#ifdef DEBUG_OUTPUT__PARENT_
    qDebug() << "   is no root item , top parent";
#endif
        return QModelIndex();
    }

    SADataFeatureItem* grandParItem = parItem->getParent();//祖父指针，这个比较关键
    if(nullptr == grandParItem)
    {//如果祖父为0，说明它是第二层级，parent是1层，但不能用它自身的parItem->row(), parItem->column()
     //需要在QLsit里查找它的层次
        int row = m_items.indexOf(parItem);
#ifdef DEBUG_OUTPUT__PARENT_
    qDebug() << "   is grandParItem: row:"<<row;
#endif
        if(row<0)
        {//说明没有在QList找到
            return QModelIndex();
        }
#ifdef DEBUG_OUTPUT__PARENT_
    qDebug() << "    createIndex("<<row<<",0"<<"):parItem->getChild(row)";
#endif
        return createIndex(row, 0, parItem);
    }
#ifdef DEBUG_OUTPUT__PARENT_
    qDebug() << "    createIndex("<<parItem->getCurrentRowIndex()<<",0"<<"):parItem->getChild(row)";
#endif
    return createIndex(parItem->getCurrentRowIndex(), 0, parItem);//挂载parent的都是只有一列的
}

int DataFeatureTreeModel::rowCount(const QModelIndex &parent) const
{
#ifdef DEBUG_OUTPUT__ROW_COUNT_
    qDebug()<<"rowCount:parent.isvalid:"<<parent.isValid() << ",m_items.size:"<<m_items.size();
#endif
    if(!parent.isValid())
    {
#ifdef DEBUG_OUTPUT__ROW_COUNT_
        qDebug()<<"  return:"<<m_items.size();
#endif
        return m_items.size();
    }
    SADataFeatureItem* parItem = toItemPtr(parent);
#ifdef DEBUG_OUTPUT__ROW_COUNT_
    if(nullptr == parItem)
    {
        qDebug()<<"  parItem is null";
    }
    else
    {
        qDebug()<<"  parent name:" <<parItem->getName()
               << "return:"<<parItem->getChildCount();
    }
#endif
    return parItem ? parItem->getChildCount() : 0;
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
            return item->getName();
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



void DataFeatureTreeModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

void DataFeatureTreeModel::setPlotItem(SAFigureWindow *p, QwtPlotItem *itemPtr, SADataFeatureItem *items)
{
    SADataFeatureItem *itemFig = findFigureItem(p,true);
    if(nullptr == itemFig)
    {
        return;
    }
    beginResetModel();
    QVariant var = QVariant::fromValue<quintptr>((quintptr)itemPtr);
    items->setData(var,ROLE_PLOT_ITEM_PTR);
    const int childCount = itemFig->getChildCount();
    int replaceItemIndex = -1;
    for(int i=0;i<childCount;++i)
    {
        QVariant v = itemFig->getChild(i)->getData(ROLE_PLOT_ITEM_PTR);
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
        SADataFeatureItem * oldItem = itemFig->setChild(replaceItemIndex,items);
        if(oldItem)
        {
            delete oldItem;
        }
    }
    else
    {
        itemFig->appendItem(items);
    }
    endResetModel();
}

SADataFeatureItem *DataFeatureTreeModel::createFigureItem(SAFigureWindow* figure)
{
    beginResetModel();
    SADataFeatureItem* figItem = new SADataFeatureItem(figure->windowTitle());
    figItem->setData(QVariant::fromValue<quintptr>((quintptr)figure),ROLE_FIGURE_PTR);
    m_fig2item[figure] = figItem;
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
SADataFeatureItem *DataFeatureTreeModel::findFigureItem(SAFigureWindow *p, bool autoCreate)
{
    SADataFeatureItem *item = m_fig2item.value(p,nullptr);
    if(autoCreate)
    {
       if(nullptr == item)
       {
           item = createFigureItem(p);
       }
    }
    return item;
}

SADataFeatureItem *DataFeatureTreeModel::toItemPtr(const QModelIndex &index) const
{
    return static_cast<SADataFeatureItem *>(index.internalPointer());
}

