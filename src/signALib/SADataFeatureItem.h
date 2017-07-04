#ifndef SADATAFEATUREITEM_H
#define SADATAFEATUREITEM_H
#include "SALibGlobal.h"
#include <QVariant>
#include <QHash>
#include <QBrush>
class QXmlStreamWriter;
class QXmlStreamReader;
///
/// \brief DataFeatureItem接口
///
class SALIB_EXPORT SADataFeatureItem
{
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
public:
    enum ItemType{
        UnKnow
        ,DescribeItem///< 用于描述的条目，和值项目关联
        ,ValueItem///< 值项目
        ,PointItem///< 点项目
    };
    SADataFeatureItem(const QString &name,const QVariant& data);
    //值设置
    void setValue(const QVariant& var);
    QVariant getValue() const;
    //转换为xml，此转换不是标准xml文件，而是以<fi>打头</fi>结尾的xml文件
    QString toXml() const;
    //从xml转换为item
    bool fromXml(const QString& xml);
    //转换为xml，此转换不是标准xml文件，而是以<fi>打头</fi>结尾的xml文件
    static QString toXml(const SADataFeatureItem* item);
    //从xml转换为item
    static bool fromXml(const QString& xmlStr, SADataFeatureItem* item);
    //获取条目类型
    ItemType getItemType() const;
    //设置条目类型
    void setItemType(ItemType type);
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

protected:
    void setParent(SADataFeatureItem *parent);
private:
    static int getTypeInt(const SADataFeatureItem *item);
    static void writeItem(QXmlStreamWriter* xml,const SADataFeatureItem* item);
    static bool readItem(QXmlStreamReader* xml, SADataFeatureItem *item);
private:
    QString m_name;
    SADataFeatureItem* m_parent;
    int m_currentRowIndex;
    QList<SADataFeatureItem*> m_childs;
    QVariant m_value;
    QHash<int,QVariant> m_datas;
};
Q_DECLARE_METATYPE(SADataFeatureItem)

#endif // SADATAFEATUREITEM_H
