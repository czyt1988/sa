#include "SADataFeatureTreeModel.h"
#include <QSet>
#include "SAFigureWindow.h"
#include "SAVariantCaster.h"
#include "SAChart.h"
#include "SAChart2D.h"
#include "SAItem.h"
//#define DEBUG_OUTPUT__
//#define DEBUG_OUTPUT__INDEX_
//#define DEBUG_OUTPUT__PARENT_
//#define DEBUG_OUTPUT__ROW_COUNT_
#ifdef DEBUG_OUTPUT__
#include <QDebug>
#endif
#ifndef ROLE_FIGURE_PTR
#define ROLE_FIGURE_PTR		(Qt::UserRole+2)
#endif
#ifndef ROLE_PLOT_ITEM_PTR
#define ROLE_PLOT_ITEM_PTR	(Qt::UserRole+3)
#endif
SADataFeatureTreeModel::SADataFeatureTreeModel(SAFigureWindow *fig, QObject *parent) : QAbstractItemModel(parent)
    , m_fig(nullptr)
{
    setFigure(fig);
}


QModelIndex SADataFeatureTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if ((row < 0) || (column < 0)) {
        return (QModelIndex());
    }

    if (!parent.isValid()) {//说明是顶层
        if ((row >= m_2dcharts.size())
            ||
            (column >= 2)) {
            return (QModelIndex());
        }
        return (createIndex(row, column, m_2dcharts[row]));//顶层节点
    }
    //二层或者三层
    QModelIndex grapar = parent.parent();

    if (!grapar.isValid()) {
        //说明是二层
        if ((column >= 2) || (grapar.row() >= m_2dcharts.size())) {
            return (QModelIndex());
        }
        SAChart2D *chart = static_cast<SAChart2D *>(parent.internalPointer());
        QwtPlotItemList items = filterCanDisplayItems(chart->itemList());
        if (row >= items.size()) {
            return (QModelIndex());
        }
        return (createIndex(row, column, items[row]));//2层节点
    }
    QModelIndex gragrapar = grapar.parent();

    if (!gragrapar.isValid()) {
        //说明是3层

        SAChart2D *chart = static_cast<SAChart2D *>(grapar.internalPointer());
        QwtPlotItemList items = filterCanDisplayItems(chart->itemList());
        if (parent.row() >= items.size()) {
            return (QModelIndex());
        }
        QwtPlotItem *item = static_cast<QwtPlotItem *>(items[parent.row()]);
        auto ite = m_plotitemFeatures.find(item);
        if (ite == m_plotitemFeatures.end()) {
            return (QModelIndex());
        }
        const QList<ItemSmtPtr::element_type *>& itsp = ite.value();
        if ((row > itsp.size()) || (column >= 2)) {
            return (QModelIndex());
        }
        return (createIndex(row, column, itsp[row]));//3层节点
    }else {
        //说明是第四层h
        ItemSmtPtr::element_type *sai = reinterpret_cast<ItemSmtPtr::element_type *>(parent.internalPointer());
        if ((row >= sai->childItemCount()) || (column >= 2)) {
            return (QModelIndex());
        }
        return (createIndex(row, column, sai->childItem(row)));//4层节点
    }
    return (QModelIndex());
}


QModelIndex SADataFeatureTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return (QModelIndex());
    }
    void *p = index.internalPointer();

    if (isChart2DPtr(p)) {
        //说明是顶层SAChart2D*，返回无效的index
        return (QModelIndex());
    }

    if (isQwtPlotItemPtr(p)) {
        //是QwtPlotItem
        SAChart2D *chart = static_cast<SAChart2D *>(reinterpret_cast<QwtPlotItem *>(p)->plot());
        return (createIndex(m_2dcharts.indexOf(chart), 0, chart));
    }
    ItemSmtPtr::element_type *sai = reinterpret_cast<ItemSmtPtr::element_type *>(p);
    QwtPlotItem *item = findPlotItemFromItem(sai);

    if (item) {
        SAChart2D *chart = static_cast<SAChart2D *>(item->plot());
        int i = filterCanDisplayItems(chart->itemList()).indexOf(item);
        if (i >= 0) {
            return (createIndex(i, 0, item));
        }
    }else if (!sai->isTop()) {
        //说明是次一级的saitem
        int parFieldRow = sai->parent()->fieldRow();
        if (parFieldRow < 0) {
            //说明这个parent是挂在QwtPlotItem下
            QwtPlotItem *plotitem = findPlotItemFromItem(sai->parent());
            if (plotitem) {
                QList<ItemSmtPtr::element_type *> v = m_plotitemFeatures.value(plotitem);
                int fieldrow = v.indexOf(sai->parent());
                if (fieldrow >= 0) {
                    return (createIndex(fieldrow, 0, sai->parent()));
                }
            }
            return (QModelIndex());
        }
        return (createIndex(sai->parent()->fieldRow(), 0, sai->parent()));
    }
    return (QModelIndex());
}


