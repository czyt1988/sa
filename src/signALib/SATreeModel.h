#ifndef SATREEMODEL_H
#define SATREEMODEL_H
#include "SALibGlobal.h"
#include <QAbstractItemModel>
class SATreeModelPrivate;
class SAItem;
class SALIB_EXPORT SATreeModel : public QAbstractItemModel
{
    Q_OBJECT
    SA_IMPL(SATreeModel)
public:
    explicit SATreeModel(QObject *par = nullptr,int colCount = 1);
    ~SATreeModel();
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    void setColumnCount(int col,const QVector<int>& roles);
    //设置除第一列的其他列索引，这个函数需要在列数不为1时设置，否则无法获知其余列的索引位置
    void setColumnsDisplayIDs(const QVector<int>& roles);
    void setColumnDisplayID(int index,int role);
    //插入水平header对应的item，效仿QStandardItemModel
    void setHorizontalHeaderItem(int column, SAItem *item);

private:
    SAItem *toItemPtr(const QModelIndex &index) const;

};

#endif // SATREEMODEL_H
