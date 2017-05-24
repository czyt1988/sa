#ifndef QWTPLOTITEMDATAMODEL_H
#define QWTPLOTITEMDATAMODEL_H
#include "SAChartGlobals.h"

#include <QAbstractTableModel>
#include <QList>
#include <algorithm>
#include <vector>
#include <qwt_plot_item.h>

class SA_CHART_EXPORT QwtPlotItemDataModel : public QAbstractTableModel
{
public:
    QwtPlotItemDataModel(QObject *parent = 0);
    void setQwtPlotItems(const QList<QwtPlotItem*>& items);
    void addQwtPlotItems(QwtPlotItem* item);
    void clear();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role) const;

    void enableBackgroundColor(bool enable, int alpha = 30);
private:
    void updateMaxRow(const QList<QwtPlotItem*>& items);
    int toIndex(int col) const;
    QwtPlotItem* getItemFromCol(int col) const;
    QString getItemNameFromCol(int col) const;

public:
    static int getItemDataCount(QwtPlotItem* item);
    static QVariant getItemDataX(QwtPlotItem* item,int index);
    static QVariant getItemDataY(QwtPlotItem* item,int index);
    static QColor getItemColor(QwtPlotItem* item);
    static bool isIntervalType(QwtPlotItem* item);
private:
    QList<QwtPlotItem*> m_items;
    int m_max_row;
    int m_columnCount;
    bool m_enableBkColor;///< 是否允许背景色
    int m_bkAlpha;///< 背景透明度
};

#endif // QWTPLOTITEMDATAMODEL_H
