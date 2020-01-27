#ifndef SAXMLPROTOCOLPARSER_H
#define SAXMLPROTOCOLPARSER_H
#include "SAProtocolGlobal.h"
#include "SAAbstractProtocolParser.h"
class SAXMLProtocolParserPrivate;

class SAXMLProtocolParser : public SAAbstractProtocolParser
{
    SA_IMPL(SAXMLProtocolParser)
public:
    SAXMLProtocolParser(QObject* par = nullptr);
   // 设置协议的内容
   virtual bool setProtocolData(const QByteArray& data);
   // 检测是否存在分组
   virtual bool isHasGroup(const QString& groupName) const;
   // 检查在分组名下是否存在对应的键值 复杂度O(1)
   virtual bool isHasKey(const QString& groupName, const QString& keyName) const;
   // 获取键值对应的内容
   virtual QVariant getValue(const QString& groupName, const QString& keyName,const QVariant& defaultVal = QVariant()) const;
public:
    // 获取错误信息
    QString getErrorString() const;
};

#endif // SAXMLPROTOCOLPARSER_H
