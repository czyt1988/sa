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


SAItem::SAItem(SAItem *parentItem):d_ptr(new SAItemPrivate(this))
{
    setID(int(this));
    if(parentItem)
    {
        parentItem->appendChild(this);
    }
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
///
/// \brief 设置条目名称
/// \param name 名称
///
void SAItem::setName(const QString &name)
{
    d_ptr->m_name = name;
}
///
/// \brief 条目名称
/// \return
///
QString SAItem::getName() const
{
    return d_ptr->m_name;
}
///
/// \brief 设置条目图标
/// \param icon 图标
///
void SAItem::setIcon(const QIcon &icon)
{
    d_ptr->m_icon = icon;
}
///
/// \brief 获取条目图标
/// \return
///
const QIcon &SAItem::getIcon() const
{
    return d_ptr->m_icon;
}
///
/// \brief 条目id
///
/// \return
///
int SAItem::getID() const
{
    return d_ptr->m_id;
}
///
/// \brief 设置扩展数据
/// \param roleID 标示id
/// \param var 数据内容
///
void SAItem::setData(int roleID, const QVariant &var)
{
    d_ptr->m_datas[roleID] = var;
}
///
/// \brief 判断是否存在id对应的扩展数据
/// \param id 标示id
/// \return
///
bool SAItem::isHaveData(int id) const
{
    return d_ptr->m_datas.contains(id);
}
///
/// \brief 扩展数据的个数
/// \return
///
int SAItem::getDataCount() const
{
    return d_ptr->m_datas.size();
}
///
/// \brief 根据id获取扩展数据
/// \param id
/// \return
///
const QVariant &SAItem::getData(int id) const
{
    return d_ptr->m_datas[id];
}
///
/// \brief 根据id获取扩展数据
/// \param id
/// \return 获取为引用，修改将直接影响条目保存的数据内容
///
QVariant &SAItem::getData(int id)
{
    return d_ptr->m_datas[id];
}
///
/// \brief 根据索引顺序获取扩展数据，此函数仅仅为了方便遍历所有扩展数据用
/// \param index 索引
/// \param id
/// \param var
///
void SAItem::getData(int index, int &id, QVariant &var) const
{
    auto ite = d_ptr->m_datas.cbegin();
    ite = ite + index;
    id = ite.key();
    var = ite.value();
}
///
/// \brief 子条目的数目
/// \return
///
int SAItem::childCount() const
{
    return d_ptr->m_childs.size();
}
///
/// \brief 字条目指针
/// \param row
/// \return
///
SAItem *SAItem::child(int row) const
{
    return d_ptr->m_childs[row];
}
///
/// \brief 追加子条目
/// \param item
///
void SAItem::appendChild(SAItem *item)
{
    item->d_ptr->m_fieldRow = d_ptr->m_childs.size();
    item->d_ptr->m_parent = this;
    d_ptr->m_childs.append(item);
}
///
/// \brief 插入子条目
/// \param item
///
void SAItem::insertChild(SAItem *item, int row)
{
    item->d_ptr->m_fieldRow = row;
    d_ptr->m_childs.insert(row,item);
    item->d_ptr->m_parent = this;
    //修改后面的item的m_fieldRow
    d_ptr->updateFieldCount(row+1);
}
///
/// \brief 清除所有字条目，包括内存
///
void SAItem::clearChild()
{
    d_ptr->clearChild();
}
///
/// \brief 提取字条目，此时字条目的内容将不归此条目管理
/// \param row
/// \return
///
SAItem *SAItem::takeChild(int row)
{
    SAItem* item = d_ptr->m_childs.takeAt(row);
    item->d_ptr->m_parent = nullptr;
    //修改后面的item的m_fieldRow
    d_ptr->updateFieldCount(row+1);
    return item;
}
///
/// \brief 获取条目的父级条目，如果没有，返回nullptr
/// \return  如果没有，返回nullptr
///
SAItem *SAItem::parent() const
{
    return d_ptr->m_parent;
}
///
/// \brief 获取当前条目所在父级条目的行数，如果当前条目是子条目，这个函数返回这个字条目是对应父级条目的第几行
/// \note \sa takeChild \sa insertChild 都会影响此函数的结果
/// \return
///
int SAItem::fieldRow() const
{
    return d_ptr->m_fieldRow;
}

void SAItem::setID(int id)
{
    d_ptr->m_id = id;
}
