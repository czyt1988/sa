#ifndef SADATAFEATURETREEMODEL_H
#define SADATAFEATURETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QSet>
#include <QMap>
#include <memory>
#include "SAFigureWindow.h"
#include "SAItem.h"
#include "SADataFeatureItem.h"
#include "qwt_plot.h"
class SAChart2D;


/**
 * @brief 保存绘图参数的统计以及一些计算结果的model
 * 显示层级为 SAChart2D
 *               |_ PlotItem
 *                       |_ feature(SAItem)
 * 数据区域保留了QwtPlotItem 和 feature的映射
 */
class SADataFeatureTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    typedef std::shared_ptr<SAItem>		ItemSmtPtr;
    typedef ItemSmtPtr::element_type *	ItemPtr;
    explicit SADataFeatureTreeModel(SAFigureWindow *fig, QObject *parent = 0);
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QModelIndex parent(const QModelIndex& index) const;
    virtual int rowCount(const QModelIndex& parent) const;
    virtual int columnCount(const QModelIndex& parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    void clear();
    void reflash();

public:
    //设置fig
    void setFigure(SAFigureWindow *fig);

    //设置item属性的值
    ItemPtr setItemValue(QwtPlotItem *plotitem, const QString& name, const QVariant& value);

    //筛选可显示的items
    static QwtPlotItemList filterCanDisplayItems(const QwtPlotItemList& its);

    //转换为SAItem*
    ItemPtr toItemPtr(const QModelIndex& i) const;

    //通过model index 找到对应的chart 2d，无法找到返回nullptr
    SAChart2D *findChart2D(const QModelIndex& i) const;

private slots:
    //添加了一个绘图发送的信号
    void onChartAdded(QwtPlot *plot);

    //删除了一个绘图发送的信号
    void onChartRemoved(QwtPlot *plot);

private:
    //绑定item
    bool bindItem(QwtPlotItem *plotitem, ItemSmtPtr item);
    QVariant dataDisplayRole(const QModelIndex& index) const;
    QVariant dataBackgroundRole(const QModelIndex& index) const;
    QVariant dataDecorationRole(const QModelIndex& index) const;

    //判断item是否可用
    static bool isPlotitemCanDisplay(QwtPlotItem *item);

    //根据指针，返回item的名字
    static QString plotitemToTitleName(QwtPlotItem *item);

    //找到顶层节点
    static QModelIndex findTop(const QModelIndex& i);

    //判断是否记录的chart2d指针
    bool isChart2DPtr(void *p) const;

    //判断是否QwtPlotItem指针
    bool isQwtPlotItemPtr(void *p) const;

    //刷新数据，会把绘图（SAChart2D*）的数据进行更新，但item数据不进行修订
    void resetData();

    //通过saitem查找对应的plotitem
    QwtPlotItem *findPlotItemFromItem(ItemSmtPtr::element_type *i) const;

private:
    SAFigureWindow *m_fig;
    QList<SAChart2D *> m_2dcharts;                                                  ///< 保存2d绘图的数量，避免每次都调用m_fig->get2DPlots()
    QMap<QwtPlotItem *, QList<ItemSmtPtr::element_type *> > m_plotitemFeatures;     ///< 记录每个plotitem下面的feature
    QMap<ItemSmtPtr::element_type *, QwtPlotItem *> m_featureToPlotitem;
    QMap<ItemSmtPtr::element_type *, ItemSmtPtr> m_ptr2smtptr;
};

#endif // DATAFEATURETREEMODEL_H
