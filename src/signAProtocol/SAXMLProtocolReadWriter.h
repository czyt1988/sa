#ifndef SAXMLPROTOCOLREADWRITER_H
#define SAXMLPROTOCOLREADWRITER_H
#include "SAProtocolGlobal.h"
#include <QVariant>
#include <QVector>
#include <QPointF>

class SAXMLProtocolReadWriterPrivate;

/**
 * @brief SA XML协议的读写类
 * sa xml协议分为协议头和内容区两大部分,一个空的协议报文如下：
 * @code
 * <sa>
 *  <root>
 *   <header></header>
 *   <content></content>
 *  </root>
 * </sa>
 * @endcode
 * 
 * 协议所有内容在root下，root下默认有header和content两个组
 * 组可以用户自定义，但任何报文都必须包含header和content，在大多传输协议中，header用于包含一些关键信息
 *
 * 此类使用@sa writeValue 函数进行变量的写入，写入位置视用户定义，默认在content组，用户可以通过@sa changeGroup切换分组
 *
 *
 * @code
 * QVector<QPointF> points;
 * points << QPointF(1,2)<< QPointF(1,3)<< QPointF(2,3)<< QPointF(2,3);
 * int sequenceID = 123;
 * SAXMLProtocolReadWriter xml;
 * xml.writeValue("point-size",points.size());
 * xml.writeValue("sequenceID",sequenceID);
 * xml.writeValue("points",points);
 * @endcode
 * 
 * 上诉输出结果为：
 *
 * @code
 * <sa>
 *  <root>
 *   <header>
 *   </header>
 *   <content>
 *     <item type="var-value" name="point-size" varType="uint">4</item>
 *     <item type="var-value" name="sequenceID" varType="int">123</item>
 *     <item type="vectorpointf-value" name="points">1,2|1,3|2,3|2,3|</item>
 *   </content>
 *  </root>
 * </sa>
 * @endcode
 *
 */
class SA_PROTOCOL_EXPORT SAXMLProtocolReadWriter
{
    SA_IMPL(SAXMLProtocolReadWriter)
public:
    SAXMLProtocolReadWriter();
    ~SAXMLProtocolReadWriter();
    //切换分组
    void changeGroup(const QString& name,bool createGopIfNone = true);
    //切换到content分组
    void changeToDefaultGroup();
    //切换到头部分区
    void changeToHeaderGroup();
    //判断当前是否在默认分组
    bool isInDefaultGroup() const;
    //在内容区写入数据
    void writeValue(const QString& name, const QString &d);
    void writeValue(const QString& name, int d);
    void writeValue(const QString& name, const QVector<QPoint>& d);
    void writeValue(const QString& name, const QVector<QPointF>& d);
    void writeVariantValue(const QString& name, const QVariant &d);
    //转换为文本
    QString toString(int indent = 1) const;
public:
    //获取用户分组信息
    QList<QString> getUserGroup() const;
    //获取所有Content区的变量名
    QList<QString> getContentGroupValueNames() const;
public:
    //把vector,list等类容器数据类型转换为文本
    template<typename T>
    static QString castToString(const QVector<T>& d,const QString& seprator = "|");
    //把接受QString::arg的数据类型转换为文本
    template<typename T>
    static QString castToString(const T& d);
    //把二维点转换为文本
    static QString castToString(const QPointF& d);
    static QString castToString(const QPoint& d);

};

/**
 * @brief 把容器类型的数据转换为文本
 * @param d 数据
 * @param seprator 分隔符，默认为|
 * @return 返回如"xx|xx|xxx|xx"的文本
 */
template<typename T>
QString SAXMLProtocolReadWriter::castToString(const QVector<T>& d, const QString &seprator)
{
    QString res;
    for (const T& v : d)
    {
        res += (castToString(v) + seprator);
    }
    return res;
}

/**
 * @brief 把接受QString::arg的数据类型转换为文本
 * @param d
 * @return 参照QString::arg的返回
 */
template<typename T>
QString SAXMLProtocolReadWriter::castToString(const T &d)
{
    return QString("%1").arg(d);
}
#endif // SAXMLWRITEHELPER_H
