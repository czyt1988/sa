#include "SAServeHandleFun.h"
#include "SAXMLProtocolParser.h"
#include "SAServerDefine.h"
#include "SAXMLTagDefined.h"
#include <QCryptographicHash>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDataStream>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <memory>
#include "SAVariantCaster.h"
#include "SACRC.h"
#include "SAPoint.h"

static QMap<SAPoint,SA::FunHandle> fun_handle_map = QMap<SAPoint,SA::FunHandle>();
QMutex fun_handle_mutex;
//把variant写入item里
void write_variant_property_to_xml(QXmlStreamWriter& xml,const QMap<QString, QVariant>& props);
void write_variant_item_to_xml(QXmlStreamWriter& xml,const QMap<int, QVariant>& props);
void write_variant_to_xml_item(QXmlStreamWriter& xml,const QString& name,const QVariant& value);
void write_saitem_to_xml(QXmlStreamWriter& xml,const SAItem* item);

bool SA::ensure_write(const char *data, qint64 len,SATcpSocket* socket,short maxtry)
{
    int count = 0;
    qint64 wed = 0;
    do
    {
        wed += (socket->write(data+wed,len-wed));
        ++count;
    }while(wed < len && count < maxtry);
    return (wed == len);
}
bool SA::ensure_write(const QByteArray &data,SATcpSocket* socket,short maxtry)
{
    qint64 wed = socket->write(data);
    if(wed < data.size())
    {
        //没写完继续写
        return ensure_write(data.data()+wed,data.size()-wed,socket,maxtry);
    }
    return true;
}

/**
 * @brief 写socket
 * @param header 协议头
 * @param data 协议数据
 * @param socket socket
 * @return
 */
bool SA::write(const SAProtocolHeader &header, const QByteArray &data, SATcpSocket *socket)
{
    bool stat = false;
    stat = ensure_write((const char*)(&header),sizeof(SAProtocolHeader),socket);
    if(data.size()>0)
    {
        stat &= ensure_write(data,socket);
    }
    socket->flush();
    return stat;
}

/**
 * @brief 写xml协议
 * @param socket
 * @param xml
 * @param sequenceID
 * @param extendValue
 * @return
 */
bool SA::write_xml_protocol(SATcpSocket *socket, const SAXMLProtocolParser *xml, int funid, int sequenceID, uint32_t extendValue)
{
    QByteArray data = xml->toByteArray();
    SAProtocolHeader header;
    header.init();
    header.protocolFunID = funid;
    header.dataSize = data.size();
    header.protocolTypeID = SA::ProtocolTypeXml;
    header.dataCrc32 = SACRC::crc32(data);
    header.sequenceID = sequenceID;
    header.extendValue = extendValue;
    return write(header,data,socket);
}

/**
 * @brief 注册处理函数
 *
 * serve的处理将通过注册的函数获取处理的方法，如果不注册将无法处理，通过此函数同时可以修改已有的处理方法
 * @param funid 函数id
 * @param fun 函数指针
 */
void SA::register_serve_funciton_handle(int classid, int funid, SA::FunHandle fun)
{
    QMutexLocker locker(&fun_handle_mutex);
    fun_handle_map[SAPoint(classid,funid)] = fun;
}


/**
 * @brief 获取处理的方法
 * @param funid 功能id
 * @return 对应处理方法指针，如果没有返回nullptr
 */
SA::FunHandle SA::get_serve_funciton_handle(int classid,int funid)
{
    QMutexLocker locker(&fun_handle_mutex);
    return fun_handle_map.value(SAPoint(classid,funid),nullptr);
}

/**
 * @brief 初始化服务功能处理指针
 */
void SA::init_serve_funciotn_handle()
{

}

/**
 * @brief 根据pid和appid创建token
 * @param pid
 * @param appID
 * @return
 */
QString SA::make_token(int pid, const QString &appID)
{
    QByteArray pidraw;
    pidraw.setNum(pid);
    QByteArray mixcode = QCryptographicHash::hash(pidraw,QCryptographicHash::Md5)
            + QCryptographicHash::hash(appID.toUtf8(),QCryptographicHash::Sha3_256);
    QString res = QString(mixcode.toBase64()).toUtf8();
    return res;
}

/**
 * @brief 客户端-发出token请求
 * @param pid
 * @param appid
 * @param socket
 * @return
 */
bool SA::request_token_xml(int pid, const QString &appid, SATcpSocket *socket, int sequenceID, uint32_t extendValue)
{
    FUNCTION_RUN_PRINT();
    //请求token
    SAXMLProtocolParser data;
    data.setClassID(SA::ProtocolTypeXml);
    data.setFunctionID(SA::ProtocolFunReqToken);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",pid);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid",appid);
    return write_xml_protocol(socket,&data,SA::ProtocolFunReqToken,sequenceID,extendValue);
}







/**
 * @brief 请求心跳
 * @param socket
 * @return
 */
bool SA::request_heartbreat(SATcpSocket *socket)
{
    FUNCTION_RUN_PRINT();
    SAProtocolHeader header;
    header.init();
    header.dataSize = 0;
    header.protocolTypeID = SA::ProtocolTypeHeartbreat;
    header.protocolFunID = SA::ProtocolFunReqHeartbreat;
    header.extendValue = 1; // 心跳请求值为1
    return write(header,QByteArray(),socket);
}







/**
 * @brief 把xml转换为tree
 * @param xml
 * @param tree
 * @return
 */
