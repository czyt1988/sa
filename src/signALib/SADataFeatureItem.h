#ifndef SADATAFEATUREITEM_H
#define SADATAFEATUREITEM_H
#include "SALibGlobal.h"
#include <QVariant>
#include <QHash>
#include <QBrush>
class QXmlStreamWriter;
class QXmlStreamReader;
class QDomElement;
class SADataFeatureItemPrivate;
///
/// \brief SADataFeatureItem接口,类似QStandardItem
///
class SALIB_EXPORT SADataFeatureItem
{
    Q_DISABLE_COPY(SADataFeatureItem)
    SA_IMPL(SADataFeatureItem)
public:
    SADataFeatureItem();
    SADataFeatureItem(const QString & text);

    virtual ~SADataFeatureItem();

    //获取最顶层条目
    SADataFeatureItem* topParent() const;
    //设置名称
    void setName(const QString& name);
    //获取名称
    QString getName() const;
    //like QStandardItem::data
    QVariant getData(int role) const;
    //like QStandardItem::setData
    void setData(const QVariant& var,int role);
    //插入一个double条目
    void appendItem(SADataFeatureItem *item);
    //父级
    SADataFeatureItem *getParent() const;
    //子条目数
    int getChildCount() const;
    //获取子节点
    SADataFeatureItem *getChild(int index) const;
    //获取当前行
    int getCurrentRowIndex() const;
    //设置子条目指针，返回旧的指针，返回的指针内存交由调用者管理
    SADataFeatureItem* setChild(int index,SADataFeatureItem* newItemPtr);
public:
    SADataFeatureItem(const QString &name,const QVariant& data);
    //值设置
    void setValue(const QVariant& var);
    QVariant getValue() const;

    //子条目添加点序列条目
    SADataFeatureItem* appendItem(const QString &name, const QVector<QPointF>& datas);
    //子条目添加点序列条目
    SADataFeatureItem *appendItem(const QString &name,const QVector<double>& datas);
    //插入一个double条目
    SADataFeatureItem *appendItem(const QString &name,const QVariant& data);
    //获取颜色
    QVariant getBackground() const;
    QBrush getBackgroundBrush() const;
    void setBackground(const QBrush& b);
    void setBackgroundColor(const QColor& c);
    //获取数据类型
    int getValueType() const;
    //描述类型，描述类型不显示value，只是作为group
    bool isGroupType() const;
    void setToGroup(bool isGroup = true);
protected:
    void setParent(SADataFeatureItem *parent);
private:
};


#endif // SADATAFEATUREITEM_H
