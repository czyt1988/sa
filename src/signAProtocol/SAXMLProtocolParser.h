#ifndef SAXMLPROTOCOLPARSER_H
#define SAXMLPROTOCOLPARSER_H
#include "SAProtocolGlobal.h"
#include "SAAbstractProtocolParser.h"
class SAXMLProtocolParserPrivate;

/**
 * @brief SA XML协议的读写类
 * sa xml协议主要用于保存qvariant类型数据，形如
 * @code
 * <sa>
 *  <values>
 *   <group name="test">
 *      <item name="a" type="int">1</item>
 *   </group>
 *  </values>
 * </sa>
 * @endcode
 *
 * 协议所有内容在values下，values下需要有对应的组，组可以用户自定义
 *
 * 此类使用@sa SAAbstractProtocolParser::setValue 函数进行变量的写入
 *
 *
 * @code
 * QVariantList points;
 * points << QPointF(1,2)<< QPointF(1,3)<< QPointF(2,3)<< QPointF(4,5);
 * int sequenceID = 123;
 * SAXMLProtocolParser xml;
 * xml.setValue("value",1);
 * xml.setValue("g","point-size",points.size());
 * xml.setValue("g","sequenceID",sequenceID);
 * xml.setValue("g","points",points);
 * @endcode
 *
 * 上诉输出结果为：
 *
 * @code
 * <sa type="xml" classid="" funid="">
 *  <values>
 *   <default-group>
 *     <item type="int" name="value">1</item>
 *   </default-group>
 *   <group name="g">
 *     <item type="int" name="point-size">4</item>
 *     <item type="int" name="sequenceID">123</item>
 *     <item type="QVariantList" name="points">
 *        <item type="QPointF" name="">1;2</item>
 *        <item type="QPointF" name="">1;3</item>
 *        <item type="QPointF" name="">2;3</item>
 *        <item type="QPointF" name="">4;5</item>
 *     </item>
 *   </group>
 *  </values>
 * </sa>
 * @endcode
 *
 */
class SA_PROTOCOL_EXPORT SAXMLProtocolParser : public SAAbstractProtocolParser
{
    SA_IMPL(SAXMLProtocolParser)
public:
    SAXMLProtocolParser();
    SAXMLProtocolParser(const SAXMLProtocolParser& other);
    //移动构造函数
    SAXMLProtocolParser(SAXMLProtocolParser&& other);
    SAXMLProtocolParser &operator =(const SAXMLProtocolParser& other);
    virtual ~SAXMLProtocolParser();

    //设置协议功能号
    virtual void setFunctionID(int funid);
    //获取协议功能号
    virtual int getFunctionID() const;
    //设置协议类号
    virtual void setClassID(int classid);
    //获取协议类号
    virtual int getClassID() const;
    //设置值
    virtual void setValue(const QString& groupName, const QString& keyName, const QVariant& var);
    virtual void setValue(const QString& keyName, const QVariant& var);
    // 复杂度O(1)
    virtual QStringList getGroupNames() const;
    // 复杂度O(n)
    virtual QStringList getKeyNames(const QString& groupName) const;
    // 从文本转换
    virtual bool fromString(const QString& str);
    //转换为文本
    virtual QString toString() const;
    // 设置协议的内容
    virtual bool fromByteArray(const QByteArray& data);
    // 转换为bytearray
    virtual QByteArray toByteArray() const;
    // 检测是否存在分组
    virtual bool isHasGroup(const QString& groupName) const;
    // 检查在分组名下是否存在对应的键值 复杂度O(1)
    virtual bool isHasKey(const QString& groupName, const QString& keyName) const;
    // 获取键值对应的内容
    virtual QVariant getValueInGroup(const QString& groupName, const QString& keyName,const QVariant& defaultVal = QVariant()) const;
    virtual QVariant getValue(const QString& keyName,const QVariant& defaultVal = QVariant()) const;
public:
    // 获取错误信息
    QString getErrorString() const;
};



#endif // SAXMLPROTOCOLPARSER_H
