#include "QwtPlotItemDataModel.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_plot_barchart.h>
#include "SAChart.h"

QwtPlotItemDataModel::QwtPlotItemDataModel(QObject* p)
    :QAbstractTableModel(p)
    ,m_rowCount(0)
    ,m_enableBkColor(true)
    ,m_bkAlpha(30)
{
}

void QwtPlotItemDataModel::setQwtPlotItems(const QList<QwtPlotItem*>& items)
{
    beginResetModel();
    m_items = items;
    updateMaxRow();
    updateColumnCount();
    updateItemColor();
    endResetModel();
}



void QwtPlotItemDataModel::clear()
{
    beginResetModel();
    m_rowCount = 0;
    m_items.clear();
    m_itemsRowCount.clear();
    m_itemsColor.clear();
    m_itemsColumnStartIndex.clear();
    m_columnMap.clear();
    endResetModel();
}

int QwtPlotItemDataModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_rowCount;
}

int QwtPlotItemDataModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_columnMap.size();
}

QVariant QwtPlotItemDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if(Qt::Horizontal == orientation)
    {//说明是水平表头
        int index = 0;
        QwtPlotItem* item = getItemFromCol(section,&index);
        qDebug("headerData index:%d",index);
        if(!item)
            return QVariant();  
        QString name = QStringLiteral("%1\n%2")
                           .arg(item->title ().text ())
                           .arg(getItemDimDescribe(item,index));
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
        QwtPlotItem* item = getItemFromCol(index.column());
        if(!item)
            return QVariant();
        if(index.row() >= getItemRowCount(item))
            return QVariant();
        return getItemData(index.row(),index.column());
    }
    else if(role == Qt::BackgroundColorRole)
    {
        if(!m_enableBkColor)
            return QVariant();
        QwtPlotItem* item = getItemFromCol(index.column ());
        if(!item)
            return QVariant();
        return getItemColor(item);
    }
    return QVariant();
}

void QwtPlotItemDataModel::enableBackgroundColor(bool enable, int alpha)
{
    beginResetModel();
    m_enableBkColor = enable;
    m_bkAlpha = alpha;
    if(m_enableBkColor)
    {
        updateItemColor();
    }
    endResetModel();
}

double QwtPlotItemDataModel::nan()
{
    return std::numeric_limits<double>::quiet_NaN();
}

void QwtPlotItemDataModel::updateMaxRow()
{
    m_rowCount = 0;
    m_itemsRowCount.clear();
    for(auto i = m_items.begin ();i!=m_items.end ();++i)
    {
        int dataCount = calcItemDataRowCount(*i);
        if(dataCount>m_rowCount)
        {
            m_rowCount = dataCount;
        }
        m_itemsRowCount[*i] = dataCount;
    }
}

void QwtPlotItemDataModel::updateColumnCount()
{
    m_columnMap.clear();
    m_itemsColumnStartIndex.clear();
    for(auto i = m_items.begin ();i!=m_items.end ();++i)
    {
        int dim = calcItemDataColumnCount(*i);
        qDebug("updateColumnCount : %u ,dim:%d",(uint)*i,dim);
        int startIndex = m_columnMap.size();
        m_itemsColumnStartIndex[*i] = startIndex;
        for(int d=0;d<dim;++d)
        {
            qDebug("updateColumnCount : %u ,col:%d",(uint)*i,m_columnMap.size()+d);
            m_columnMap[startIndex+d] = qMakePair<QwtPlotItem*,int>(*i,d);
        }
    }
}

void QwtPlotItemDataModel::updateItemColor()
{
    m_itemsColor.clear();
    for(auto i = m_items.begin ();i!=m_items.end ();++i)
    {
        QColor c = SAChart::dynamicGetItemColor(*i);
        if(m_bkAlpha < 255)
        {
            c.setAlpha(m_bkAlpha);
        }
        m_itemsColor[*i] = c;
    }
}



///
/// \brief 通过列号获取item
/// \param col 列的序号
/// \return 如果没有，返回nullptr
///
QwtPlotItem*QwtPlotItemDataModel::getItemFromCol(int col,int* dataColumnDim) const
{
    QPair<QwtPlotItem*,int> pair = m_columnMap.value(col,qMakePair<QwtPlotItem*,int>(nullptr,0));
    if(dataColumnDim)
    {
        *dataColumnDim = pair.second;
    }
    return pair.first;
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
    {
        return item->title ().text ();
    }
    return QString();
}
///
/// \brief 获取最大的行数
/// 重载此函数，可以让表格显示的行数
/// \param item
/// \return
///
int QwtPlotItemDataModel::calcItemDataRowCount(QwtPlotItem* item)
{
    return SAChart::dynamicGetPlotChartItemDataCount(item);
}
///
/// \brief 获取曲线的维度
///
/// \param item
/// \return
///
int QwtPlotItemDataModel::calcItemDataColumnCount(QwtPlotItem *item)
{
    if (const QwtSeriesStore<QPointF>* p = dynamic_cast<const QwtSeriesStore<QPointF>*>(item))
    {
        Q_UNUSED(p);
        return 2;
    }
    else if(const QwtSeriesStore<QwtIntervalSample>* p = dynamic_cast<const QwtSeriesStore<QwtIntervalSample>*>(item))
    {
        Q_UNUSED(p);
        return 3;
    }
    else if(const QwtSeriesStore<QwtSetSample>* p = dynamic_cast<const QwtSeriesStore<QwtSetSample>*>(item))
    {
        int maxDim = 0;
        const int size = p->dataSize();
        for(int i=0;i<size;++i)
        {
            int s = p->sample(i).set.size();
            if(s > maxDim)
            {
                maxDim = s;
            }
        }
        return maxDim + 1;
    }
    else if(const QwtSeriesStore<QwtPoint3D>* p = dynamic_cast<const QwtSeriesStore<QwtPoint3D>*>(item))
    {
        Q_UNUSED(p);
        return 3;
    }
    else if(const QwtSeriesStore<QwtOHLCSample>* p = dynamic_cast<const QwtSeriesStore<QwtOHLCSample>*>(item))
    {
        Q_UNUSED(p);
        return 5;
    }
    return -1;
}

