#ifndef QWTPLOTITEMTREEMODEL_H
#define QWTPLOTITEMTREEMODEL_H
#include "SAChartGlobals.h"

#include <QAbstractItemModel>
#include <QList>

class QwtPlot;
#include <qwt_plot_item.h>

class SA_CHART_EXPORT QwtPlotItemTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    QwtPlotItemTreeModel(QObject * parent = 0);
    ~QwtPlotItemTreeModel();
    void addPlot(QwtPlot* plot);
    void setPlots(const QList<QwtPlot*>& plot);
    void clear();
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    QwtPlot* getQwtPlotFromIndex(const QModelIndex& index) const;
    QwtPlotItem* getQwtPlotItemFromIndex(const QModelIndex& index) const;

protected:
    virtual QVariant getColorFromItem(const QwtPlotItem* item,int alpha=255) const;
    virtual QVariant displayDecorationRole(const QModelIndex &index) const;
    virtual QList<QwtPlotItem*> getItems(QwtPlot* plot) const;
private:
    QList<QwtPlot*> m_plot;
};

#endif // QWTPLOTITEMTREEMODEL_H
