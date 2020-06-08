#include "SADataFeatureTreeModel.h"
#include <QSet>
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
SADataFeatureTreeModel::SADataFeatureTreeModel(SAFigureWindow* fig,QObject *parent) : QAbstractItemModel(parent)
  ,m_fig(nullptr)
{
    setFigure(fig);
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
    Q_UNUSED(parent);
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
    if(!index.parent().isValid())
    {
        //第1层
        //说明是chart
        if(index.row() < m_2dcharts.size())
        {
            //说明是2d图形，如果有3d图形，会在后面继续
            SAChart2D* chart = static_cast<SAChart2D*>(index.internalPointer());
            if(Qt::DisplayRole == role && 0 == index.column())
            {
                QString title = chart->title().text();
                if(title.isEmpty())
                {
                    title = tr("figure%1").arg(index.row()+1);
                }
                return title;
            }
        }
    }
    else if(index.parent().parent().isValid())
    {
        //第2层
        //说明是plotitem层
        QwtPlotItem* item = static_cast<QwtPlotItem*>(index.internalPointer());
        if(Qt::DisplayRole == role)
        {
            QString title = item->title().text();
            if(title.isEmpty())
            {
                title = tr("figure%1").arg(index.row()+1);
            }
            return title;
        }
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
    m_fig = nullptr;
    m_items.clear();
    m_2dcharts.clear();
    endResetModel();
}

/**
 * @brief 设置figure
 *
 * 此函数会绑定绘图窗口的添加删除信号
 * @param fig
 *
 */
void SADataFeatureTreeModel::setFigure(SAFigureWindow *fig)
{
    clear();
    if(m_fig)
    {
        //解除绑定
        disconnect(m_fig,&SAFigureWindow::chartAdded,this,&SADataFeatureTreeModel::onChartAdded);
        disconnect(m_fig,&SAFigureWindow::chartRemoved,this,&SADataFeatureTreeModel::onChartRemoved);
    }
    m_fig = fig;
    if(m_fig)
    {
        //建立新绑定
        connect(m_fig,&SAFigureWindow::chartAdded,this,&SADataFeatureTreeModel::onChartAdded);
        connect(m_fig,&SAFigureWindow::chartRemoved,this,&SADataFeatureTreeModel::onChartRemoved);
    }
    reflashData();
}


/**
 * @brief SADataFeatureTreeModel::setPlotItem
 * @param chartPtr
 * @param itemPtr
 * @param items
 */
void SADataFeatureTreeModel::setPlotItem(QwtPlotItem *plotitem, std::shared_ptr<SAItem> item)
{
    Q_CHECK_PTR(plotitem);
    beginResetModel();
    m_features.insert(item);
    auto il = m_plotitemFeatures.find(plotitem);
    if(il == m_plotitemFeatures.end())
    {
        il = m_plotitemFeatures.insert(plotitem,QList<SAItem*>());
    }
    il.value().append(item.get());
    m_featureToPlotitem[item.get()] = plotitem;
    endResetModel();
}


/**
 * @brief figure 有绘图窗口加入触发
 * @param plot
 */
void SADataFeatureTreeModel::onChartAdded(QwtPlot *plot)
{
    Q_UNUSED(plot);
    beginResetModel();
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
}

/**
 * @brief figure 有绘图窗口移除触发
 * @param plot
 */
void SADataFeatureTreeModel::onChartRemoved(QwtPlot *plot)
{
    beginResetModel();
    //触发此信号，plot还未销毁
    QwtPlotItemList itemlist = plot->itemList();
    for(QwtPlotItem* item : itemlist)
    {
        auto i = m_plotitemFeatures.find(item);
        if(i != m_plotitemFeatures.end())
        {
            QList<SAItem*> items = i.value();
            for(SAItem* it : items)
            {
                m_featureToPlotitem.remove(it);
            }
        }
    }
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
}

/**
 * @brief 根据指针，返回item的名字
 * @param item
 * @return
 */
QString SADataFeatureTreeModel::plotitemToTitleName(QwtPlotItem *item)
{
    switch (item->rtti()) {
    case QwtPlotItem::Rtti_PlotCurve:
        return tr("curve");
    case QwtPlotItem::Rtti_PlotMultiBarChart:
        return tr("mult bar chart");
    case QwtPlotItem::Rtti_PlotScale:
        return tr("scale");
    case QwtPlotItem::Rtti_PlotBarChart:
        return tr("bar");
    case QwtPlotItem::Rtti_PlotHistogram:
        return tr("histogram");
    case QwtPlotItem::Rtti_PlotSpectroCurve:
        return tr("spectro curve");
    case QwtPlotItem::Rtti_PlotIntervalCurve:
        return tr("interval curve");
    case QwtPlotItem::Rtti_PlotSpectrogram:
        return tr("spectrogram");
    case QwtPlotItem::Rtti_PlotTradingCurve:
        return tr("trading curve");
    default:
        break;
    }
    return tr("item");
}


///
/// \brief 通过节点获取图形指针
/// \param item
/// \return
///
SAChart2D *SADataFeatureTreeModel::getChartPtrFromItem(SAItem *item)
{
    SAItem* top = item;
    while (nullptr != item->parent()) {
        top = item->parent();
    }
    if(top)
    {
        auto i = m_featureToPlotitem.find(top);
        if(i != m_featureToPlotitem.end())
        {
            return qobject_cast<SAChart2D*>(i.value()->plot());
        }
    }
    return nullptr;
}



SADataFeatureItem *SADataFeatureTreeModel::toItemPtr(const QModelIndex &index) const
{
    return static_cast<SADataFeatureItem *>(index.internalPointer());
}

/**
 * @brief 刷新数据，会把绘图（SAChart2D*）的数据进行更新，但item数据不进行修订
 */
void SADataFeatureTreeModel::reflashData()
{
    if(nullptr == m_fig)
    {
        return;
    }
    //匹配m_plotitemFeatures，把不存在的plot删除
    QSet<QwtPlotItem*> plotitems = m_plotitemFeatures.keys().toSet();
    m_2dcharts = m_fig->get2DPlots();
    for(SAChart2D* chart : m_2dcharts)
    {
        QwtPlotItemList items = chart->itemList();
        for(QwtPlotItem* i : items)
        {
            if(!plotitems.contains(i))
            {
                m_plotitemFeatures.remove(i);
            }
        }
    }

}

