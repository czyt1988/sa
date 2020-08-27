#ifndef SAPLOTLAYERTREEMODEL_H
#define SAPLOTLAYERTREEMODEL_H
#include <QAbstractItemModel>
#include "qwt_plot.h"
#include "SAFigureWindow.h"

/**
 * @brief 针对SAFigure的model
 */
class SAPlotLayerTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SAPlotLayerTreeModel(SAFigureWindow *fig, QObject *parent = 0);
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    void update();

public:
    //设置fig
    void setFigure(SAFigureWindow *fig);

    //index转换为item
    QwtPlotItem *indexToItem(const QModelIndex& i);

signals:
    //模型重置发出的信号
    void modelReseted();

private slots:
    //添加了一个绘图发送的信号
    void onChartAdded(QwtPlot *plot);

    //删除了一个绘图发送的信号
    void onChartWillRemove(QwtPlot *plot);

    //item的添加
    void onChartItemAttached(QwtPlotItem *plotItem, bool on);

private:
    //判断是否记录的chart2d指针
    bool isChart2DPtr(void *p) const;

    //data - DisplayRole
    QVariant dataDisplayRole(const QModelIndex& index) const;

    //data - BackgroundRole
    QVariant dataBackgroundRole(const QModelIndex& index) const;

    //data - DecorationRole
    QVariant dataDecorationRole(const QModelIndex& index) const;

    //筛选可显示的item数
    static int filterCanDisplayItemSize(const QwtPlotItemList& its);

    //item转换为名字
    static QString itemToName(const QwtPlotItem *item);

private:
    SAFigureWindow *m_fig;
    QList<SAChart2D *> m_2dcharts;
};

#endif // SAPLOTLAYERTREEMODEL_H
