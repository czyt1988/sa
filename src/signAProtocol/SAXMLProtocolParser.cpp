#include "SAXMLProtocolParser.h"
#include <QDomDocument>
#include <QHash>
#include <QDomNodeList>
#include "SAXMLTagDefined.h"
#include "SAVariantCaster.h"

class SAXMLProtocolParserPrivate
{
    SA_IMPL_PUBLIC(SAXMLProtocolParser)
public:
    SAXMLProtocolParserPrivate(SAXMLProtocolParser* p);
    bool isValid() const;
    bool setProtocolData(const QByteArray &data);
    bool isHasGroup(const QString &groupName) const;
    bool isHasKey(const QString &groupName, const QString &keyName) const;
    QString makeFullItemName(const QString &groupName, const QString &keyName) const;
    QDomElement getGroup(const QString &groupName) const;
    QVariant getValue(const QString &groupName, const QString &keyName, const QVariant &defaultVal) const;
    QDomElement getValueEle(const QString &groupName, const QString &keyName) const;
    QDomDocument mDoc;
    QString mErrorMsg;// 错误信息
    QDomElement mRootEle; //根节点
    QDomElement mValuesEle; //根节点
    QHash<QString,QDomElement> mGroupEle; //保存所有的组节点
    QHash<QString,QDomElement> mItemEles; // 保存所有的item节点，key是GroupName/ItemName的组合
    bool mIsValid;
};

SAXMLProtocolParserPrivate::SAXMLProtocolParserPrivate(SAXMLProtocolParser *p)
    :q_ptr(p)
    ,mIsValid(false)
{

}

bool SAXMLProtocolParserPrivate::isValid() const
{
    return mIsValid;
}

bool SAXMLProtocolParserPrivate::setProtocolData(const QByteArray &data)
{
    bool isok = mDoc.setContent(data,&mErrorMsg);
    if (!isok)
        return false;
    // 获取根节点
    mRootEle = mDoc.documentElement();
    if (mRootEle.isNull())
    {
        mErrorMsg = QObject::tr("root element error");
        return false;
    }
    // 到这里就认为协议正确，无论后面有没有内容
    mIsValid = true;
    // 获取values节点,如果有多个values，会忽略后面的values
    QDomNodeList valuesEle = mRootEle.elementsByTagName(SA_XML_TAG_VALUES);
    if (valuesEle.isEmpty())
    {
        return mIsValid;
    }
    mValuesEle = valuesEle.at(0).toElement();
    // 获取group内容
    QDomNodeList groupEles = mValuesEle.elementsByTagName(SA_XML_TAG_GROUP);
    auto size = groupEles.size();
    for (auto i = 0;i<size;++i)
    {
        QDomElement ge = groupEles.at(i).toElement();
        QString groupname = ge.attribute(SA_XML_ATT_NAME);
        if (groupname.isEmpty())
            continue;
        mGroupEle[groupname] = ge;
        // 获取group下的item信息
        QDomNodeList itemEles = ge.elementsByTagName(SA_XML_TAG_ITEM);
        auto itemsize = itemEles.size();
        for (auto j = 0;j<itemsize;++j)
        {
            QDomElement ie = itemEles.at(j).toElement();
            QString itemname = ie.attribute(SA_XML_ATT_NAME);
            mItemEles[makeFullItemName(groupname,itemname)] = ie;
        }
    }
    return mIsValid;
}

bool SAXMLProtocolParserPrivate::isHasGroup(const QString &groupName) const
{
    return mGroupEle.contains(groupName);
}

bool SAXMLProtocolParserPrivate::isHasKey(const QString &groupName, const QString &keyName) const
{
    return mItemEles.contains(makeFullItemName(groupName,keyName));
}

QString SAXMLProtocolParserPrivate::makeFullItemName(const QString &groupName, const QString &keyName) const
{
    return groupName + "/" + keyName;
}

QDomElement SAXMLProtocolParserPrivate::getGroup(const QString &groupName) const
{
    return mGroupEle.value(groupName,QDomElement());
}

QVariant SAXMLProtocolParserPrivate::getValue(const QString &groupName, const QString &keyName, const QVariant &defaultVal) const
{
    QDomElement item = getValueEle(groupName,keyName);
    if (item.isNull())
        return defaultVal;
    //
    QString varstr = item.nodeValue();
    QString vartype = item.attribute(SA_XML_ATT_TYPE);
    if(QString::compare(vartype,""))
    return SAVariantCaster::stringToVariant(varstr,vartype);
}

QDomElement SAXMLProtocolParserPrivate::getValueEle(const QString &groupName, const QString &keyName) const
{
    return mItemEles.value(makeFullItemName(groupName,keyName),QDomElement());
}

SAXMLProtocolParser::SAXMLProtocolParser(QObject *par):SAAbstractProtocolParser(par)
  ,d_ptr(new SAXMLProtocolParserPrivate(this))
{

}

bool SAXMLProtocolParser::setProtocolData(const QByteArray &data)
{
    return d_ptr->setProtocolData(data);
}

bool SAXMLProtocolParser::isHasGroup(const QString &groupName) const
{
    return d_ptr->isHasGroup(groupName);
}

bool SAXMLProtocolParser::isHasKey(const QString &groupName, const QString &keyName) const
{
    return d_ptr->isHasKey(groupName,keyName);
}

QVariant SAXMLProtocolParser::getValue(const QString &groupName, const QString &keyName, const QVariant &defaultVal) const
{
    return d_ptr->getValue(groupName,keyName,defaultVal);
}

/**
 * @brief 获取错误信息
 * @return 在@sa setProtocolData 返回false时调用
 */
QString SAXMLProtocolParser::getErrorString() const
{
    return d_ptr->mErrorMsg;
}


