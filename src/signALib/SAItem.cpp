#include "SAItem.h"
#include <QList>
#include <QVector>
#include <QDebug>
#include "SATree.h"
#include "SAOrderMap.h"
//把from的子对象都复制一份到to
void copy_childs(const SAItem *from, SAItem *to);

//打印一个item内容
QDebug& print_one_item(QDebug& dbg, const SAItem& item, const QString& prefix, bool isNewline = true);
QDebug& print_item_and_child_items(QDebug& dbg, const SAItem& item, int indent);

void copy_childs(const SAItem *from, SAItem *to)
{
    QList<SAItem *> items = from->getChildItems();
    const auto size = items.size();

    for (auto i = 0; i < size; ++i)
    {
        SAItem *tmp = new SAItem();
        //如果还有子item，会触发递归
        *tmp = *(items[i]);
        to->appendChild(tmp);
    }
}


class SAItemPrivate
{
    SA_IMPL_PUBLIC(SAItem)
public:
    SAItem *m_parent;
    QList<SAItem *> m_childs;
    int m_fieldRow;                                                                 ///<用于记录当前所处的层级，如果parent不为nullptr，这个将返回parent下次item对应的层级
    int m_id;
    SATree *m_tree;                                                                 ///< 绑定的树
    SAOrderMap<int, QVariant, QVector<int>, QVector<QVariant> > m_propertys;        ///< 定义属性
//    QList<int> m_dataIDs;           ///< data的id，使用两个数组来维护，避免短小操作也进行二叉树查找
//    QVector<QVariant> m_datas;      ///< data
    SAItemPrivate(SAItem *par)
        : q_ptr(par)
        , m_parent(nullptr)
        , m_fieldRow(-1)
        , m_id(reinterpret_cast<int>(par))
        , m_tree(nullptr)
    {
        m_propertys[SAItem::RoleName] = QString();
        m_propertys[SAItem::RoleIcon] = QIcon();
        m_propertys[SAItem::RoleValue] = QVariant();
    }


    ~SAItemPrivate()
    {
        clearChild();
    }


    void clearChild()
    {
        QList<SAItem *> childs = m_childs;

        for (int i = 0; i < childs.size(); ++i)
        {
            delete childs[i];
        }
        m_childs.clear();
    }


    void updateFieldCount(int startRow = 0)
    {
        const int cc = m_childs.size();

        for (int i = startRow; i < cc; ++i)
        {
            m_childs[i]->d_ptr->m_fieldRow = i;
        }
    }
};



SAItem::SAItem(SAItem *parentItem) : d_ptr(new SAItemPrivate(this))
{
    if (parentItem) {
        parentItem->appendChild(this);
    }
}


SAItem::SAItem(const QString& text) : d_ptr(new SAItemPrivate(this))
{
    setName(text);
}


SAItem::SAItem(const QIcon& icon, const QString& text) : d_ptr(new SAItemPrivate(this))
{
    setName(text);
    setIcon(icon);
}


/**
 * @brief 拷贝构造函数
 * @param c
 */
SAItem::SAItem(const SAItem& c) : d_ptr(new SAItemPrivate(this))
{
    *this = c;
}


SAItem::~SAItem()
{
    clearChild();
    SAItem *par = parent();

    if (par) {
        int indexOfPar = par->childIndex(this);
        if (indexOfPar >= 0) {
            par->d_ptr->m_childs.removeAt(indexOfPar);
            par->d_ptr->updateFieldCount(indexOfPar);
        }
    }
    if (d_ptr->m_tree) {
        //d_ptr->m_tree->
    }
}


/**
 * @brief 等号操作符
 * @param item 另外等待拷贝的item
 * @return 返回自身引用
 * @note m_parent,m_fieldRow,id 不会发生拷贝
 */
SAItem& SAItem::operator =(const SAItem& item)
{
    clearChild();
    d_ptr->m_propertys = item.d_ptr->m_propertys;
    //复制子对象
    copy_childs(&item, this);
    return (*this);
}


///
/// \brief 设置条目名称
/// \param name 名称
///
void SAItem::setName(const QString& name)
{
    d_ptr->m_propertys.orderValue(static_cast<int>(RoleName)) = name;
}


