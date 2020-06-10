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
    typedef std::shared_ptr<SADataFeatureItem> ItemPtr;
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
signals:

private slots:
    //添加了一个绘图发送的信号
    void onChartAdded(QwtPlot* plot);
    //删除了一个绘图发送的信号
    void onChartRemoved(QwtPlot* plot);
private:
    static bool isPlotitemCanDisplay(QwtPlotItem *item);
    //根据指针，返回item的名字
    static QString plotitemToTitleName(QwtPlotItem* item);
    //
    QObject *toPtr(const QModelIndex &index) const;
    //刷新数据，会把绘图（SAChart2D*）的数据进行更新，但item数据不进行修订
    void reflashData();
private:
    SAFigureWindow* m_fig;
    QList<SAChart2D*> m_2dcharts;///< 保存2d绘图的数量，避免每次都调用m_fig->get2DPlots()
    QSet<ItemPtr> m_features;
    QMap<QwtPlotItem*,QList<ItemPtr>> m_plotitemFeatures; ///< 记录每个plotitem下面的feature
    QMap<ItemPtr,QwtPlotItem*> m_featureToPlotitem;
};

#endif // DATAFEATURETREEMODEL_H