int SADataFeatureTreeModel::rowCount(const QModelIndex& parent) const
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
        return (filterCanDisplayItems(chart->itemList()).size());
    }
    QModelIndex gragrapar = grapar.parent();

    if (!gragrapar.isValid()) {
        //说明是3层
        SAChart2D *chart = static_cast<SAChart2D *>(grapar.internalPointer());
        QwtPlotItemList items = filterCanDisplayItems(chart->itemList());
        if (parent.row() >= items.size()) {
            return (0);
        }
        QwtPlotItem *item = static_cast<QwtPlotItem *>(items[parent.row()]);
        auto ite = m_plotitemFeatures.find(item);
        if (ite == m_plotitemFeatures.end()) {
            return (0);
        }
        return (ite.value().size());
    }else {
        ItemSmtPtr::element_type *sai = reinterpret_cast<ItemSmtPtr::element_type *>(parent.internalPointer());
        return (sai->childItemCount());
    }
    return (0);
}


int SADataFeatureTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return (2);
}


QVariant SADataFeatureTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return (QVariant());
    }
    if (Qt::Horizontal == orientation) {//说明是水平表头
        switch (section)
        {
        case 0:
            return (tr("property"));

        case 1:
            return (tr("value"));//值
        }
    }
    return (QVariant());
}


Qt::ItemFlags SADataFeatureTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return (Qt::NoItemFlags);
    }
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}


QVariant SADataFeatureTreeModel::data(const QModelIndex& index, int role) const
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

    default:
        break;
    }
    return (QVariant());
}


void SADataFeatureTreeModel::clear()
{
    beginResetModel();
    m_2dcharts.clear();
    m_plotitemFeatures.clear();
    m_featureToPlotitem.clear();
    m_ptr2smtptr.clear();
    endResetModel();
}


/**
 * @brief 刷新图表item
 */
void SADataFeatureTreeModel::reflash()
{
    beginResetModel();
    for (SAChart2D *c : m_2dcharts)
    {
        QwtPlotItemList its = c->itemList();
        for (QwtPlotItem *item : its)
        {
            if (isPlotitemCanDisplay(item)) {
                if (!m_plotitemFeatures.contains(item)) {
                    m_plotitemFeatures.insert(item, QList<ItemSmtPtr::element_type *>());
                }
            }
        }
    }
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
    if (m_fig) {
        //解除绑定
        disconnect(m_fig, &SAFigureWindow::chartAdded, this, &SADataFeatureTreeModel::onChartAdded);
        disconnect(m_fig, &SAFigureWindow::chartRemoved, this, &SADataFeatureTreeModel::onChartRemoved);
    }
    m_fig = fig;
    if (m_fig) {
        //建立新绑定
        connect(m_fig, &SAFigureWindow::chartAdded, this, &SADataFeatureTreeModel::onChartAdded);
        connect(m_fig, &SAFigureWindow::chartRemoved, this, &SADataFeatureTreeModel::onChartRemoved);
    }
    resetData();
}


/**
 * @brief 绑定item
 * @param plotitem
 * @param item
 * @return
 */
bool SADataFeatureTreeModel::bindItem(QwtPlotItem *plotitem, SADataFeatureTreeModel::ItemSmtPtr item)
{
    auto i = m_plotitemFeatures.find(plotitem);

    if (i == m_plotitemFeatures.end()) {
        return (false);
    }
    i.value().append(item.get());
    m_featureToPlotitem[item.get()] = plotitem;
    m_ptr2smtptr[item.get()] = item;
    return (true);
}


