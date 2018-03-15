#include "QwtPlotItemDataModel.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_barchart.h"
#include "qwt_plot_multi_barchart.h"
#include "qwt_plot_spectrocurve.h"
#include "qwt_plot_tradingcurve.h"
#include "SAChart.h"

QwtPlotItemDataModel::QwtPlotItemDataModel(QObject* p)
    :QAbstractTableModel(p)
    ,m_rowCount(0)
    ,m_enableBkColor(true)
    ,m_bkAlpha(30)
{
}

void QwtPlotItemDataModel::setPlotItems(const QList<QwtPlotItem*>& items)
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


        int col = m_itemsColumnStartIndex.value(item,0);
        col = index.column() - col;

        if(index.row() >= getItemRowCount(item))
            return QVariant();
        return getItemData(index.row(),col,item);
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

bool QwtPlotItemDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (Qt::EditRole != role)
        return false;
    QwtPlotItem* item = getItemFromCol(index.column());
    if(nullptr == item)
        return false;
    int col = getItemsColumnStartIndex(item);
    if(col < 0)
        return false;
    col = index.column() - col;
    return setItemData(index.row(),col,item,value);
}

Qt::ItemFlags QwtPlotItemDataModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
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
///
/// \brief 数据开始的那一列列号
/// \param item
/// \return 如果没有对应的item返回-1
///
int QwtPlotItemDataModel::getItemsColumnStartIndex(QwtPlotItem *item) const
{
    return m_itemsColumnStartIndex.value(item,-1);
}
///
/// \brief 获取item对应的列范围
/// \param startCol
/// \param endCol
///
void QwtPlotItemDataModel::getItemColumnRange(QwtPlotItem* item,int *startCol, int *endCol) const
{
    int startIndex = getItemsColumnStartIndex(item);
    if(startCol)
        *startCol = startIndex;
    if(endCol)
    {
        int dim2 = calcItemDataColumnCount(item);
        *endCol = startIndex+dim2-1;
    }
}
///
/// \brief 计算QwtPlotMultiBarChart的最大维度
/// \param p
/// \return QwtSetSample.set的最大尺寸+1，其中加1是对应QwtSetSample.value
///
int QwtPlotItemDataModel::calcPlotMultiBarChartDim(const QwtPlotMultiBarChart *p)
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
        qDebug() << "rtti:" << (*i)->rtti() << " dim:" << dim;
        if(dim > 0)
        {
            int startIndex = m_columnMap.size();
            m_itemsColumnStartIndex[*i] = startIndex;
            for(int d=0;d<dim;++d)
            {
                m_columnMap[startIndex+d] = qMakePair<QwtPlotItem*,int>(*i,d);
            }
        }
    }
}

