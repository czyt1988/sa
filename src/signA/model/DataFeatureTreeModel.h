#ifndef DATAFEATURETREEMODEL_H
#define DATAFEATURETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QMap>
#include <memory>
#include "SADataFeatureItem.h"
class SAFigureWindow;
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
    //把item挂载到对应的条目上
    void setPlotItem(SAFigureWindow* p,QwtPlotItem* itemPtr,SADataFeatureItem* items);
protected:
    //创建一个figureitem，其他item挂载在这个figureitem下面
    SADataFeatureItem* createFigureItem(SAFigureWindow *figure);
    //通过figure指针查找到对应的item条目，figure指针对应以第一层item
    SADataFeatureItem* findFigureItem(SAFigureWindow* p,bool autoCreate = false);
    //

signals:

public slots:
private:
    SADataFeatureItem *toItemPtr(const QModelIndex &index) const;
private:
    QList<SADataFeatureItem* > m_items;
    QMap<SAFigureWindow *,SADataFeatureItem*> m_fig2item;
};

#endif // DATAFEATURETREEMODEL_H