///
/// \brief 条目名称
/// \return
///
QString SAItem::getName() const
{
    return (d_ptr->m_propertys.orderValue(static_cast<int>(RoleName)).toString());
}


///
/// \brief 设置条目图标
/// \param icon 图标
///
void SAItem::setIcon(const QIcon& icon)
{
    d_ptr->m_propertys.orderValue(static_cast<int>(RoleIcon)) = icon;
}


///
/// \brief 获取条目图标
/// \return
///
QIcon SAItem::getIcon() const
{
    return (d_ptr->m_propertys.orderValue(static_cast<int>(RoleIcon)).value<QIcon>());
}


///
/// \brief 条目id
///
/// \return
///
int SAItem::getID() const
{
    return (d_ptr->m_id);
}


///
/// \brief 设置扩展数据
/// \param roleID 标示id
/// \param var 数据内容
///
void SAItem::setProperty(int roleID, const QVariant& var)
{
    d_ptr->m_propertys[roleID] = var;
}


///
/// \brief 判断是否存在id对应的扩展数据
/// \param id 标示id
/// \return
///
bool SAItem::isHaveProperty(int roleID) const
{
    return (d_ptr->m_propertys.contains(roleID));
}


///
/// \brief 扩展数据的个数
/// \return
///
int SAItem::getPropertyCount() const
{
    return (d_ptr->m_propertys.size());
}


///
/// \brief 根据id获取扩展数据
/// \param id
/// \return
///
const QVariant& SAItem::property(int id) const
{
    return (d_ptr->m_propertys[id]);
}


///
/// \brief 根据id获取扩展数据
/// \param id
/// \return 获取为引用，修改将直接影响条目保存的数据内容
///
QVariant& SAItem::property(int id)
{
    return (d_ptr->m_propertys[id]);
}


///
/// \brief 根据索引顺序获取扩展数据，此函数仅仅为了方便遍历所有扩展数据用
/// \param index 索引顺序
/// \param id 返回hash的key
/// \param var 返回hash的value
///
void SAItem::property(int index, int& id, QVariant& var) const
{
    auto ite = d_ptr->m_propertys.cbegin();

    ite = ite + index;
    id = ite.key();
    var = ite.value();
}


/**
 * @brief 获取属性值
 * @param id
 * @param defaultvar
 * @return
 */
QVariant SAItem::getProperty(int id, const QVariant& defaultvar) const
{
    return (d_ptr->m_propertys.value(id, defaultvar));
}


/**
 * @brief 获取所有属性
 * @return
 */
QMap<int, QVariant> SAItem::getPropertys() const
{
    return (d_ptr->m_propertys.toMap());
}


///
/// \brief 子条目的数目
/// \return
///
int SAItem::childItemCount() const
{
    return (d_ptr->m_childs.size());
}


///
/// \brief 索引子条目
/// \param row 0base的行数索引
/// \return
///
SAItem *SAItem::childItem(int row) const
{
    return (d_ptr->m_childs[row]);
}


/**
 * @brief 获取所有子节点
 * @return
 */
QList<SAItem *> SAItem::getChildItems() const
{
    return (d_ptr->m_childs);
}


