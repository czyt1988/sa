#ifndef SADATAFEATUREITEM_H
#define SADATAFEATUREITEM_H
#include "SALibGlobal.h"
#include <QStandardItem>
class QXmlStreamWriter;
class QXmlStreamReader;
///
/// \brief DataFeatureItem接口
///
class SALIB_EXPORT SADataFeatureItem : public QStandardItem
{
public:
    SADataFeatureItem();
    SADataFeatureItem(const QString & text);
    //SADataFeatureItem(const SADataFeatureItem::ItemType & type, const QString & text);
    SADataFeatureItem(double data);
    SADataFeatureItem(int data);
    SADataFeatureItem(const QPointF& data);
    virtual ~SADataFeatureItem();
    enum ItemType{
        UnKnow
        ,DescribeItem///< 用于描述的条目，和值项目关联
        ,ValueItem///< 值项目
        ,PointItem///< 点项目
        ,VectorValueItem///< 值系列项目
        ,VectorPointItem///< 点系列项目项目
    };
    //子条目添加点序列条目
    void appendItem(const QString &name, const QVector<QPointF>& datas);
    //子条目添加点序列条目
    void appendItem(const QString &name,const QVector<double>& datas);
    //插入一个double条目
    void appendItem(const QString &name,int data);
    //插入一个double条目
    void appendItem(const QString &name,double data);
    //插入一个QPointF条目
    void appendItem(const QString &name,const QPointF& data);
    //获取显示的内容
    QVariant displayRole() const;
    //获取条目类型
    ItemType getItemType() const;
    //设置条目类型
    void setItemType(ItemType type);
    //获取最顶层条目
    SADataFeatureItem* topParent() const;
    //转换为xml，此转换不是标准xml文件，而是以<fi>打头</fi>结尾的xml文件
    QString toXml() const;
    //从xml转换为item
    void fromXml(const QString& xml);
    //转换为xml，此转换不是标准xml文件，而是以<fi>打头</fi>结尾的xml文件
    static QString toXml(const SADataFeatureItem* item);
    //从xml转换为item
    static void fromXml(const QString& xmlStr, SADataFeatureItem* item);
private:
    static int getTypeInt(const QStandardItem* item);
    static void writeItem(QXmlStreamWriter* xml,const QStandardItem* item);
    static void readItem(QXmlStreamReader* xml, SADataFeatureItem *item);
};
Q_DECLARE_METATYPE(SADataFeatureItem)

#endif // SADATAFEATUREITEM_H
