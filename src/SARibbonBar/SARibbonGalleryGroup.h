#ifndef SARIBBONGALLERYGROUP_H
#define SARIBBONGALLERYGROUP_H
#include <QList>
#include "SARibbonGalleryItem.h"
#include <QListView>

class SARibbonGalleryGroupModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SARibbonGalleryGroupModel(QObject *parent = Q_NULLPTR);
    ~SARibbonGalleryGroupModel();
    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    void clear();
    SARibbonGalleryItem * at(int row) const;
    void insert(int row, SARibbonGalleryItem *item);
    SARibbonGalleryItem * take(int row);
    void append(SARibbonGalleryItem *item);
private:
    QList<SARibbonGalleryItem*> m_items;
};
///
/// \brief Gallery的组
///
/// 组负责管理Gallery Item，
///
#include "SARibbonGalleryItem.h"
class SARibbonGalleryGroup : public QListView
{
    Q_OBJECT
public:
    SARibbonGalleryGroup(QWidget* w = 0);

    virtual ~SARibbonGalleryGroup();

    void addItem(const QIcon& icon);
    void addItem(SARibbonGalleryItem *item);
    SARibbonGalleryGroupModel* groupModel();
};


#endif // SARIBBONGALLERYGROUP_H