/**
 * @brief 设置属性值
 * @param plotitem 属性值对应的句柄
 * @param name 属性名
 * @param value 属性值
 * @return 成功绑定返回对应指针
 */
SADataFeatureTreeModel::ItemPtr SADataFeatureTreeModel::setItemValue(QwtPlotItem *plotitem, const QString& name, const QVariant& value)
{
    SADataFeatureTreeModel::ItemSmtPtr item = std::make_shared<SAItem>(name);

    if (value.isValid()) {
        if (value.canConvert<QVector<QPointF> >()) {
            QVector<QPointF> v = value.value<QVector<QPointF> >();
            for (int i = 0; i < v.size(); ++i)
            {
                std::unique_ptr<SAItem> ci = std::make_unique<SAItem>(tr("[%1]").arg(i+1));
                ci->setProperty(SAItem::RoleValue, v[i]);
                item->appendChild(ci.release());
            }
        }else {
            item->setProperty(SAItem::RoleValue, value);
        }
    }
    if (bindItem(plotitem, item)) {
        return (item.get());
    }
    return (nullptr);
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

    for (QwtPlotItem *item : itemlist)
    {
        auto i = m_plotitemFeatures.find(item);
        if (i != m_plotitemFeatures.end()) {
            QList<ItemSmtPtr::element_type *> items = i.value();
            for (ItemSmtPtr::element_type *it : items)
            {
                m_featureToPlotitem.remove(it);
                m_ptr2smtptr.remove(it);
            }
        }
        m_plotitemFeatures.remove(item);
    }
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
}


QVariant SADataFeatureTreeModel::dataDisplayRole(const QModelIndex& index) const
{
    void *p = index.internalPointer();

    if (isChart2DPtr(p)) {
        if (index.column() != 0) {
            return (QVariant());
        }
        SAChart2D *chart = static_cast<SAChart2D *>(p);
        QString title = chart->title().text();
        if (title.isEmpty()) {
            int r = m_2dcharts.indexOf(chart);
            return (tr("chart %1").arg(r+1));
        }
        return (title);
    }else if (isQwtPlotItemPtr(p)) {
        if (index.column() != 0) {
            return (QVariant());
        }
        QwtPlotItem *item = static_cast<QwtPlotItem *>(p);
        QString title = item->title().text();
        if (title.isEmpty()) {
            return (plotitemToTitleName(item));
        }
        return (title);
    }else {
        SAItem *i = static_cast<SAItem *>(p);
        if (0 == index.column()) {
            return (i->getName());
        }
        QVariant v = i->getProperty(SAItem::RoleValue);
        if (v.canConvert<QPoint>()) {
            QPoint p = v.value<QPoint>();
            return (QString("(%1,%2)").arg(p.x()).arg(p.y()));
        }else if (v.canConvert<QPointF>()) {
            QPointF p = v.value<QPointF>();
            return (QString("(%1,%2)").arg(p.x()).arg(p.y()));
        }
        return (SAVariantCaster::variantToString(v));
    }
    return (QVariant());
}


QVariant SADataFeatureTreeModel::dataBackgroundRole(const QModelIndex& index) const
{
    void *p = index.internalPointer();

    if (isQwtPlotItemPtr(p)) {
        QwtPlotItem *item = static_cast<QwtPlotItem *>(p);
        QColor color = SAChart::getItemColor(item, QColor());
        if (color.isValid()) {
            color.setAlpha(30);
            return (QBrush(color));
        }
    }
    return (QVariant());
}


/**
 * @brief 筛选可显示的items
 * @param its
 * @return
 */
QwtPlotItemList SADataFeatureTreeModel::filterCanDisplayItems(const QwtPlotItemList& its)
{
    QwtPlotItemList res;

    for (QwtPlotItem *i : its)
    {
        if (isPlotitemCanDisplay(i)) {
            res.append(i);
        }
    }
    return (res);
}


/**
 * @brief 把modelindex转换为SAItem，如果无法转换，返回nullptr
 *
 * @param i QModelIndex
 * @return 如果无法转换，返回nullptr
 */