bool SA::cast_protocol_to_satree(const SAXMLProtocolParser *xml, SATree *tree)
{
    tree->setProperty("classid",xml->getClassID());
    tree->setProperty("funid",xml->getFunctionID());
    QStringList dfk = xml->getKeyNames();//获取默认分组的item
    for(const QString& k : dfk)
    {
        QVariant v = xml->getDefaultGroupValue(k);
        if (v.isValid())
        {
            std::unique_ptr<SAItem> i = std::make_unique<SAItem>();
            i->setName(k);
            i->setProperty(SAItem::RoleValue,v);
            tree->appendItem(i.release());
        }
    }
    QStringList gs = xml->getGroupNames();
    for (const QString& g : gs)
    {
        SAItem* gi = new SAItem(g);
        QStringList dfk = xml->getKeyNames(g);
        for(const QString& k : dfk)
        {
            QVariant v = xml->getValue(g,k);
            if (v.isValid())
            {
                std::unique_ptr<SAItem> i = std::make_unique<SAItem>();
                i->setName(k);
                i->setProperty(SAItem::RoleValue,v);
                tree->appendItem(i.release());
            }
        }
        tree->appendItem(gi);
    }
    return true;
}

/**
 * @brief SA::cast_satree_to_xml
 * @param tree
 * @return
 */
QString SA::cast_satree_to_xml(const SATree *tree)
{
    QString str;
    QXmlStreamWriter xml(&str);
    xml.setAutoFormatting(true);
    //<?xml version='1.0' encoding='UTF-8'?>
    //<sa funid="-1" classid="-1" type="xml">
    xml.writeStartDocument();
    xml.writeStartElement("sa");
    xml.writeAttribute("funid","-2");
    xml.writeAttribute("classid","-2");
    xml.writeAttribute("type","xml");
    // <prop>
    //   <item name="firstStart" type="bool">0</item>
    // </prop>
    write_variant_property_to_xml(xml,tree->getPropertys());
    //values
    QList<SAItem*> items = tree->getItems();
    xml.writeStartElement(SA_XML_TAG_VALUES);
    for(const SAItem* i : items)
    {
        write_saitem_to_xml(xml,i);
    }
    xml.writeEndElement(); // values

    xml.writeEndElement(); // sa
    xml.writeEndDocument();
    return str;
}

/**
 * @brief 把属性写入属性栏中
 * @param xml
 * @param props
 */
void write_variant_property_to_xml(QXmlStreamWriter& xml,const QMap<QString, QVariant>& props)
{
    xml.writeStartElement(SA_XML_TAG_PROPERTY);
    for(auto i = props.begin();i!=props.end();++i)
    {
        write_variant_to_xml_item(xml,i.key(),i.value());
    }
    xml.writeEndElement(); // prop
}
/**
 * @brief 批量写入item
 * @param xml
 * @param props
 */
void write_variant_item_to_xml(QXmlStreamWriter& xml,const QMap<int, QVariant>& props)
{
    for(auto i = props.begin();i!=props.end();++i)
    {
        write_variant_to_xml_item(xml,QString::number(i.key()),i.value());
    }
}

void write_variant_to_xml_item(QXmlStreamWriter& xml,const QString& name,const QVariant& value)
{
    xml.writeStartElement(SA_XML_TAG_ITEM);
    QString vartype = value.typeName();
    if (!name.isNull())
    {
        xml.writeAttribute(SA_XML_ATT_NAME,name);
    }
    xml.writeAttribute(SA_XML_ATT_TYPE,vartype);
    if(0 == QString::compare(vartype,SA_XML_VAR_ARR_LIST,Qt::CaseInsensitive))
    {
        QList<QVariant> l = value.toList();
        for (auto i = l.begin();i!=l.end();++i)
        {
            write_variant_to_xml_item(xml,QString::null,*i);
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_MAP,Qt::CaseInsensitive))
    {
        QMap<QString, QVariant> l = value.toMap();
        for (auto i = l.begin();i!=l.end();++i)
        {
            write_variant_to_xml_item(xml,i.key(),i.value());
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_HASH,Qt::CaseInsensitive))
    {
        QHash<QString, QVariant> l = value.toHash();
        for (auto i = l.begin();i!=l.end();++i)
        {
            write_variant_to_xml_item(xml,i.key(),i.value());
        }
    }
    else if (0 == QString::compare(vartype,SA_XML_VAR_ARR_STRLIST,Qt::CaseInsensitive))
    {
        QStringList l = value.toStringList();
        for (auto i = l.begin();i!=l.end();++i)
        {
            write_variant_to_xml_item(xml,QString::null,*i);
        }
    }
    else
    {
        xml.writeCharacters(SAVariantCaster::variantToString(value));
    }
    xml.writeEndElement();
}

/**
 * @brief 把saitem写入到xml中，saitem作为saprotocol的group对象
 * @param xml QXmlStreamWriter 此时的xml必须是在一个group或者再values下
 * @param item
 */
void write_saitem_to_xml(QXmlStreamWriter& xml,const SAItem* item)
{
    xml.writeStartElement(SA_XML_TAG_GROUP);
    xml.writeAttribute(SA_XML_ATT_NAME,item->getName());
    xml.writeAttribute(SA_XML_ATT_ID,QString::number(item->getID()));
    QIcon icon = item->getIcon();
    if(!icon.isNull())
    {
        QByteArray byte;
        QDataStream st(&byte,QIODevice::ReadWrite);
        st << icon;
        xml.writeAttribute(SA_XML_ATT_ICON,byte.toBase64());
    }
    //写入property xml protocol的item标签写入
    if(item->getPropertyCount() > 0)
    {
        write_variant_item_to_xml(xml,item->getPropertys());
    }
    //写入子item
    QList<SAItem*> childs = item->getChildItems();
    for(const SAItem* i : childs)
    {
        write_saitem_to_xml(xml,i);
    }
    xml.writeEndElement();
}
