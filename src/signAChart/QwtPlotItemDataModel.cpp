#include "QwtPlotItemDataModel.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_plot_barchart.h>
QwtPlotItemDataModel::QwtPlotItemDataModel(QObject* parent)
    :QAbstractTableModel(parent)
    ,m_max_row(0)
    ,m_columnCount(0)
    ,m_enableBkColor(true)
    ,m_bkAlpha(30)
{
}

void QwtPlotItemDataModel::setQwtPlotItems(const QList<QwtPlotItem*>& items)
{
    beginResetModel();
    m_items = items;
    updateMaxRow(items);
    m_columnCount = m_items.size ()*2;
    endResetModel();
}

void QwtPlotItemDataModel::addQwtPlotItems(QwtPlotItem* item)
{
    beginResetModel();
    m_items.append (item);
    updateMaxRow(QList<QwtPlotItem*>()<<item);
    m_columnCount = m_items.size ()*2;
    endResetModel();
}

void QwtPlotItemDataModel::clear()
{
    beginResetModel();
    m_items.clear();
    updateMaxRow(QList<QwtPlotItem*>());
    endResetModel();
}

int QwtPlotItemDataModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_max_row;
}

int QwtPlotItemDataModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_columnCount;
}

QVariant QwtPlotItemDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        QwtPlotItem* item = getItemFromCol(section);
        if(!item)
            return QVariant();
        QString name;
        if(isIntervalType(item))
        {
            name = QStringLiteral("%1-%2")
                           .arg(item->title ().text ())
                           .arg((section%2==0) ? "Value" : "Interval");
        }
        else
        {
            name = QStringLiteral("%1-%2")
                           .arg(item->title ().text ())
                           .arg((section%2==0) ? "X" : "Y");
        }
        return name;
    }
    else
    {
        return (section+1);
    }
    return QVariant();
}

QVariant QwtPlotItemDataModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole){ //返回的是对其方式
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    { //返回的是现实内容
        QwtPlotItem* item = getItemFromCol(index.column ());
        if(!item)
            return QVariant();
        if(index.row() >= getItemDataCount(item))
            return QVariant();
        return index.column()%2 == 0
        ? getItemDataX(item,index.row())
        : getItemDataY(item,index.row());//显示
    }
    else if(role == Qt::BackgroundColorRole)
    {
        if(!m_enableBkColor)
            return QVariant();
        QwtPlotItem* item = getItemFromCol(index.column ());
        if(!item)
            return QVariant();
        if(index.row() >= getItemDataCount(item))
            return QVariant();
        QColor bk = getItemColor(item);
        bk.setAlpha (m_bkAlpha);
        return bk;
    }
    return QVariant();
}

void QwtPlotItemDataModel::enableBackgroundColor(bool enable, int alpha)
{
    m_enableBkColor = enable;
    m_bkAlpha = alpha;
}

void QwtPlotItemDataModel::updateMaxRow(const QList<QwtPlotItem*>& items)
{
    for(auto i = items.begin ();i!=items.end ();++i)
    {
        int dataCount = getItemDataCount(*i);
        if(dataCount>m_max_row)
            m_max_row = dataCount;
    }
}

inline int QwtPlotItemDataModel::toIndex(int col) const
{
    return int(col/2);
}
///
/// \brief 通过列号获取item
/// \param col 列的序号
/// \return 如果没有，返回nullptr
///
QwtPlotItem*QwtPlotItemDataModel::getItemFromCol(int col) const
{
    int colIndex = toIndex(col);
    if (colIndex>=m_items.size ())
        return nullptr;
    return m_items[colIndex];
}
///
/// \brief 通过列号获取item的名字
/// \param col 列的序号
/// \return item的名字
///
QString QwtPlotItemDataModel::getItemNameFromCol(int col) const
{
    QwtPlotItem* item = getItemFromCol(col);
    if(item)
        return item->title ().text ();
    return QString();
}

int QwtPlotItemDataModel::getItemDataCount(QwtPlotItem* item)
{
    const int rtti = item->rtti();
    switch (rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve :
        {
            QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(item);
            return cur->data ()->size ();
        }
    case QwtPlotItem::Rtti_PlotHistogram:
        {
            QwtPlotHistogram* his = static_cast<QwtPlotHistogram*>(item);
            return his->data ()->size ();
        }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
        {
            QwtPlotIntervalCurve* plc = static_cast<QwtPlotIntervalCurve*>(item);
            return plc->data ()->size ();
        }
    }
    return 0;
}

QVariant QwtPlotItemDataModel::getItemDataX(QwtPlotItem* item, int index)
{
    const int rtti = item->rtti();
    switch (rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve :
        {
            QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(item);
            return cur->data ()->sample (index).x ();
        }
    case QwtPlotItem::Rtti_PlotHistogram:
        {
            QwtPlotHistogram* his = static_cast<QwtPlotHistogram*>(item);
            return his->data ()->sample (index).value;
        }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
        {
            QwtPlotIntervalCurve* plc = static_cast<QwtPlotIntervalCurve*>(item);
            return plc->data ()->sample (index).value;
        }
    case QwtPlotItem::Rtti_PlotBarChart:
        {
            QwtPlotBarChart* bar2 = static_cast<QwtPlotBarChart*>(item);
            return bar2->data ()->sample (index).x ();
        }
    }
    return 0;
}

QVariant QwtPlotItemDataModel::getItemDataY(QwtPlotItem* item, int index)
{
    const int rtti = item->rtti();
    switch (rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve :
    {
            QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(item);
            return cur->data ()->sample (index).y ();
    }
    case QwtPlotItem::Rtti_PlotHistogram:
        {
            QwtPlotHistogram* his = static_cast<QwtPlotHistogram*>(item);
            return QString("min:%1;max:%2").arg(his->data ()->sample (index).interval.minValue ())
                           .arg(his->data ()->sample (index).interval.maxValue ());
        }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
        {
            QwtPlotIntervalCurve* plc = static_cast<QwtPlotIntervalCurve*>(item);
            return QString("min:%1;max:%2").arg(plc->data ()->sample (index).interval.minValue ())
                    .arg(plc->data ()->sample (index).interval.maxValue ());
        }
    case QwtPlotItem::Rtti_PlotBarChart:
        {
            QwtPlotBarChart* bar2 = static_cast<QwtPlotBarChart*>(item);
            return bar2->data ()->sample (index).y ();
        }
    }
    return 0;
}
///
/// \brief 获取item的颜色
/// \param item
/// \return QColor
///
QColor QwtPlotItemDataModel::getItemColor(QwtPlotItem* item)
{
    const int rtti = item->rtti();
    switch (rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve :
    {
            QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(item);
            return cur->pen ().color ();
    }
    case QwtPlotItem::Rtti_PlotHistogram:
        {
            QwtPlotHistogram* his = static_cast<QwtPlotHistogram*>(item);
            return his->pen ().color ();
        }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
        {
            QwtPlotIntervalCurve* plc = static_cast<QwtPlotIntervalCurve*>(item);
            return plc->pen ().color ();
        }
    }
    return QColor(255,255,255);
}

bool QwtPlotItemDataModel::isIntervalType(QwtPlotItem* item)
{
    const int rtti = item->rtti();
    switch (rtti)
    {
    case QwtPlotItem::Rtti_PlotHistogram:
    case QwtPlotItem::Rtti_PlotIntervalCurve:
            return true;
    }
    return false;
}


