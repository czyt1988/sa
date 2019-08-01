#include "SATree.h"
#include "SAItem.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDataStream>
#include "SAVariantCaster.h"
QJsonObject write_item_to_json(SAItem* item);
QJsonValue variant_to_jsonvalue(const QVariant& var);

class SATreePrivate
{
    SA_IMPL_PUBLIC(SATree)
public:
    SATreePrivate(SATree* c)
        :q_ptr(c)
    {
    }
    QList<SAItem*> rootNodes;///< 记录所有根节点
};


SATree::SATree():d_ptr(new SATreePrivate(this))
{

}

SATree::SATree(const SATree &c):d_ptr(new SATreePrivate(this))
{
    *this = c;
}

SATree::~SATree()
{
    clear();
}
/**
 * @brief 重载等号操作符实现深拷贝
 * @param tree
 * @return
 */
SATree &SATree::operator =(const SATree &tree)
{
    clear();
    QList<SAItem*> items = tree.getItems();
    const int c = items.size();
    for(int i=0;i<c;++i)
    {
        SAItem* item = new SAItem();
        *item = *(items[c]);
        appendItem(item);
    }
    return *this;
}

void SATree::clear()
{
    const auto size = d_ptr->rootNodes.size();
    for(auto i = 0;i<size;++i)
    {
        delete d_ptr->rootNodes[i];
    }
    d_ptr->rootNodes.clear();
}
/**
 * @brief 获取子节点的个数
 * @return
 */
int SATree::getItemCount() const
{
    return d_ptr->rootNodes.size();
}
/**
 * @brief 索引子条目
 * @param row 0base的行数索引
 * @return
 */
SAItem *SATree::getItem(int row) const
{
    return d_ptr->rootNodes[row];
}
/**
 * @brief 获取所有子节点
 * @return
 */
QList<SAItem *> SATree::getItems() const
{
    return d_ptr->rootNodes;
}
/**
 * @brief 追加子条目
 * @param item
 * item的所有权交由satree管理
 */
void SATree::appendItem(SAItem *item)
{
    d_ptr->rootNodes.append(item);
    item->setTree(this);
}
/**
 * @brief 插入子条目
 * @param item
 * @param row
 */
void SATree::insertItem(SAItem *item, int row)
{
    d_ptr->rootNodes.insert(row,item);
    item->setTree(this);
}
/**
 * @brief 判断是否存在子节点
 * @param item
 * @return
 */
bool SATree::haveItem(SAItem* item) const
{
    return d_ptr->rootNodes.contains(item);
}
/**
 * @brief 把item解除satree的关系
 * @param item
 */
void SATree::takeItemPtr(SAItem *item)
{
    d_ptr->rootNodes.removeOne(item);
    //item->setTree(nullptr); 不能这样调用会循环调用
    item->__setTreePtr(nullptr);
}
/**
 * @brief 根据索引把item返回，同时解除satree的关系
 * @param row 索引
 */
SAItem *SATree::takeItem(int row)
{
    SAItem* item = getItem(row);
    takeItemPtr(item);
    return item;
}



QDebug &operator<<(QDebug &dbg, const SATree &tree)
{
    QList<SAItem*> items = tree.getItems();
    const auto c = items.size();
    for(auto i=0;i<c;++i)
    {
        dbg << *items[i];
    }
    return dbg;
}

/**
 * @brief 转为json
 * @param tree
 * @return 输出为json
 */
QString toJson(const SATree *tree)
{
    QList<SAItem*> items = tree->getItems();
    const auto c = items.size();
    QJsonArray mainJTree;
    for(auto i=0;i<c;++i)
    {
        SAItem* item = items[i];
        mainJTree.append(write_item_to_json(item));
    }
    QJsonDocument json(mainJTree);
    return json.toJson();
}

QJsonObject write_item_to_json(SAItem* item)
{
    QJsonObject itemObj;
    itemObj.insert("name",item->getName());
    QIcon icon = item->getIcon();
    if(!icon.isNull())
    {
        QByteArray byte;
        QDataStream st(&byte,QIODevice::ReadWrite);
        st << icon;
        itemObj.insert("icon",QString(byte.toBase64()));
    }
    const auto c = item->getPropertyCount();
    if(c > 0)
    {
        QJsonObject propObj;
        for(auto i=0;i<c;++i)
        {
            int id;
            QVariant var;
            item->getProperty(i,id,var);
            propObj.insert(QString::number(id),variant_to_jsonvalue(var));
        }
        itemObj.insert("porperty",propObj);
    }
    const auto cc = item->childItemCount();
    if(cc > 0)
    {
        QJsonArray jArrVal;
        for(auto i=0;i<cc;++i)
        {
            QJsonObject cj = write_item_to_json(item->childItem(i));
            jArrVal.append(cj);
        }
        itemObj.insert("childItems",jArrVal);
    }
    return itemObj;
}

QJsonValue variant_to_jsonvalue(const QVariant& var)
{
    return QJsonValue(SAVariantCaster::variantToString(var));
}
