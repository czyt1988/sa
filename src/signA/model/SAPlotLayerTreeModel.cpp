#include "SAPlotLayerTreeModel.h"
#include "SAChart2D.h"

SAPlotLayerTreeModel::SAPlotLayerTreeModel(SAFigureWindow *fig, QObject *parent) : QAbstractItemModel(parent)
    , m_fig(nullptr)
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
    return (0);
}


int SAPlotLayerTreeModel::columnCount(const QModelIndex& parent) const
{
    return (0);
}


QVariant SAPlotLayerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return (0);
}


Qt::ItemFlags SAPlotLayerTreeModel::flags(const QModelIndex& index) const
{
    return (0);
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
    if (m_fig) {
        //解除绑定
        disconnect(m_fig, &SAFigureWindow::chartAdded, this, &SAPlotLayerTreeModel::onChartAdded);
        disconnect(m_fig, &SAFigureWindow::chartWillRemove, this, &SAPlotLayerTreeModel::onChartWillRemove);
    }
    m_fig = fig;
    if (m_fig) {
        //建立新绑定
        connect(m_fig, &SAFigureWindow::chartAdded, this, &SAPlotLayerTreeModel::onChartAdded);
        connect(m_fig, &SAFigureWindow::chartWillRemove, this, &SAPlotLayerTreeModel::onChartWillRemove);
    }
}


void SAPlotLayerTreeModel::onChartAdded(QwtPlot *plot)
{
    Q_UNUSED(plot);
    beginResetModel();
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
}


void SAPlotLayerTreeModel::onChartWillRemove(QwtPlot *plot)
{
    Q_UNUSED(plot);
    beginResetModel();
    //触发此信号，plot还未销毁
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
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
