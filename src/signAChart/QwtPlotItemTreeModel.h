#ifndef QWTPLOTITEMTREEMODEL_H
#define QWTPLOTITEMTREEMODEL_H
#include "SAChartGlobals.h"

#include <QAbstractItemModel>
#include <QList>

class QwtPlot;
#include <qwt_plot_item.h>

class SA_CHART_EXPORT QwtPlotItemTreeModel : public QAbstractItemModel
{
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
    //设定需要显示的item
    void setFilter( const QList<QwtPlotItem::RttiValues>& itemShow);
    //清除设定的过滤项目
    void clearFilter();
private:
    QVariant getColorFromItem(const QwtPlotItem* item,int alpha=255) const;
    QVariant displayDecorationRole(const QModelIndex &index) const;
    QList<QwtPlotItem*> getItems(QwtPlot* plot,const QList<QwtPlotItem::RttiValues>& filter=QList<QwtPlotItem::RttiValues>()) const;
private:
    QList<QwtPlot*> m_plot;
    QList<QwtPlotItem::RttiValues> m_showItemType;///< 显示的条目
};

#endif // QWTPLOTITEMTREEMODEL_H
