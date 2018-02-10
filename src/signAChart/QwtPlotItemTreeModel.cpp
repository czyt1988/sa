#include "QwtPlotItemTreeModel.h"
#include "SAChart.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>
QwtPlotItemTreeModel::QwtPlotItemTreeModel(QObject* parent):QAbstractItemModel(parent)
{
    m_plot.clear ();
}

QwtPlotItemTreeModel::~QwtPlotItemTreeModel()
{

}

void QwtPlotItemTreeModel::addPlot(QwtPlot* plot)
{
    beginResetModel();
    m_plot.append (plot);
    endResetModel();
}

void QwtPlotItemTreeModel::setPlots(const QList<QwtPlot *> &plot)
{
    beginResetModel();
    m_plot = plot;
    endResetModel();
}

void QwtPlotItemTreeModel::clear()
{
    beginResetModel();
    m_plot.clear ();
    endResetModel();
}

QModelIndex QwtPlotItemTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (row < 0 || column < 0)
        return QModelIndex();
    if(!parent.isValid ())//说明是顶层
    {
        if(row >= m_plot.size ())
            return QModelIndex();
        return createIndex(row, column, m_plot[row]);
    }
    QwtPlot* qwt = static_cast<QwtPlot*>(parent.internalPointer ());
    if(m_plot.indexOf (qwt) != -1) //顶层是qwtplot
    {
        QList<QwtPlotItem*> items = getItems(qwt);
        if (row >= items.size ())
            return QModelIndex();
        return createIndex(row, column, items[row]);
    }
    return QModelIndex();
}

QModelIndex QwtPlotItemTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();
    QwtPlot* qwt = static_cast<QwtPlot*>(index.internalPointer ());
    int qwtIndex = m_plot.indexOf (qwt);
    if (-1 != qwtIndex)//说明有这个指针
        return QModelIndex();
    QwtPlotItem* item = static_cast<QwtPlotItem*>(index.internalPointer ());
    if(item)
    {
       qwt = item->plot ();
       int row = m_plot.indexOf (qwt);
       if(row<0)
           return QModelIndex();
       return createIndex(row, 0, qwt);
    }
    return QModelIndex();
}

int QwtPlotItemTreeModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid ())
    {
        QwtPlot* qwt = static_cast<QwtPlot*>(parent.internalPointer ());
        int qwtIndex = m_plot.indexOf (qwt);
        if (-1 != qwtIndex)//说明有这个指针
            return getItems(qwt).size ();
        return 0;
    }
    return m_plot.size ();
}

int QwtPlotItemTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant QwtPlotItemTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        switch(section)
        {
        case 0:return tr("item");//条目
        }
    }
    return QVariant();
}

Qt::ItemFlags QwtPlotItemTreeModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant QwtPlotItemTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (m_plot.size () <= 0)
        return QVariant();
    if (role == Qt::TextAlignmentRole)
    { //返回的是对其方式
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole) //返回的是现实内容
    {
        QwtPlot* qwt = getQwtPlotFromIndex(index);
        if(qwt)
        {
             QString str = qwt->title ().text ();
             if(str.isEmpty ())
                 return tr("figure-%1").arg(m_plot.indexOf (qwt)+1);//图
             return str;
        }

        QwtPlotItem* item = getQwtPlotItemFromIndex(index);
        if(item)
        {
            QString str = item->title ().text ();
            if(str.isEmpty ())
                return tr("item-%1").arg(item->plot ()->itemList ().indexOf (item)+1);//图元
            return str;
        }
    }
    else if (role == Qt::DecorationRole)
    {
        return displayDecorationRole(index);
    }
    else if(role == Qt::BackgroundRole)
    {
        QwtPlotItem* item = getQwtPlotItemFromIndex(index);
        if(item)
        {
            if(1 == index.column())
                return getColorFromItem(item);//现在暂时想第二列是颜色
            return getColorFromItem(item,30);
        }
        return QVariant();
    }
    return QVariant();
}
///
/// \brief 通过QModelIndex获取QwtPlot*
/// \param index
/// \return 如果不是对应关联，返回nullptr
///
QwtPlot* QwtPlotItemTreeModel::getQwtPlotFromIndex(const QModelIndex& index) const
{
    if(index.parent ().isValid ())//QwtPlot是顶层，没有父级
        return nullptr;
    QwtPlot* qwt = static_cast<QwtPlot*>(index.internalPointer ());
    if (-1 != m_plot.indexOf (qwt))//说明有这个指针
        return qwt;
    return nullptr;
}
///
/// \brief 通过index获取QwtPlotItem*
/// \param index
/// \return 不进行检测，请确保正确性
///
QwtPlotItem* QwtPlotItemTreeModel::getQwtPlotItemFromIndex(const QModelIndex& index) const
{
    if(!index.parent ().isValid ())//QwtPlot是顶层，没有父级
        return nullptr;
    return static_cast<QwtPlotItem*>(index.internalPointer ());
}



QVariant QwtPlotItemTreeModel::getColorFromItem(const QwtPlotItem* item,int alpha) const
{
    QColor c = SAChart::getItemColor(item,Qt::transparent);
    if(alpha<255)
        c.setAlpha(alpha);
    return c;
}

QVariant QwtPlotItemTreeModel::displayDecorationRole(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return QVariant();
}
///
/// \brief 获取plot的所有item，若想过滤不需要的item，可以重载此函数
/// \param plot
/// \param filter
/// \return
///
QList<QwtPlotItem*> QwtPlotItemTreeModel::getItems(QwtPlot* plot) const
{
    QList<QwtPlotItem*> items = plot->itemList ();
    return items;
}

