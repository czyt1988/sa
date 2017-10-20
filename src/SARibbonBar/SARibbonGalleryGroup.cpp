#include "SARibbonGalleryGroup.h"


SARibbonGalleryGroupModel::SARibbonGalleryGroupModel(QObject *parent):QAbstractListModel(parent)
{

}

SARibbonGalleryGroupModel::~SARibbonGalleryGroupModel()
{
    clear();
}

int SARibbonGalleryGroupModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_items.size();
}

Qt::ItemFlags SARibbonGalleryGroupModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()
            ||
            index.row()>=m_items.size())
        return Qt::NoItemFlags;
    return m_items.at(index.row())->flags();
}

QVariant SARibbonGalleryGroupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.count())
            return QVariant();
    return m_items.at(index.row())->data(role);
}

QModelIndex SARibbonGalleryGroupModel::index(int row, int column, const QModelIndex &parent) const
{
    if (hasIndex(row, column, parent))
        return createIndex(row, column, m_items.at(row));
    return QModelIndex();
}

bool SARibbonGalleryGroupModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_items.count())
        return false;

    m_items.at(index.row())->setData(role, value);
    return true;
}

void SARibbonGalleryGroupModel::clear()
{
    beginResetModel();
    for (int i = 0; i < m_items.count(); ++i) {
        if (m_items.at(i))
        {
            delete m_items.at(i);
        }
    }
    m_items.clear();
    endResetModel();
}

SARibbonGalleryItem *SARibbonGalleryGroupModel::at(int row) const
{
    return m_items.value(row);
}

void SARibbonGalleryGroupModel::insert(int row, SARibbonGalleryItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

SARibbonGalleryItem *SARibbonGalleryGroupModel::take(int row)
{
    if (row < 0 || row >= m_items.count())
        return 0;

    beginRemoveRows(QModelIndex(), row, row);
    SARibbonGalleryItem *item = m_items.takeAt(row);
    endRemoveRows();
    return item;
}

void SARibbonGalleryGroupModel::append(SARibbonGalleryItem *item)
{
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count()+1);
    m_items.append(item);
    endInsertRows();
}



SARibbonGalleryGroup::SARibbonGalleryGroup(QWidget *w):QListView(w)
{
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Adjust);
    setSelectionRectVisible(true);
    setUniformItemSizes(true);

    setIconSize(QSize(72,60));
    setGridSize(QSize(72,60));
}

SARibbonGalleryGroup::~SARibbonGalleryGroup()
{

}

void SARibbonGalleryGroup::addItem(const QIcon& icon)
{
    if(nullptr == groupModel())
    {
        return;
    }
    addItem(new SARibbonGalleryItem(icon));
}

void SARibbonGalleryGroup::addItem(SARibbonGalleryItem *item)
{
    if(nullptr == groupModel())
    {
        return;
    }
    groupModel()->append(item);
}

SARibbonGalleryGroupModel *SARibbonGalleryGroup::groupModel()
{
    return qobject_cast<SARibbonGalleryGroupModel*>(model());
}




