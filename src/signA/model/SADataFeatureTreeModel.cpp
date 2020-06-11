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
        if((row >= m_2dcharts.size())
            ||
            (column >= 2))
        {
            return QModelIndex();
        }
        return createIndex(row, column, m_2dcharts[row]);//顶层节点
    }
    //二层或者三层
    QModelIndex grapar = parent.parent();
    if(!grapar.isValid())
    {
        //说明是二层
        if((column >= 2) || (grapar.row() >= m_2dcharts.size()))
        {
            return QModelIndex();
        }
        SAChart2D* chart = static_cast<SAChart2D*>(parent.internalPointer());
        QwtPlotItemList items = filterCanDisplayItems(chart->itemList());
        if(row >= items.size())
        {
            return QModelIndex();
        }
        return createIndex(row, column, items[row]);//2层节点
    }
    QModelIndex gragrapar = grapar.parent();
    if(!gragrapar.isValid())
    {
        //说明是3层
        SAChart2D* chart = static_cast<SAChart2D*>(grapar.internalPointer());
        QwtPlotItemList items = filterCanDisplayItems(chart->itemList());
        if(parent.row() >= items.size())
        {
            return QModelIndex();
        }
        QwtPlotItem* item = static_cast<QwtPlotItem*>(items[parent.row()]);
        auto ite = m_plotitemFeatures.find(item);
        if(ite == m_plotitemFeatures.end())
        {
            return QModelIndex();
        }
        const QList<ItemPtr::element_type*>& itsp = ite.value();
        if(row > itsp.size() || column >= 2)
        {
            return QModelIndex();
        }
        return createIndex(row, column, itsp[row]);//3层节点
    }
    return QModelIndex();
}

QModelIndex SADataFeatureTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QModelIndex();
    }
    void* p = index.internalPointer();
    if(isChart2DPtr(p))
    {
        //说明是顶层SAChart2D*，返回无效的index
        return QModelIndex();
    }

    if(isQwtPlotItemPtr(p))
    {
        //是QwtPlotItem
        SAChart2D* chart = static_cast<SAChart2D*>(reinterpret_cast<QwtPlotItem*>(p)->plot());
        return createIndex(m_2dcharts.indexOf(chart),0,chart);
    }

    auto ite = m_featureToPlotitem.find(reinterpret_cast<ItemPtr::element_type*>(p));
    if(ite != m_featureToPlotitem.end())
    {
        QwtPlotItem* item = ite.value();
        SAChart2D* chart = static_cast<SAChart2D*>(item->plot());
        int i = filterCanDisplayItems(chart->itemList()).indexOf(item);
        if(i >= 0)
        {
            return createIndex(i,0,item);
        }
    }
    return QModelIndex();
}

int SADataFeatureTreeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid ())//说明是顶层
    {
        return m_2dcharts.size();
    }
    //二层或者三层
    QModelIndex grapar = parent.parent();
    if(!grapar.isValid())
    {
        //说明是二层
        if(grapar.row() >= m_2dcharts.size())
        {
            return 0;
        }
        SAChart2D* chart = static_cast<SAChart2D*>(parent.internalPointer());
        return filterCanDisplayItems(chart->itemList()).size();
    }
    QModelIndex gragrapar = grapar.parent();
    if(!gragrapar.isValid())
    {
        //说明是3层
        SAChart2D* chart = static_cast<SAChart2D*>(grapar.internalPointer());
        QwtPlotItemList items = filterCanDisplayItems(chart->itemList());
        if(parent.row() >= items.size())
        {
            return 0;
        }
        QwtPlotItem* item = static_cast<QwtPlotItem*>(items[parent.row()]);
        auto ite = m_plotitemFeatures.find(item);
        if(ite == m_plotitemFeatures.end())
        {
            return 0;
        }
        return ite.value().size();
    }
    return 0;
}

int SADataFeatureTreeModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid ())//说明是顶层
    {
        return 1;
    }
    //二层或者三层
    QModelIndex grapar = parent.parent();
    if(!grapar.isValid())
    {
        //说明是二层
        if(grapar.row() >= m_2dcharts.size())
        {
            return 0;
        }
        return 1;
    }
    QModelIndex gragrapar = grapar.parent();
    if(!gragrapar.isValid())
    {
        //说明是3层
        return 2;
    }
    return 0;
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
    SADataFeatureItem* item = toPtr(index);
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
    m_2dcharts.clear();
    m_features.clear();
    m_plotitemFeatures.clear();
    m_featureToPlotitem.clear();
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
            QList<ItemPtr> items = i.value();
            for(ItemPtr it : items)
            {
                m_featureToPlotitem.remove(it);
                m_features.remove(it);
            }
        }
        m_plotitemFeatures.remove(item);
    }
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
}

/**
 * @brief 筛选可显示的items
 * @param its
 * @return
 */
QwtPlotItemList SADataFeatureTreeModel::filterCanDisplayItems(const QwtPlotItemList &its)
{
    QwtPlotItemList res;
    for(QwtPlotItem* i : its)
    {
        if(isPlotitemCanDisplay(i))
        {
            res.append(i);
        }
    }
    return res;
}

/**
 * @brief 判断QwtPlotItem是否允许显示
 * @param item
 * @return
 */
bool SADataFeatureTreeModel::isPlotitemCanDisplay(QwtPlotItem *item)
{
    switch (item->rtti()) {
    case QwtPlotItem::Rtti_PlotCurve:
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    case QwtPlotItem::Rtti_PlotScale:
    case QwtPlotItem::Rtti_PlotBarChart:
    case QwtPlotItem::Rtti_PlotHistogram:
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    case QwtPlotItem::Rtti_PlotSpectrogram:
    case QwtPlotItem::Rtti_PlotTradingCurve:
    case QwtPlotItem::Rtti_PlotShape:
        return true;
    default:
        break;
    }
    return false;
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

/**
 * @brief 判断是否记录的chart2d指针
 * @param p
 * @return
 */
bool SADataFeatureTreeModel::isChart2DPtr(void *p) const
{
    for(SAChart2D* v : m_2dcharts)
    {
        if(v == p)
            return true;
    }
    return false;
}

/**
 * @brief 判断是否QwtPlotItem指针
 * @param p
 * @return
 */
bool SADataFeatureTreeModel::isQwtPlotItemPtr(void *p) const
{
    for(SAChart2D* v : m_2dcharts)
    {
        if(v == p)
        {
            return false;
        }
        const QwtPlotItemList& ils = v->itemList();
        for(QwtPlotItem* i : ils)
        {
            if(i == p)
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief 刷新数据，会把绘图（SAChart2D*）的数据进行更新，但item数据不进行修订
 */
void SADataFeatureTreeModel::reflashData()
{
    //clear在null判断前面，以防止跳过清除
    clear();
    if(nullptr == m_fig)
    {
        return;
    }
    //匹配m_plotitemFeatures，把不存在的plot删除
    m_2dcharts = m_fig->get2DPlots();
}