///
/// \brief 追加子条目
/// \param item
/// item的所有权交由父级item管理
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
/// item的所有权交由父级item管理
///
void SAItem::insertChild(SAItem *item, int row)
{
    item->d_ptr->m_fieldRow = row;
    d_ptr->m_childs.insert(row, item);
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


/**
 * @brief 判断是否存在子节点
 * @param item 节点
 * @return 如果存在返回true
 */
bool SAItem::haveChild(SAItem *const item) const
{
    return (d_ptr->m_childs.contains(item));
}


///
/// \brief 提取条目，此时字条目的内容将不归此条目管理
/// \param row
/// \return
///
SAItem *SAItem::takeChild(int row)
{
    SAItem *item = d_ptr->m_childs.takeAt(row);

    item->d_ptr->m_parent = nullptr;
    //修改后面的item的m_fieldRow
    d_ptr->updateFieldCount(row+1);
    return (item);
}


/**
 * @brief 提取出子节点
 * @param childItem
 */
bool SAItem::takeChild(SAItem *const item)
{
    int index = childIndex(item);

    if (index < 0) {
        return (false);
    }
    takeChild(index);
    return (true);
}


/**
 * @brief 返回child的索引
 * @param item
 * @return 返回这个child对应的索引
 * @note 此函数操作的时间复杂度为O(n),若没有，返回-1
 */
int SAItem::childIndex(SAItem *const item) const
{
    return (d_ptr->m_childs.indexOf(item));
}


/**
 * @brief 删除子对象
 * @param item 子对象的指针，如果没有将忽略
 * @note 此操作会回收内存
 */
void SAItem::removeChild(SAItem *item)
{
    int index = childIndex(item);

    if (index >= 0) {
        //删除会自动和父节点的m_childs脱离关系
        delete item;
        //修改索引item的m_fieldRow
        d_ptr->updateFieldCount(index);
    }
}


///
/// \brief 获取条目的父级条目，如果没有，返回nullptr
/// \return  如果没有，返回nullptr
///
SAItem *SAItem::parent() const
{
    return (d_ptr->m_parent);
}


///
/// \brief 获取当前条目所在父级条目的行数，如果当前条目是子条目，这个函数返回这个字条目是对应父级条目的第几行
/// \note \sa takeChild \sa insertChild 都会影响此函数的结果
/// \return
///
int SAItem::fieldRow() const
{
    return (d_ptr->m_fieldRow);
}


/**
 * @brief 判断是否在树节点上
 * @return 如果此item是在satree上，此函数返回true，否则为false
 */
bool SAItem::isOnTree() const
{
    return ((d_ptr->m_tree) != nullptr);
}


/**
 * @brief 判断是否是顶层，parent为nullptr既是说明在顶层
 * @return
 */
bool SAItem::isTop() const
{
    return ((d_ptr->m_parent) == nullptr);
}


void SAItem::setID(int id)
{
    d_ptr->m_id = id;
}


/**
 * @brief 设置树
 * @param tree
 */
void SAItem::setTree(SATree *tree)
{
    if (d_ptr->m_tree == tree) {
        return;
    }else if (d_ptr->m_tree) {
        d_ptr->m_tree->takeItemPtr(this);
    }
    d_ptr->m_tree = tree;
}


void SAItem::_setTreePtr(SATree *tree)
{
    d_ptr->m_tree = tree;
}


/**
 * @brief 移除父级，私有调用，用于移除父级关联，但不会联动父级清除itemlist的关系，一般用于父级批量删除子节点避免过多调用使用
 */
void SAItem::_removeParent()
{
    d_ptr->m_parent = nullptr;
}


/**
 * @brief 输出到qdebug
 * @param dbg
 * @param item
 * @return
 */
QDebug& operator<<(QDebug& dbg, const SAItem& item)
{
    dbg = print_one_item(dbg, item, "");
    QList<SAItem *> cis = item.getChildItems();

    for (int i = 0; i < cis.size(); ++i)
    {
        dbg = print_item_and_child_items(dbg, *cis[i], 2);
    }
    return (dbg);
}


QDebug& print_one_item(QDebug& dbg, const SAItem& item, const QString& prefix, bool isNewline)
{
    int pc = item.getPropertyCount();

    if (pc > 0) {
        dbg.noquote() << prefix << item.getName() << "{";
        int id;
        QVariant val;
        item.property(0, id, val);
        dbg.noquote() << id << ":" << val;
        for (int i = 1; i < pc; ++i)
        {
            item.property(i, id, val);
            dbg.noquote() << "," << id << ":" << val;
        }
        dbg.noquote() << "}";
    }else {
        dbg.noquote() << prefix << item.getName();
    }
    if (isNewline) {
        dbg << "\n";
    }
    return (dbg);
}


QDebug& print_item_and_child_items(QDebug& dbg, const SAItem& item, int indent)
{
    QString str(indent, ' ');

    str += QStringLiteral("└");
    print_one_item(dbg, item, str);
    QList<SAItem *> cis = item.getChildItems();

    for (int i = 0; i < cis.size(); ++i)
    {
        print_item_and_child_items(dbg, *cis[i], indent+2);
    }
    return (dbg);
}
