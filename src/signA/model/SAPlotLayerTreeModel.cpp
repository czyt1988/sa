#include "SAPlotLayerTreeModel.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAIconHelper.h"
#include "QPixmap"

SAPlotLayerTreeModel::SAPlotLayerTreeModel(SAFigureWindow *fig, QObject *parent) : QAbstractItemModel(parent)
    , m_fig(fig)
{
    setFigure(fig);
}


QModelIndex SAPlotLayerTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if ((row < 0) || (column < 0)) {
        return (QModelIndex());
    }
    //说明是顶层 顶层对应chart
    if (!parent.isValid()) {
        if (row >= m_2dcharts.size()) {
            return (QModelIndex());
        }
        return (createIndex(row, column, m_2dcharts[row]));//顶层节点
    }
    //chart 下面的item层和元素层
    QModelIndex grapar = parent.parent();

    if (!grapar.isValid()) {
        if (parent.row() >= m_2dcharts.size()) {
            return (QModelIndex());
        }
        SAChart2D *chart = static_cast<SAChart2D *>(parent.internalPointer());
        const QwtPlotItemList& it = chart->itemList();
        if (row < it.size()) {
            return (createIndex(row, column, it[row]));//坐标轴
        }
    }
    return (QModelIndex());
}


QModelIndex SAPlotLayerTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return (QModelIndex());
    }
    void *p = index.internalPointer();

    if (isChart2DPtr(p)) {
        //说明是顶层SAChart2D*，返回无效的index
        return (QModelIndex());
    }
    //p不是chart，那么就是item
    //chart下面就是各个item属性
    QwtPlotItem *i = static_cast<QwtPlotItem *>(p);
    SAChart2D *plot = qobject_cast<SAChart2D *>(i->plot());

    if (plot) {
        int r = m_2dcharts.indexOf(plot);
        if (r >= 0) {
            return (createIndex(r, 0, plot));
        }
    }
    return (QModelIndex());
}


int SAPlotLayerTreeModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) {//说明是顶层
        return (m_2dcharts.size());
    }
    //二层或者三层
    QModelIndex grapar = parent.parent();

    if (!grapar.isValid()) {
        //说明是二层
        if (grapar.row() >= m_2dcharts.size()) {
            return (0);
        }
        SAChart2D *chart = static_cast<SAChart2D *>(parent.internalPointer());
        return (filterCanDisplayItemSize(chart->itemList()));
    }
    return (0);
}


int SAPlotLayerTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    //0-名称 1-颜色 2-可见性
    return (3);
}


QVariant SAPlotLayerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return (QVariant());
    }
    if (Qt::Horizontal == orientation) {//说明是水平表头
        switch (section)
        {
        case 0:
            return (tr("name"));

        case 1:
            return (tr("color"));//值

        case 2:
            return (tr("visible"));//值
        }
    }
    return (QVariant());
}


Qt::ItemFlags SAPlotLayerTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return (Qt::NoItemFlags);
    }
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}


QVariant SAPlotLayerTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return (QVariant());
    }
    switch (role)
    {
    case Qt::DisplayRole:
        return (dataDisplayRole(index));

    case Qt::BackgroundRole:
        return (dataBackgroundRole(index));

    case Qt::DecorationRole:
        return (dataDecorationRole(index));

    default:
        break;
    }
    return (QVariant());
}


void SAPlotLayerTreeModel::update()
{
    beginResetModel();
    if (m_fig) {
        m_2dcharts = m_fig->get2DPlots();
    }
    endResetModel();
    emit modelReseted();
}


/**
 * @brief 设置figure
 *
 * 此函数会绑定绘图窗口的添加删除信号
 * @param fig
 *
 */
