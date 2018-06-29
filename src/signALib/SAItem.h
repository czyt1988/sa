#ifndef SAITEM_H
#define SAITEM_H
#include "SALibGlobal.h"
#include <QStandardItem>

class SAItemPrivate;
///
/// \ingroup SALib
/// \brief 基本树形结构的条目，是SAAbstractData的基类，提供了名称和图标以及父子关系管理功能
///
class SALIB_EXPORT SAItem
{
    SA_IMPL(SAItem)
public:
    SAItem(SAItem* parentItem = nullptr);
    SAItem(const QString & text);
    SAItem(const QIcon & icon, const QString & text);
    virtual ~SAItem();
    //名字
    void setName(const QString& name);
    QString getName() const;
    //图标
    void setIcon(const QIcon& icon);
    const QIcon& getIcon() const;
    //id
    int getID() const;
    //扩展数据操作相关
    void setData(int roleID,const QVariant& var);
    bool isHaveData(int id) const;
    int getDataCount() const;

    //扩展数据的获取操作
    const QVariant& getData(int id) const;
    QVariant& getData(int id);
    void getData(int index,int& id,QVariant& var) const;
    //父子条目操作相关
    int childCount() const;
    SAItem *child(int row) const;
    void appendChild(SAItem* item);
    void insertChild(SAItem* item,int row);
    void clearChild();
    SAItem* takeChild(int row);
    SAItem* parent() const;
    //用于记录当前所处的层级，如果parent不为nullptr，这个将返回parent下次item对应的层级,如果没有parent，返回-1
    int fieldRow() const;
public:
    //接口，获取类型名称和类型id
    virtual QString getTypeName() const = 0;
    virtual int getType() const = 0;
protected:
    void setID(int id);
};
#endif // SAITEM_H