SADataFeatureTreeModel::ItemPtr SADataFeatureTreeModel::toItemPtr(const QModelIndex& i) const
{
    if (!i.isValid()) {             //说明是顶层
        return (nullptr);       //顶层节点
    }
    QModelIndex parindex = i.parent();

    if (!parindex.isValid()) {
        //第二层
        return (nullptr);
    }
    //二层或者三层
    QModelIndex grapar = parindex.parent();

    //grapar (Chart)
    //  |_ par  (Plot Item)
    //       |_ item (Feature Item)
    if (!grapar.isValid()) {
        //说明祖父节点不存在，处于plot item
        return (nullptr);
    }
    return (static_cast<SADataFeatureTreeModel::ItemPtr>(i.internalPointer()));
}


/**
 * @brief 通过model index 找到对应的chart 2d，无法找到返回nullptr
 * @param i
 * @return  无法找到返回nullptr
 */
SAChart2D *SADataFeatureTreeModel::findChart2D(const QModelIndex& i) const
{
    QModelIndex top = findTop(i);
    SAChart2D *pc = static_cast<SAChart2D *>(top.internalPointer());

    if (m_2dcharts.contains(pc)) {
        return (pc);
    }
    return (nullptr);
}


/**
 * @brief 判断QwtPlotItem是否允许显示
 * @param item
 * @return
 */
bool SADataFeatureTreeModel::isPlotitemCanDisplay(QwtPlotItem *item)
{
    switch (item->rtti())
    {
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
        return (true);

    default:
        break;
    }
    return (false);
}


/**
 * @brief 根据指针，返回item的名字
 * @param item
 * @return
 */
QString SADataFeatureTreeModel::plotitemToTitleName(QwtPlotItem *item)
{
    switch (item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
        return (tr("curve"));

    case QwtPlotItem::Rtti_PlotMultiBarChart:
        return (tr("mult bar chart"));

    case QwtPlotItem::Rtti_PlotScale:
        return (tr("scale"));

    case QwtPlotItem::Rtti_PlotBarChart:
        return (tr("bar"));

    case QwtPlotItem::Rtti_PlotHistogram:
        return (tr("histogram"));

    case QwtPlotItem::Rtti_PlotSpectroCurve:
        return (tr("spectro curve"));

    case QwtPlotItem::Rtti_PlotIntervalCurve:
        return (tr("interval curve"));

    case QwtPlotItem::Rtti_PlotSpectrogram:
        return (tr("spectrogram"));

    case QwtPlotItem::Rtti_PlotTradingCurve:
        return (tr("trading curve"));

    default:
        break;
    }
    return (tr("item"));
}


/**
 * @brief 找到顶层节点
 * @param i
 * @return
 */
QModelIndex SADataFeatureTreeModel::findTop(const QModelIndex& i)
{
    QModelIndex tmp = i;

    while (tmp.parent().isValid())
    {
        tmp = tmp.parent();
    }
    return (tmp);
}


/**
 * @brief 判断是否记录的chart2d指针
 * @param p
 * @return
 */
bool SADataFeatureTreeModel::isChart2DPtr(void *p) const
{
    for (SAChart2D *v : m_2dcharts)
    {
        if (v == p) {
            return (true);
        }
    }
    return (false);
}


/**
 * @brief 判断是否QwtPlotItem指针
 * @param p
 * @return
 */
bool SADataFeatureTreeModel::isQwtPlotItemPtr(void *p) const
{
    for (SAChart2D *v : m_2dcharts)
    {
        if (v == p) {
            return (false);
        }
        const QwtPlotItemList& ils = v->itemList();
        for (QwtPlotItem *i : ils)
        {
            if (i == p) {
                return (true);
            }
        }
    }
    return (false);
}


/**
 * @brief 刷新数据，会把绘图（SAChart2D*）的数据进行更新，但item数据不进行修订
 */
void SADataFeatureTreeModel::resetData()
{
    //clear在null判断前面，以防止跳过清除
    clear();
    if (nullptr == m_fig) {
        return;
    }
    //匹配m_plotitemFeatures，把不存在的plot删除
    beginResetModel();
    m_2dcharts = m_fig->get2DPlots();
    endResetModel();
    reflash();
}


/**
 * @brief 通过saitem查找对应的plotitem
 * @param i
 * @return
 */
QwtPlotItem *SADataFeatureTreeModel::findPlotItemFromItem(ItemSmtPtr::element_type *i) const
{
    return (m_featureToPlotitem.value(i, nullptr));
}