void SAPlotLayerTreeModel::setFigure(SAFigureWindow *fig)
{
    if (m_fig == fig) {
        return;
    }
    beginResetModel();
    if (m_fig) {
        //解除绑定
        disconnect(m_fig, &SAFigureWindow::chartAdded, this, &SAPlotLayerTreeModel::onChartAdded);
        disconnect(m_fig, &SAFigureWindow::chartWillRemove, this, &SAPlotLayerTreeModel::onChartWillRemove);
        for (SAChart2D *c : m_2dcharts)
        {
            disconnect(c, &SAChart2D::itemAttached, this, &SAPlotLayerTreeModel::onChartItemAttached);
        }
    }
    m_fig = fig;
    if (m_fig) {
        //建立新绑定
        connect(m_fig, &SAFigureWindow::chartAdded, this, &SAPlotLayerTreeModel::onChartAdded);
        connect(m_fig, &SAFigureWindow::chartWillRemove, this, &SAPlotLayerTreeModel::onChartWillRemove);
        m_2dcharts = m_fig->get2DPlots();
        for (SAChart2D *c : m_2dcharts)
        {
            connect(c, &SAChart2D::itemAttached, this, &SAPlotLayerTreeModel::onChartItemAttached);
        }
    }else {
        m_2dcharts.clear();
    }
    endResetModel();
}


/**
 * @brief index转换为item
 * @param index
 * @return
 */
QwtPlotItem *SAPlotLayerTreeModel::indexToItem(const QModelIndex& i)
{
    if (!i.isValid()) {
        return (nullptr);
    }
    void *p = i.internalPointer();
    QModelIndex par = i.parent();

    if (!par.isValid()) {
        //par无效，说明i是顶层
        return (nullptr);
    }
    //par有效，说明i是item
    return (static_cast<QwtPlotItem *>(p));
}


void SAPlotLayerTreeModel::onChartAdded(QwtPlot *plot)
{
    Q_UNUSED(plot);
    beginResetModel();
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
    emit modelReseted();
}


void SAPlotLayerTreeModel::onChartWillRemove(QwtPlot *plot)
{
    Q_UNUSED(plot);
    beginResetModel();
    //触发此信号，plot还未销毁
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
    emit modelReseted();
}


void SAPlotLayerTreeModel::onChartItemAttached(QwtPlotItem *plotItem, bool on)
{
    Q_UNUSED(plotItem);
    Q_UNUSED(on);
    beginResetModel();
    endResetModel();
    emit modelReseted();
}


/**
 * @brief 判断是否记录的chart2d指针
 * @param p
 * @return
 */
bool SAPlotLayerTreeModel::isChart2DPtr(void *p) const
{
    for (SAChart2D *v : m_2dcharts)
    {
        if (v == p) {
            return (true);
        }
    }
    return (false);
}


QVariant SAPlotLayerTreeModel::dataDisplayRole(const QModelIndex& index) const
{
    if (index.column() != 0) {
        return (QVariant());
    }
    void *p = index.internalPointer();

    if (isChart2DPtr(p)) {
        SAChart2D *chart = static_cast<SAChart2D *>(p);
        QString title = chart->title().text();
        if (title.isEmpty()) {
            int r = m_2dcharts.indexOf(chart);
            return (tr("chart %1").arg(r+1));
        }
        return (title);
    }else {
        QwtPlotItem *item = static_cast<QwtPlotItem *>(p);
        QString title = item->title().text();
        if (title.isEmpty()) {
            return (itemToName(item));
        }
        return (title);
    }
}


QVariant SAPlotLayerTreeModel::dataBackgroundRole(const QModelIndex& index) const
{
    void *p = index.internalPointer();

    if (isChart2DPtr(p)) {
        return (QVariant());
    }else {
        QwtPlotItem *item = static_cast<QwtPlotItem *>(p);
        QColor color = SAChart::getItemColor(item, QColor());
        if (color.isValid()) {
            color.setAlpha(30);
            return (QBrush(color));
        }
    }
    return (QVariant());
}