double QwtPlotItemDataModel::getItemData(int row, int col) const
{
    int dim = 0;
    QwtPlotItem* item = getItemFromCol(col,&dim);
    if(nullptr == item)
    {
        return nan();
    }
    int index = m_itemsColumnStartIndex.value(item,0);
    index = col - index;
    if(index < 0)
    {
        return nan();
    }
    //读取数据
    if (const QwtSeriesStore<QPointF>* p = dynamic_cast<const QwtSeriesStore<QPointF>*>(item))
    {
        if(row < p->dataSize())
        {
            switch(index)
            {
                case 0:return p->sample(row).x();
                case 1:return p->sample(row).y();
            }
        }
    }
    else if(const QwtSeriesStore<QwtIntervalSample>* p = dynamic_cast<const QwtSeriesStore<QwtIntervalSample>*>(item))
    {
        if(row < p->dataSize())
        {
            switch(index)
            {
                case 0:return p->sample(row).value;
                case 1:return p->sample(row).interval.minValue();
                case 2:return p->sample(row).interval.maxValue();
            }
        }
    }
    else if(const QwtSeriesStore<QwtSetSample>* p = dynamic_cast<const QwtSeriesStore<QwtSetSample>*>(item))
    {
        if(row < p->dataSize())
        {
            const QwtSetSample& s = p->sample(row);
            if(0 == index)
            {
                return s.value;
            }
            else
            {
                int setIndex = index - 1;
                if(setIndex >=0 && setIndex < s.set.size())
                {
                    return s.set.value(setIndex);
                }
            }

        }
    }
    else if(const QwtSeriesStore<QwtPoint3D>* p = dynamic_cast<const QwtSeriesStore<QwtPoint3D>*>(item))
    {
        if(row < p->dataSize())
        {
            switch(index)
            {
                case 0:return p->sample(row).x();
                case 1:return p->sample(row).y();
                case 2:return p->sample(row).z();
            }
        }
    }
    else if(const QwtSeriesStore<QwtOHLCSample>* p = dynamic_cast<const QwtSeriesStore<QwtOHLCSample>*>(item))
    {
        if(row < p->dataSize())
        {
            switch(index)
            {
                case 0:return p->sample(row).time;
                case 1:return p->sample(row).open;
                case 2:return p->sample(row).high;
                case 3:return p->sample(row).low;
                case 4:return p->sample(row).close;
            }
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

QString QwtPlotItemDataModel::getItemDimDescribe(QwtPlotItem *item, int index) const
{

    if (const QwtSeriesStore<QPointF>* p = dynamic_cast<const QwtSeriesStore<QPointF>*>(item))
    {
        Q_UNUSED(p);
        switch(index)
        {
        case 0:return tr("x");
        case 1:return tr("y");
        default:return QString();
        }
    }
    else if(const QwtSeriesStore<QwtIntervalSample>* p = dynamic_cast<const QwtSeriesStore<QwtIntervalSample>*>(item))
    {
        Q_UNUSED(p);
        switch(index)
        {
        case 0:return tr("value");
        case 1:return tr("min");
        case 2:return tr("max");
        default:return QString();
        }
    }
    else if(const QwtSeriesStore<QwtSetSample>* p = dynamic_cast<const QwtSeriesStore<QwtSetSample>*>(item))
    {
        Q_UNUSED(p);
        if(0 == index)
        {
            return tr("value");
        }
        else
        {
            return tr("set %1").arg(index);
        }
    }
    else if(const QwtSeriesStore<QwtPoint3D>* p = dynamic_cast<const QwtSeriesStore<QwtPoint3D>*>(item))
    {
        Q_UNUSED(p);
        switch(index)
        {
        case 0:return tr("x");
        case 1:return tr("y");
        case 2:return tr("z");
        default:return QString();
        }
    }
    else if(const QwtSeriesStore<QwtOHLCSample>* p = dynamic_cast<const QwtSeriesStore<QwtOHLCSample>*>(item))
    {

        switch(index)
        {
        case 0:return tr("time");
        case 1:return tr("open");
        case 2:return tr("high");
        case 3:return tr("low");
        case 4:return tr("close");
        }

    }
    return QString();
}

///
/// \brief 获取item的颜色
/// \param item
/// \return QColor
///
QColor QwtPlotItemDataModel::getItemColor(QwtPlotItem* item) const
{
    return m_itemsColor.value(item,QColor());
}

int QwtPlotItemDataModel::getItemRowCount(QwtPlotItem *item) const
{
    return m_itemsRowCount.value(item,0);
}




