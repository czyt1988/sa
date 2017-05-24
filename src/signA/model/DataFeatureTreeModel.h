#ifndef DATAFEATURETREEMODEL_H
#define DATAFEATURETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <DataFeatureItem.h>
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
    void appendDataFeatureItem(AbstractDataFeatureItem* item);
    void clear();
    void setDataFeatureItems(const QList<AbstractDataFeatureItem*>& items);
signals:

public slots:
private:
    AbstractDataFeatureItem *toItemPtr(const QModelIndex &index) const;
private:
    QList<AbstractDataFeatureItem*> m_items;
};

#endif // DATAFEATURETREEMODEL_H
