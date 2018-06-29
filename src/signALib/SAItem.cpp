#include "SAItem.h"
#include <QHash>
class SAItemPrivate
{
    SA_IMPL_PUBLIC(SAItem)
public:
    SAItem* m_parent;
    int m_fieldRow;///<用于记录当前所处的层级，如果parent不为nullptr，这个将返回parent下次item对应的层级
    QList<SAItem*> m_childs;
    QString m_name;
    QIcon m_icon;
    int m_id;
    QHash<int,QVariant> m_datas;
    SAItemPrivate(SAItem* par)
        :q_ptr(par)
        ,m_parent(nullptr)
        ,m_fieldRow(-1)
        ,m_id(0)
    {

    }
    ~SAItemPrivate()
    {
        clearChild();
    }
    void clearChild()
    {
        for(int i=0;i<m_childs.size();++i)
        {
            delete m_childs[i];
        }
        m_childs.clear();
    }
    void updateFieldCount(int startRow)
    {
        const int cc = m_childs.size();
        for(int i=startRow;i<cc;++i)
        {
            m_childs[cc]->d_ptr->m_fieldRow = i;
        }
    }
};


SAItem::SAItem():d_ptr(new SAItemPrivate(this))
{
    setID(int(this));
}

SAItem::SAItem(const QString &text):d_ptr(new SAItemPrivate(this))
{
    d_ptr->m_name = text;
    setID(int(this));
}

SAItem::SAItem(const QIcon &icon, const QString &text):d_ptr(new SAItemPrivate(this))
{
    d_ptr->m_name = text;
    d_ptr->m_icon = icon;
    setID(int(this));
}

SAItem::~SAItem()
{

}

void SAItem::setName(const QString &name)
{
    d_ptr->m_name = name;
}

QString SAItem::getName() const
{
    return d_ptr->m_name;
}

void SAItem::setIcon(const QIcon &icon)
{
    d_ptr->m_icon = icon;
}

const QIcon &SAItem::getIcon() const
{
    return d_ptr->m_icon;
}

int SAItem::getID() const
{
    return d_ptr->m_id;
}

void SAItem::setData(int roleID, const QVariant &var)
{
    d_ptr->m_datas[roleID] = var;
}

bool SAItem::isHaveData(int id) const
{
    return d_ptr->m_datas.contains(id);
}

int SAItem::getDataCount() const
{
    return d_ptr->m_datas.size();
}

const QVariant &SAItem::getData(int id) const
{
    return d_ptr->m_datas[id];
}

QVariant &SAItem::getData(int id)
{
    return d_ptr->m_datas[id];
}

void SAItem::getData(int index, int &id, QVariant &var) const
{
    auto ite = d_ptr->m_datas.cbegin();
    ite = ite + index;
    id = ite.key();
    var = ite.value();
}

int SAItem::childCount() const
{
    return d_ptr->m_childs.size();
}

SAItem *SAItem::child(int row) const
{
    return d_ptr->m_childs[row];
}

void SAItem::appendChild(SAItem *item)
{
    item->d_ptr->m_fieldRow = d_ptr->m_childs.size();
    item->d_ptr->m_parent = this;
    d_ptr->m_childs.append(item);
}

void SAItem::insertChild(SAItem *item, int row)
{
    item->d_ptr->m_fieldRow = row;
    d_ptr->m_childs.insert(row,item);
    item->d_ptr->m_parent = this;
    //修改后面的item的m_fieldRow
    d_ptr->updateFieldCount(row+1);
}

void SAItem::clearChild()
{
    d_ptr->clearChild();
}

SAItem *SAItem::takeChild(int row)
{
    SAItem* item = d_ptr->m_childs.takeAt(row);
    item->d_ptr->m_parent = nullptr;
    //修改后面的item的m_fieldRow
    d_ptr->updateFieldCount(row+1);
    return item;
}

SAItem *SAItem::parent() const
{
    return d_ptr->m_parent;
}

int SAItem::fieldRow() const
{
    return d_ptr->m_fieldRow;
}

void SAItem::setID(int id)
{
    d_ptr->m_id = id;
}
