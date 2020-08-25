#ifndef SAPLOTLAYERTREEMODEL_H
#define SAPLOTLAYERTREEMODEL_H
#include <QAbstractItemModel>
#include "qwt_plot.h"
#include "SAFigureWindow.h"

class SAPlotLayerTreeModel : public QAbstractItemModel
{
public:
    explicit SAPlotLayerTreeModel(SAFigureWindow *fig, QObject *parent = 0);
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

public:
    //设置fig
    void setFigure(SAFigureWindow *fig);

private slots:
    //添加了一个绘图发送的信号
    void onChartAdded(QwtPlot *plot);

    //删除了一个绘图发送的信号
    void onChartWillRemove(QwtPlot *plot);

private:
    //判断是否记录的chart2d指针
    bool isChart2DPtr(void *p) const;

private:
    SAFigureWindow *m_fig;
    QList<SAChart2D *> m_2dcharts;
};

#endif // SAPLOTLAYERTREEMODEL_H
