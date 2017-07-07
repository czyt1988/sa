#ifndef DATAFEATURETREEMODEL_H
#define DATAFEATURETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QSet>
#include <QMap>
#include <memory>
#include "SADataFeatureItem.h"
class SAChart2D;
class QwtPlotItem;
class DataFeatureTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DataFeatureTreeModel(QObject *parent = 0);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    void clear();
public:
    //移除根节点
    bool takeRootItem(SADataFeatureItem* item);
    //把item挂载到对应的条目上
    void setPlotItem(SAChart2D* chartPtr, QwtPlotItem *itemPtr, SADataFeatureItem* items);
    //获取顶层节点
    QList<SADataFeatureItem* > getRootItems() const;
    //通过节点获取图形指针
    static SAChart2D* getChartPtrFromItem(const SADataFeatureItem *item);
    //通过节点获取对应的itemlist
    static QList<QwtPlotItem*> getItemListFromItem(const SADataFeatureItem *item);
    static QSet<QwtPlotItem*> getItemSetFromItem(const SADataFeatureItem *item);
    //通过figure指针查找到对应的item条目
    SADataFeatureItem* findChartItem(SAChart2D* p);
protected:
    //创建一个figureitem，其他item挂载在这个figureitem下面
    SADataFeatureItem* createChartItems(SAChart2D *chart);
    //通过figure指针查找到对应的item条目，figure指针对应以第一层item
    SADataFeatureItem* findChartItem(SAChart2D* p,bool autoCreate);
signals:

public slots:
private:
    SADataFeatureItem *toItemPtr(const QModelIndex &index) const;
private:
    QList<SADataFeatureItem* > m_items;
    QMap<SAChart2D *,SADataFeatureItem*> m_fig2item;
};

#endif // DATAFEATURETREEMODEL_H
