#include "SATree.h"
#include "SAItem.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDataStream>
#include <memory>

QJsonObject write_item_to_json(SAItem* item);
bool read_item_from_json(const QJsonObject &json, SAItem* item);
class SATreePrivate
{
    SA_IMPL_PUBLIC(SATree)
public:
    SATreePrivate(SATree* c)
        :q_ptr(c)
    {
    }
    QList<SAItem*> mRootNodes;///< 记录所有根节点
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
    const auto size = d_ptr->mRootNodes.size();
    for(auto i = 0;i<size;++i)
    {
        delete d_ptr->mRootNodes[i];
    }
    d_ptr->mRootNodes.clear();
}
/**
 * @brief 获取子节点的个数
 * @return
 */
int SATree::getItemCount() const
{
    return d_ptr->mRootNodes.size();
}
/**
 * @brief 索引子条目
 * @param row 0base的行数索引
 * @return
 */
SAItem *SATree::getItem(int row) const
{
    return d_ptr->mRootNodes[row];
}
/**
 * @brief 获取所有子节点
 * @return
 */
QList<SAItem *> SATree::getItems() const
{
    return d_ptr->mRootNodes;
}
/**
 * @brief 追加子条目
 * @param item
 * item的所有权交由satree管理
 */
void SATree::appendItem(SAItem *item)
{
    d_ptr->mRootNodes.append(item);
    item->setTree(this);
}
/**
 * @brief 插入子条目
 * @param item
 * @param row
 */
void SATree::insertItem(SAItem *item, int row)
{
    d_ptr->mRootNodes.insert(row,item);
    item->setTree(this);
}
/**
 * @brief 判断是否存在子节点
 * @param item
 * @return
 */
bool SATree::haveItem(SAItem* item) const
{
    return d_ptr->mRootNodes.contains(item);
}
/**
 * @brief 把item解除satree的关系
 * @param item
 */
void SATree::takeItemPtr(SAItem *item)
{
    d_ptr->mRootNodes.removeOne(item);
    //item->setTree(nullptr); 不能这样调用会循环调用
    item->_setTreePtr(nullptr);
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
/**
 * @brief 返回item对应的树层级
 * @param item
 * @return
 */
int SATree::indexOfItem(const SAItem *item) const
{
    return d_ptr->mRootNodes.indexOf(const_cast<SAItem *>(item));
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
 * @brief 把satree转换为json string
 * @param tree tree指针
 * @return
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
            propObj.insert(QString::number(id),QJsonValue::fromVariant(var));
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


bool read_item_from_json(const QJsonObject &json, SAItem* item)
{
    auto i = json.find("name");
    if(i != json.end())
    {
        item->setName(i.value().toString());
    }
    i = json.find("icon");
    if(i != json.end())
    {
        QIcon icon;
        QByteArray byte = QByteArray::fromBase64(i.value().toString().toLocal8Bit());
        QDataStream st(&byte,QIODevice::ReadWrite);
        st >>icon;
        if(!icon.isNull())
            item->setIcon(icon);
    }
    i = json.find("porperty");
    if(i != json.end())
    {
        if(i.value().isObject())
        {
            QJsonObject propObj = i.value().toObject();
            for(auto oi = propObj.begin();oi != propObj.end();++oi)
            {
                bool isKeyOk = false;
                int propID = oi.key().toInt(&isKeyOk);
                if(!isKeyOk)
                    continue;
                QVariant var = oi.value().toVariant();
                item->setProperty(propID,var);
            }
        }
    }
    i = json.find("childItems");
    if(i != json.end())
    {
        //读取子节点
        if(i.value().isArray())
        {
            QJsonArray jArrVal = i.value().toArray();
            for(auto i=jArrVal.begin();i!=jArrVal.end();++i)
            {
                std::unique_ptr<SAItem> childitem = std::make_unique<SAItem>();
                if(read_item_from_json((*i).toObject(),childitem.get()))
                {
                    item->appendChild(childitem.release());
                }
            }
        }
    }
    return true;
}

/**
 * @brief 从标准json sting转换到tree
 * @param json jsonstring
 * @param tree 待修改的tree
 * @return 如果转换成功返回true
 */
bool fromJson(const QString &json, SATree *tree)
{
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8(), &error);
    if(!jsonDocument.isArray())
    {
        return false;
    }
    QJsonArray jsonArr = jsonDocument.array();
    const auto size = jsonArr.size();
    for(int i=0;i<size;++i)
    {
        std::unique_ptr<SAItem> item = std::make_unique<SAItem>();
        QJsonValue v = jsonArr[i];
        if(read_item_from_json(v.toObject(),item.get()))
        {
            tree->appendItem(item.release());
        }
    }
}
