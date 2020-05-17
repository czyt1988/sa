#ifndef SATREE_H
#define SATREE_H
#include "SALibGlobal.h"
#include <QVariant>
#include "SAItem.h"
class SATreePrivate;
class SATree;


/**
 * @brief 通用树形结构数据存储
 * 支持任意拷贝和赋值
 */
class SALIB_EXPORT SATree
{
    SA_IMPL(SATree)
    friend class SAItem;
public:
    SATree();
    SATree(const SATree & c);
    ~SATree();
    //重载等号操作符
    SATree& operator =(const SATree& tree);
    //清空所有节点和属性
    void clear();
    //父子条目操作相关
    int getItemCount() const;
    //索引子条目
    SAItem *getItem(int row) const;
    //获取当前下的所有子节点
    QList<SAItem*> getItems() const;
    //追加子条目
    void appendItem(SAItem* item);
    //插入子条目
    void insertItem(SAItem* item,int row);
    //判断是否存在子节点
    bool haveItem(SAItem *item) const;
    //把item解除satree的关系
    void takeItemPtr(SAItem* item);
    SAItem* takeItem(int row);
    //返回item对应的树层级
    int indexOfItem(const SAItem* item) const;
    //设置tree的属性，是的tree可以携带附加信息
    void setProperty(const QString& name,const QVariant& var);
    //获取属性
    QVariant getProperty(const QString& name,const QVariant& defaultVal = QVariant()) const;
    //移除属性
    void removeProperty(const QString& name);
    //获取所有属性名
    QList<QString> getPropertyNames() const;
    //获取所有属性
    QMap<QString,QVariant> getPropertys() const;
};
Q_DECLARE_METATYPE(SATree)
//debug输出
SALIB_EXPORT QDebug& operator<<(QDebug& dbg, const SATree &tree);
//把satree转换为json string
SALIB_EXPORT QString toJson(const SATree* tree);
//从标准json sting转换到tree
SALIB_EXPORT bool fromJson(const QString& json,SATree* tree);


#endif // SATREE_H
