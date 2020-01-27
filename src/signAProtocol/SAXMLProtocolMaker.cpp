#include "SAXMLProtocolMaker.h"
#include "SAProtocolHeader.h"
#include "SAXMLProtocolReadWriter.h"
#include "SACRC.h"
#include <QDomDocument>
#include <QHash>
#include "SAXMLTagDefined.h"
#include "SAVariantCaster.h"

class SAXMLProtocolMakerPrivate
{
    SA_IMPL_PUBLIC(SAXMLProtocolMaker)
public:
    SAXMLProtocolMakerPrivate(SAXMLProtocolMaker* p,const QString &roottype);
    //获取组队员的dom节点，如果没有就创建一个
    QDomElement getGroupEle(const QString& name);
    QDomElement getGroupEle_const(const QString& name) const;
    //写入value数据
    void setValue(const QString &groupName, const QString &keyName, const QVariant &var);
    //写入value数据，数据的父级对象不在mValuesEle下，而是指定在par下
    void setValue(const QString &keyName, const QVariant &var, QDomElement par);
    //获取组写的所有item名
    QStringList getKeyNames(const QString &groupName) const;
    //
    //由于是随机读写，不能用xmlstream
    //
    QDomDocument mDoc;
    QDomElement mRootEle; //根节点
    QDomElement mValuesEle; //根节点
    QHash<QString,QDomElement> mGroupEle; //保存所有的组节点
};

SAXMLProtocolMakerPrivate::SAXMLProtocolMakerPrivate(SAXMLProtocolMaker *p, const QString &roottype):q_ptr(p)
{
    mRootEle = mDoc.createElement(SA_XML_TAG_SA);
    mRootEle.setAttribute(SA_XML_ATT_TYPE,roottype);
    mDoc.appendChild(mRootEle);
    mValuesEle = mDoc.createElement(SA_XML_TAG_VALUES);
    mRootEle.appendChild(mValuesEle);
}

QDomElement SAXMLProtocolMakerPrivate::getGroupEle(const QString &name)
{
    QDomElement g = mGroupEle.value(name,QDomElement());
    if (g.isNull())
    {
        //没有发现对应名字分组，创建一个
        g = mDoc.createElement(name);
        g.setAttribute(SA_XML_ATT_NAME,name);
        mGroupEle[name] = g;
    }
    return g;
}

QDomElement SAXMLProtocolMakerPrivate::getGroupEle_const(const QString &name) const
{
    return mGroupEle.value(name,QDomElement());
}

void SAXMLProtocolMakerPrivate::setValue(const QString &groupName, const QString &keyName, const QVariant &var)
{
    QDomElement g = getGroupEle(groupName);
    QDomElement item = mDoc.createElement(SA_XML_TAG_ITEM);
    QString vartype = var.typeName();
    item.setAttribute(SA_XML_ATT_NAME,keyName);
    item.setAttribute(SA_XML_ATT_TYPE,vartype);
    //对数组类型进行特殊处理
    if(0 == QString::compare(vartype,SA_XML_VAR_ARR_LIST,Qt::CaseInsensitive))
    {
        QList<QVariant> l = var.toList();
        for (auto i = l.begin();i!=l.end();++i)
        {
            setValue("",*i,item);
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_MAP,Qt::CaseInsensitive))
    {
        QMap<QString, QVariant> l = var.toMap();
        for (auto i = l.begin();i!=l.end();++i)
        {
            setValue(i.key(),i.value(),item);
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_HASH,Qt::CaseInsensitive))
    {
        QHash<QString, QVariant> l = var.toHash();
        for (auto i = l.begin();i!=l.end();++i)
        {
            setValue(i.key(),i.value(),item);
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_STRLIST,Qt::CaseInsensitive))
    {
        QStringList l = var.toStringList();
        for (auto i = l.begin();i!=l.end();++i)
        {
            setValue("",*i,item);
        }
    }
    else
    {
        item.setNodeValue(SAVariantCaster::variantToString(var));
    }
    g.appendChild(item);
}

