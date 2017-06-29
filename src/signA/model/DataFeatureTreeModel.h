#ifndef DATAFEATURETREEMODEL_H
#define DATAFEATURETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include "SADataFeatureItem.h"
class DataFeatureTreeModel : public QAbstractItemModel
{
public:
    explicit DataFeatureTreeModel(QObject *parent = 0);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    void appendDataFeatureItem(SADataFeatureItem* item);
    void clear();
    void setDataFeatureItems(const QList<SADataFeatureItem*>& items);
signals:

public slots:
private:
    SADataFeatureItem *toItemPtr(const QModelIndex &index) const;
private:
    QList<SADataFeatureItem*> m_items;
};

#endif // DATAFEATURETREEMODEL_H