void QwtPlotItemDataModel::updateItemColor()
{
    m_itemsColor.clear();
    for(auto i = m_items.begin ();i!=m_items.end ();++i)
    {
        QColor c = SAChart::getItemColor(*i);
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
int QwtPlotItemDataModel::calcItemDataRowCount(QwtPlotItem* item) const
{
    return SAChart::getPlotChartItemDataCount(item);
}

///
/// \brief 获取曲线的维度
///
/// \param item
/// \return
///
int QwtPlotItemDataModel::calcItemDataColumnCount(QwtPlotItem *item) const
{
#if QwtPlotItemDataModel_Use_Dynamic_Cast
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
#else
    int rtti = item->rtti();
    switch(rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve:
    case QwtPlotItem::Rtti_PlotBarChart:
        return 2;
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    case QwtPlotItem::Rtti_PlotHistogram:
        return 3;
    case QwtPlotItem::Rtti_PlotTradingCurve:
        return 5;
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        const QwtPlotMultiBarChart* p = static_cast<const QwtPlotMultiBarChart*>(item);
        return calcPlotMultiBarChartDim(p);
    }
    default:
        return 0;
    }
#endif
    return 0;
}

double QwtPlotItemDataModel::getItemData(int row, int col, QwtPlotItem *item) const
{
    if(nullptr == item || col < 0)
    {
        return nan();
    }
#if QwtPlotItemDataModel_Use_Dynamic_Cast
    //读取数据
    if (const QwtSeriesStore<QPointF>* p = dynamic_cast<const QwtSeriesStore<QPointF>*>(item))
    {
        if(row < p->dataSize())
        {
            switch(col)
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
            switch(col)
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
            if(0 == col)
            {
                return s.value;
            }
            else
            {
                int setIndex = col - 1;
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
            switch(col)
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
            switch(col)
            {
                case 0:return p->sample(row).time;
                case 1:return p->sample(row).open;
                case 2:return p->sample(row).high;
                case 3:return p->sample(row).low;
                case 4:return p->sample(row).close;
            }
        }
    }
#else
    int rtti = item->rtti();
    switch(rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        const QwtPlotCurve* p = static_cast<const QwtPlotCurve*>(item);
        if(row < p->dataSize())
        {
            switch(col)
            {
                case 0:return p->sample(row).x();
                case 1:return p->sample(row).y();
            }
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        const QwtPlotBarChart* p = static_cast<const QwtPlotBarChart*>(item);
        if(row < p->dataSize())
        {
            switch(col)
            {
                case 0:return p->sample(row).x();
                case 1:return p->sample(row).y();
            }
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        const QwtPlotSpectroCurve* p = static_cast<const QwtPlotSpectroCurve*>(item);
        if(row < p->dataSize())
        {
            switch(col)
            {
                case 0:return p->sample(row).x();
                case 1:return p->sample(row).y();
                case 2:return p->sample(row).z();
            }
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        const QwtPlotIntervalCurve* p = static_cast<const QwtPlotIntervalCurve*>(item);
        if(row < p->dataSize())
        {
            switch(col)
            {
                case 0:return p->sample(row).value;
                case 1:return p->sample(row).interval.minValue();
                case 2:return p->sample(row).interval.maxValue();
            }
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        const QwtPlotHistogram* p = static_cast<const QwtPlotHistogram*>(item);
        if(row < p->dataSize())
        {
            switch(col)
            {
                case 0:return p->sample(row).interval.minValue();
                case 1:return p->sample(row).interval.maxValue();
                case 2:return p->sample(row).value;
            }
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        const QwtPlotTradingCurve* p = static_cast<const QwtPlotTradingCurve*>(item);
        if(row < p->dataSize())
        {
            switch(col)
            {
                case 0:return p->sample(row).time;
                case 1:return p->sample(row).open;
                case 2:return p->sample(row).high;
                case 3:return p->sample(row).low;
                case 4:return p->sample(row).close;
            }
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        const QwtPlotMultiBarChart* p = static_cast<const QwtPlotMultiBarChart*>(item);
        if(row < p->dataSize())
        {
            const QwtSetSample& s = p->sample(row);
            if(0 == col)
            {
                return s.value;
            }
            else
            {
                int setIndex = col - 1;
                if(setIndex >=0 && setIndex < s.set.size())
                {
                    return s.set.value(setIndex);
                }
            }
        }
        break;
    }
    default:
        break;
    }

#endif

    return nan();
}
///
/// \brief 获取绘图数据维度的描述
/// \param item
/// \param index
/// \return
///
QString QwtPlotItemDataModel::getItemDimDescribe(QwtPlotItem *item, int index) const
{
#if QwtPlotItemDataModel_Use_Dynamic_Cast
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
#else
    int rtti = item->rtti();
    switch(rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve:
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        switch(index)
        {
        case 0:return tr("x");
        case 1:return tr("y");
        default:return QString();
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        switch(index)
        {
        case 0:return tr("x");
        case 1:return tr("y");
        case 2:return tr("z");
        default:return QString();
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        switch(index)
        {
        case 0:return tr("value");
        case 1:return tr("min");
        case 2:return tr("max");
        default:return QString();
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        switch(index)
        {
        case 0:return tr("x-min");
        case 1:return tr("x-max");
        case 2:return tr("value");
        default:return QString();
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        switch(index)
        {
        case 0:return tr("time");
        case 1:return tr("open");
        case 2:return tr("high");
        case 3:return tr("low");
        case 4:return tr("close");
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        if(0 == index)
        {
            return tr("value");
        }
        else
        {
            return tr("set %1").arg(index);
        }
        break;
    }
    default:
        break;
    }
#endif
    return QString();
}
///
/// \brief 依据显示规则设置数据
/// \param row 实际数据的行号
/// \param col 实际数据的列号，如果是表格列要减去这个数据的第一个维度的列数
/// \param item 数据指针
/// \param var 设置的值
/// \return 成功设置返回true
///
bool QwtPlotItemDataModel::setItemData(int row, int col, QwtPlotItem *item, const QVariant &var)
{
    if(row < 0 || col < 0 || !var.isValid() || nullptr == item)
        return false;
    bool isOK = false;
    double d = var.toDouble(&isOK);
    if(!isOK)
        return false;

    int rtti = item->rtti();
    switch(rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        if(col > 1)
            return false;
        QwtPlotCurve* p = static_cast<QwtPlotCurve*>(item);
        QVector<QPointF> xys;
        SAChart::getXYDatas(xys,p);
        if(row < xys.size())
        {
            switch(col)
            {
            case 0:
            {
                xys[row].setX(d);
                p->setSamples(xys);
                return true;
            }
            case 1:
            {
                xys[row].setY(d);
                p->setSamples(xys);
                return true;
            }
            default:
                break;
            }
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        QwtPlotBarChart* p = static_cast<QwtPlotBarChart*>(item);
        if(col > 1)
            return false;
        QVector<QPointF> xys;
        SAChart::getXYDatas(xys,p);
        if(row < xys.size())
        {
            switch(col)
            {
            case 0:
            {
                xys[row].setX(d);
                break;
            }
            case 1:
            {
                xys[row].setY(d);
                break;
            }
            default:
                break;
            }
            p->setSamples(xys);
            return true;
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        QwtPlotSpectroCurve* p = static_cast<QwtPlotSpectroCurve*>(item);
        if(col > 2)
            return false;
        QVector<QwtPoint3D> xyzs;
        SAChart::getXYZDatas(xyzs,p);
        if(row < xyzs.size())
        {
            switch(col)
            {
            case 0:
            {
                xyzs[row].setX(d);
                break;
            }
            case 1:
            {
                xyzs[row].setY(d);
                break;
            }
            case 2:
            {
                xyzs[row].setZ(d);
                break;
            }
            default:
                break;
            }
            p->setSamples(xyzs);
            return true;
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        QwtPlotIntervalCurve* p = static_cast<QwtPlotIntervalCurve*>(item);
        if(col > 2)
            return false;
        QVector<QwtIntervalSample> samples;
        SAChart::getIntervalSampleDatas(samples,p);
        if(row < samples.size())
        {
            switch(col)
            {
            case 0:
            {
                samples[row].value = d;
                break;
            }
            case 1:
            {
                samples[row].interval.setMinValue(d);
                break;
            }
            case 2:
            {
                samples[row].interval.setMaxValue(d);
                break;
            }
            default:
                break;
            }
            p->setSamples(samples);
            return true;
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        QwtPlotHistogram* p = static_cast<QwtPlotHistogram*>(item);
        if(col > 2)
            return false;
        QVector<QwtIntervalSample> samples;
        SAChart::getIntervalSampleDatas(samples,p);
        if(row < samples.size())
        {
            switch(col)
            {
            case 0:
            {
                samples[row].interval.setMinValue(d);
                break;
            }
            case 1:
            {
                samples[row].interval.setMaxValue(d);
                break;
            }
            case 2:
            {
                samples[row].value = d;
                break;
            }
            default:
                break;
            }
            p->setSamples(samples);
            return true;
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        QwtPlotTradingCurve* p = static_cast<QwtPlotTradingCurve*>(item);
        if(col > 4)
            return false;
        QVector<QwtOHLCSample> samples;
        SAChart::getSeriesData<QwtOHLCSample>(samples,p);

        if(row < samples.size())
        {
            switch(col)
            {
            case 0:
            {
                samples[row].time = d;
                break;
            }
            case 1:
            {
                samples[row].open = d;
                break;
            }
            case 2:
            {
                samples[row].high = d;
                break;
            }
            case 3:
            {
                samples[row].low = d;
                break;
            }
            case 4:
            {
                samples[row].close = d;
                break;
            }
            default:
                break;
            }
            p->setSamples(samples);
            return true;
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        QwtPlotMultiBarChart* p = static_cast<QwtPlotMultiBarChart*>(item);
        QVector<QwtSetSample> samples;
        SAChart::getSeriesData<QwtSetSample>(samples,p);
        if(row < samples.size())
        {
            if(0 == col)
            {
                samples[row].value = d;
            }
            else
            {
                if(col-1 < samples[row].set.size())
                {
                    samples[row].set[col-1] = d;
                }
                else
                {
                    return false;
                }
            }
            p->setSamples(samples);
            return true;
        }
        return false;
    }
    default:
        break;
    }
    return false;
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




