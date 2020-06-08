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
class SAChart2D;
class QwtPlotItem;
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
    explicit SADataFeatureTreeModel(SAFigureWindow* fig,QObject *parent = 0);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    void clear();
public:
    void setFigure(SAFigureWindow* fig);
public:
    //把item挂载到对应的条目上
    void setPlotItem(QwtPlotItem *plotitem, std::shared_ptr<SAItem> item);
    //通过节点获取图形指针
    SAChart2D* getChartPtrFromItem(SAItem *item);
signals:

private slots:
    //添加了一个绘图发送的信号
    void onChartAdded(QwtPlot* plot);
    //删除了一个绘图发送的信号
    void onChartRemoved(QwtPlot* plot);
private:
    //根据指针，返回item的名字
    static QString plotitemToTitleName(QwtPlotItem* item);
    //
    SADataFeatureItem *toItemPtr(const QModelIndex &index) const;
    //刷新数据，会把绘图（SAChart2D*）的数据进行更新，但item数据不进行修订
    void reflashData();
private:
    QList<SADataFeatureItem* > m_items;
    QMap<SAChart2D *,SADataFeatureItem*> m_fig2item;

    SAFigureWindow* m_fig;
    QList<SAChart2D*> m_2dcharts;///< 保存2d绘图的数量，避免每次都调用m_fig->get2DPlots()
    QSet<std::shared_ptr<SAItem>> m_features;
    QMap<QwtPlotItem*,QList<SAItem*>> m_plotitemFeatures; ///< 记录每个plotitem下面的feature
    QMap<SAItem*,QwtPlotItem*> m_featureToPlotitem;
};

#endif // DATAFEATURETREEMODEL_H
