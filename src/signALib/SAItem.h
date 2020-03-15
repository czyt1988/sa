#ifndef SAITEM_H
#define SAITEM_H
#include "SALibGlobal.h"
#include <QString>
#include <QIcon>
#include <QVariant>
class SAItemPrivate;
///
/// \ingroup SALib
/// \brief 基本树形结构的条目，是SAAbstractData的基类，提供了名称和图标以及父子关系管理功能
/// SAItem可用SATree进行管理，形成树形结构
///
class SALIB_EXPORT SAItem
{
    SA_IMPL(SAItem)
    friend class SATree;
public:
    SAItem(SAItem* parentItem = nullptr);
    SAItem(const QString & text);
    SAItem(const QIcon & icon, const QString & text);
    SAItem(const SAItem & c);
    virtual ~SAItem();
    //重载等号操作符
    SAItem& operator =(const SAItem& item);
    //名字
    void setName(const QString& name);
    QString getName() const;
    //图标
    void setIcon(const QIcon& icon);
    const QIcon& getIcon() const;
    //id
    int getID() const;
    //扩展数据操作相关
    void setProperty(int roleID,const QVariant& var);
    bool isHaveProperty(int id) const;
    int getPropertyCount() const;

    //扩展数据的获取操作
    const QVariant& property(int id) const;
    QVariant& property(int id);
    void property(int index,int& id,QVariant& var) const;
    QVariant getProperty(int id,const QVariant& defaultvar = QVariant());
    //父子条目操作相关
    int childItemCount() const;
    //索引子条目
    SAItem *childItem(int row) const;
    //获取当前下的所有子节点
    QList<SAItem*> getChildItems() const;
    //追加子条目
    void appendChild(SAItem* item);
    //插入子条目
    void insertChild(SAItem* item,int row);
    //清空所有
    void clearChild();
    //判断是否存在子节点
    bool haveChild(SAItem * const item) const;
    //提取子节点
    SAItem* takeChild(int row);
    bool takeChild(SAItem * const item);
    //返回child的索引 O(n)
    int childIndex(SAItem* item) const;
    //删除子对象
    void removeChild(SAItem* item);
    //获取父级指针
    SAItem* parent() const;
    //用于记录当前所处的层级，如果parent不为nullptr，这个将返回parent下次item对应的层级,如果没有parent，返回-1
    int fieldRow() const;
    //判断是否在树节点上，如果此item是在satree上，此函数返回true，否则为false
    bool isOnRoot() const;
protected:
    void setID(int id);
private:
    void setTree(SATree* tree);
    void _setTreePtr(SATree* tree);
    void _removeParent();
};

SALIB_EXPORT QDebug& operator<<(QDebug& dbg, const SAItem &item);

#endif // SAITEM_H