void SAXMLProtocolMakerPrivate::setValue(const QString &keyName, const QVariant &var, QDomElement par)
{
    QDomElement item = mDoc.createElement(SA_XML_TAG_ITEM);
    QString vartype = var.typeName();
    item.setAttribute(SA_XML_ATT_NAME,keyName);
    item.setAttribute(SA_XML_ATT_TYPE,vartype);
    //对数组类型进行特殊处理
    if(0 == QString::compare(vartype,SA_XML_VAR_ARR_LIST,Qt::CaseInsensitive))
    {
        QList<QVariant> l = var.toList();
        for (auto i = l.begin();i!=l.end();++i)
        {
            setValue("",*i,item);
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_MAP,Qt::CaseInsensitive))
    {
        QMap<QString, QVariant> l = var.toMap();
        for (auto i = l.begin();i!=l.end();++i)
        {
            setValue(i.key(),i.value(),item);
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_HASH,Qt::CaseInsensitive))
    {
        QHash<QString, QVariant> l = var.toHash();
        for (auto i = l.begin();i!=l.end();++i)
        {
            setValue(i.key(),i.value(),item);
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_STRLIST,Qt::CaseInsensitive))
    {
        QStringList l = var.toStringList();
        for (auto i = l.begin();i!=l.end();++i)
        {
            setValue("",*i,item);
        }
    }
    else
    {
        item.setNodeValue(SAVariantCaster::variantToString(var));
    }
    par.appendChild(item);
}

QStringList SAXMLProtocolMakerPrivate::getKeyNames(const QString &groupName) const
{
    QStringList res;
    QDomElement g = getGroupEle_const(groupName);
    QDomNodeList items = g.elementsByTagName(SA_XML_TAG_ITEM);
    auto size = items.size();
    for (int i=0;i<size;++i)
    {
        QDomElement ie = items.at(i).toElement();
        if(ie.isNull())
        {
            continue;
        }
        QString n = ie.attribute(SA_XML_ATT_NAME);
        if(!n.isEmpty())
            res.append(n);
    }
    return res;
}


SAXMLProtocolMaker::SAXMLProtocolMaker(const QString &roottype, QObject *par):SAAbstractProtocolMaker(par)
  ,d_ptr(new SAXMLProtocolMakerPrivate(this,roottype))
{
    
}

SAXMLProtocolMaker::~SAXMLProtocolMaker()
{
    
}

void SAXMLProtocolMaker::setValue(const QString &groupName, const QString &keyName, const QVariant &var)
{
    d_ptr->setValue(groupName,keyName,var);
}

QStringList SAXMLProtocolMaker::getGroupNames() const
{
    return d_ptr->mGroupEle.keys();
}

QStringList SAXMLProtocolMaker::getKeyNames(const QString &groupName) const
{
    return d_ptr->getKeyNames(groupName);
}

QString SAXMLProtocolMaker::toString() const
{
    return d_ptr->mDoc.toString();
}


/*
QByteArray SAXMLProtocolMaker::make2DPointsDescribe(const QVector<QPointF> &points, uint sequenceID)
{
    SAXMLProtocolReadWriter xml;
    xml.changeToHeaderGroup();
    xml.writeValue("function",SA_XML_FUNTION_2D_POINTS_DESCRIBE);
    xml.writeValue("version","0.1");
    xml.changeToDefaultGroup();
    xml.writeValue("point-size",points.size());
    xml.writeValue("points",points);
    QString str = xml.toString();
    return makePackage(sequenceID,str.toUtf8());
}


QByteArray SAXMLProtocolMaker::make2DPointsDescribeResult(const QHash<QString, QVariant> &res,uint sequenceID)
{
    SAXMLProtocolReadWriter xml;
    xml.changeToHeaderGroup();
    xml.writeValue("function",SA_XML_FUNTION_2D_POINTS_DESCRIBE_RES);
    xml.writeValue("version","0.1");
    xml.changeToDefaultGroup();
    for (auto i = res.begin(); i != res.end();++i)
    {
        xml.writeVariantValue(i.key(),i.value());
    }
    QString str = xml.toString();
    return makePackage(sequenceID,str.toUtf8());
}

QByteArray SAXMLProtocolMaker::makePackage(uint sequenceID, const QByteArray &datas)
{
    QByteArray buffer;
    QDataStream ds(&buffer,QIODevice::WriteOnly);
    SAProtocolHeader h;
    h.init();
    h.sequenceID = sequenceID;
    h.classID = SA_PROTOCOL_CLASS_ID_XML_PROTOCOL;
    h.functionID = SA_PROTOCOL_FUNCTION_ID_STR_PROTOCOL;
    h.dataSize = datas.size();
    h.dataCrc32 = SACRC::crc32(datas);
    ds << h << datas;
    return buffer;
}
*/