QVariant SAPlotLayerTreeModel::dataDecorationRole(const QModelIndex& index) const
{
    void *p = index.internalPointer();

    if (index.column() == 0) {
        //图标
        if (isChart2DPtr(p)) {
            return (SAIconHelper::getFigureWindowIcon());
        }else{
            return (SAIconHelper::getIconByPlotItem(static_cast<QwtPlotItem *>(p)));
        }
    }else if (index.column() == 1) {
        //绘制颜色图标
        if (isChart2DPtr(p)) {
            return (QVariant());
        }else {
            QwtPlotItem *item = static_cast<QwtPlotItem *>(p);
            QColor color = SAChart::getItemColor(item, QColor());
            if (color.isValid()) {
                QPixmap pixmap(25, 25);
                pixmap.fill(color);
                return (QIcon(pixmap));
            }
        }
    }else if (index.column() == 2) {
        //图层可见性
        if (isChart2DPtr(p)) {
            SAChart2D *chart = static_cast<SAChart2D *>(p);
            return (SAIconHelper::getLayoutVisibleIcon(chart->isVisible()));
        }else {
            QwtPlotItem *item = static_cast<QwtPlotItem *>(p);
            return (SAIconHelper::getLayoutVisibleIcon(item->isVisible()));
        }
    }
    return (QVariant());
}


/**
 * @brief 筛选可显示的item数
 * @param its
 * @return
 */
int SAPlotLayerTreeModel::filterCanDisplayItemSize(const QwtPlotItemList& its)
{
    return (its.size());
}


QString SAPlotLayerTreeModel::itemToName(const QwtPlotItem *item)
{
    switch (item->rtti())
    {
    case QwtPlotItem::Rtti_PlotItem://Unspecific value, that can be used, when it doesn't matter
        return (tr("Unspecific"));

    case QwtPlotItem::Rtti_PlotGrid://For QwtPlotGrid
        return (tr("Grid"));

    case QwtPlotItem::Rtti_PlotScale://For QwtPlotScaleItem
        return (tr("Scale Plot"));

    case QwtPlotItem::Rtti_PlotLegend://For QwtPlotLegendItem
        return (tr("Legend"));

    case QwtPlotItem::Rtti_PlotMarker://For QwtPlotMarker
        return (tr("Marker"));

    case QwtPlotItem::Rtti_PlotCurve://For QwtPlotCurve
        return (tr("Curve Plot"));

    case QwtPlotItem::Rtti_PlotSpectroCurve://For QwtPlotSpectroCurve
        return (tr("Spectro Curve Plot"));

    case QwtPlotItem::Rtti_PlotIntervalCurve://For QwtPlotIntervalCurve
        return (tr("Interval Curve Plot"));

    case QwtPlotItem::Rtti_PlotHistogram://For QwtPlotHistogram
        return (tr("Histogram Plot"));

    case QwtPlotItem::Rtti_PlotSpectrogram://For QwtPlotSpectrogram
        return (tr("Spectrogram Plot"));

    case QwtPlotItem::Rtti_PlotSVG://For QwtPlotSvgItem
        return (tr("SVG Plot"));

    case QwtPlotItem::Rtti_PlotTradingCurve://For QwtPlotTradingCurve
        return (tr("Trading Curve Plot"));

    case QwtPlotItem::Rtti_PlotBarChart://For QwtPlotBarChart
        return (tr("Bar Chart Plot"));

    case QwtPlotItem::Rtti_PlotMultiBarChart://For QwtPlotMultiBarChart
        return (tr("MultiBar Chart Plot"));

    case QwtPlotItem::Rtti_PlotShape://For QwtPlotShapeItem
        return (tr("Shape"));

    case QwtPlotItem::Rtti_PlotTextLabel://For QwtPlotTextLabel
        return (tr("Text Label"));

    case QwtPlotItem::Rtti_PlotZone://For QwtPlotZoneItem
        return (tr("Zone"));

    case QwtPlotItem::Rtti_PlotUserItem:
        return (tr("User Define"));

    default:
        break;
    }
    return (tr("unknow item"));
}
