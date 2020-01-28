#ifndef SAXMLPROTOCOLREADWRITER_H
#define SAXMLPROTOCOLREADWRITER_H
#include "SAProtocolGlobal.h"
#include <QVariant>
#include <QVector>
#include <QPointF>

class SAXMLProtocolReadWriterPrivate;


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
