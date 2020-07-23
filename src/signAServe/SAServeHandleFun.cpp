#include "SAServeHandleFun.h"
#include "SAXMLProtocolParser.h"
#include "SAServerDefine.h"
#include "SAXMLTagDefined.h"
#include <QVariant>
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

//把variant写入item里
void write_variant_property_to_xml(QXmlStreamWriter& xml, const QMap<QString, QVariant>& props);
void write_variant_item_to_xml(QXmlStreamWriter& xml, const QMap<int, QVariant>& props);
void write_variant_to_xml_item(QXmlStreamWriter& xml, const QString& name, const QVariant& value);
void write_saitem_to_xml(QXmlStreamWriter& xml, const SAItem *item);

bool SA::ensure_write(const char *data, qint64 len, SATcpSocket *socket, short maxtry)
{
    int count = 0;
    qint64 wed = 0;

    do
    {
        wed += (socket->write(data + wed, len - wed));
        ++count;
    }while(wed < len && count < maxtry);
    return (wed == len);
}


bool SA::ensure_write(const QByteArray& data, SATcpSocket *socket, short maxtry)
{
    qint64 wed = socket->write(data);

    if (wed < data.size()) {
        //没写完继续写
        return (ensure_write(data.data() + wed, data.size() - wed, socket, maxtry));
    }
    return (true);
}


/**
 * @brief 写socket
 * @param header 协议头
 * @param data 协议数据
 * @param socket socket
 * @return
 */
bool SA::write(const SAProtocolHeader& header, const QByteArray& data, SATcpSocket *socket)
{
    bool stat = false;

    stat = ensure_write((const char *)(&header), sizeof(SAProtocolHeader), socket);
    if (data.size() > 0) {
        stat &= ensure_write(data, socket);
    }
    socket->flush();
    return (stat);
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
    return (write(header, data, socket));
}


/**
 * @brief 根据pid和appid创建token
 * @param pid
 * @param appID
 * @return
 */
QString SA::make_token(int pid, const QString& appID)
{
    QByteArray pidraw;

    pidraw.setNum(pid);
    QByteArray mixcode = QCryptographicHash::hash(pidraw, QCryptographicHash::Md5)
        + QCryptographicHash::hash(appID.toUtf8(), QCryptographicHash::Sha3_256);
    QString res = QString(mixcode.toBase64()).toUtf8();

    return (res);
}


/**
 * @brief 客户端-发出token请求
 * @param pid
 * @param appid
 * @param socket
 * @return
 */
bool SA::request_token_xml(int pid, const QString& appid, SATcpSocket *socket, int sequenceID, uint32_t extendValue)
{
    FUNCTION_RUN_PRINT();
    //请求token
    SAXMLProtocolParser data;

    data.setClassID(SA::ProtocolTypeXml);
    data.setFunctionID(SA::ProtocolFunReqToken);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT, "pid", pid);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT, "appid", appid);
    return (write_xml_protocol(socket, &data, SA::ProtocolFunReqToken, sequenceID, extendValue));
}


/**
 * @brief 解析token请求参数
 * @param xml
 * @param pid
 * @param appid
 * @return
 */
bool SA::receive_request_token_xml(const SAXMLProtocolParser *xml, int& pid, QString& appid)
{
    pid = xml->getDefaultGroupValue("pid").toInt();
    appid = xml->getDefaultGroupValue("appid").toString();
    return (true);
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
    return (write(header, QByteArray(), socket));
}


/**
 * @brief 把xml转换为tree
 * @param xml
 * @param tree
 * @return
 */
bool SA::cast_protocol_to_satree(const SAXMLProtocolParser *xml, SATree *tree)
{
    tree->setProperty("classid", xml->getClassID());
    tree->setProperty("funid", xml->getFunctionID());
    QStringList dfk = xml->getKeyNames(); //获取默认分组的item

    for (const QString& k : dfk)
    {
        QVariant v = xml->getDefaultGroupValue(k);
        if (v.isValid()) {
            std::unique_ptr<SAItem> i(new SAItem());
            i->setName(k);
            i->setProperty(SAItem::RoleValue, v);
            tree->appendItem(i.release());
        }
    }
    QStringList gs = xml->getGroupNames();

    for (const QString& g : gs)
    {
        SAItem *gi = new SAItem(g);
        QStringList dfk = xml->getKeyNames(g);
        for (const QString& k : dfk)
        {
            QVariant v = xml->getValue(g, k);
            if (v.isValid()) {
                std::unique_ptr<SAItem> i(new SAItem());
                i->setName(k);
                i->setProperty(SAItem::RoleValue, v);
                tree->appendItem(i.release());
            }
        }
        tree->appendItem(gi);
    }
    return (true);
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
    xml.writeAttribute("funid", "-2");
    xml.writeAttribute("classid", "-2");
    xml.writeAttribute("type", "xml");
    // <prop>
    //   <item name="firstStart" type="bool">0</item>
    // </prop>
    write_variant_property_to_xml(xml, tree->getPropertys());
    //values
    QList<SAItem *> items = tree->getItems();

    xml.writeStartElement(SA_XML_TAG_VALUES);
    for (const SAItem *i : items)
    {
        write_saitem_to_xml(xml, i);
    }
    xml.writeEndElement();          // values

    xml.writeEndElement();          // sa
    xml.writeEndDocument();
    return (str);
}


/**
 * @brief 把属性写入属性栏中
 * @param xml
 * @param props
 */
void write_variant_property_to_xml(QXmlStreamWriter& xml, const QMap<QString, QVariant>& props)
{
    xml.writeStartElement(SA_XML_TAG_PROPERTY);
    for (auto i = props.begin(); i != props.end(); ++i)
    {
        write_variant_to_xml_item(xml, i.key(), i.value());
    }
    xml.writeEndElement(); // prop
}


/**
 * @brief 批量写入item
 * @param xml
 * @param props
 */
void write_variant_item_to_xml(QXmlStreamWriter& xml, const QMap<int, QVariant>& props)
{
    for (auto i = props.begin(); i != props.end(); ++i)
    {
        write_variant_to_xml_item(xml, QString::number(i.key()), i.value());
    }
}


void write_variant_to_xml_item(QXmlStreamWriter& xml, const QString& name, const QVariant& value)
{
    xml.writeStartElement(SA_XML_TAG_ITEM);
    QString vartype = value.typeName();

    if (!name.isNull()) {
        xml.writeAttribute(SA_XML_ATT_NAME, name);
    }
    xml.writeAttribute(SA_XML_ATT_TYPE, vartype);
    if (0 == QString::compare(vartype, SA_XML_VAR_ARR_LIST, Qt::CaseInsensitive)) {
        QList<QVariant> l = value.toList();
        for (auto i = l.begin(); i != l.end(); ++i)
        {
            write_variant_to_xml_item(xml, QString(), *i);
        }
    } else if (0 == QString::compare(vartype, SA_XML_VAR_ARR_MAP, Qt::CaseInsensitive)) {
        QMap<QString, QVariant> l = value.toMap();
        for (auto i = l.begin(); i != l.end(); ++i)
        {
            write_variant_to_xml_item(xml, i.key(), i.value());
        }
    } else if (0 == QString::compare(vartype, SA_XML_VAR_ARR_HASH, Qt::CaseInsensitive)) {
        QHash<QString, QVariant> l = value.toHash();
        for (auto i = l.begin(); i != l.end(); ++i)
        {
            write_variant_to_xml_item(xml, i.key(), i.value());
        }
    } else if (0 == QString::compare(vartype, SA_XML_VAR_ARR_STRLIST, Qt::CaseInsensitive)) {
        QStringList l = value.toStringList();
        for (auto i = l.begin(); i != l.end(); ++i)
        {
            write_variant_to_xml_item(xml, QString(), *i);
        }
    } else {
        xml.writeCharacters(SAVariantCaster::variantToString(value));
    }
    xml.writeEndElement();
}


/**
 * @brief 把saitem写入到xml中，saitem作为saprotocol的group对象
 * @param xml QXmlStreamWriter 此时的xml必须是在一个group或者再values下
 * @param item
 */
void write_saitem_to_xml(QXmlStreamWriter& xml, const SAItem *item)
{
    xml.writeStartElement(SA_XML_TAG_GROUP);
    xml.writeAttribute(SA_XML_ATT_NAME, item->getName());
    xml.writeAttribute(SA_XML_ATT_ID, QString::number(item->getID()));
    QIcon icon = item->getIcon();

    if (!icon.isNull()) {
        QByteArray byte;
        QDataStream st(&byte, QIODevice::ReadWrite);
        st << icon;
        xml.writeAttribute(SA_XML_ATT_ICON, byte.toBase64());
    }
    //写入property xml protocol的item标签写入
    if (item->getPropertyCount() > 0) {
        write_variant_item_to_xml(xml, item->getPropertys());
    }
    //写入子item
    QList<SAItem *> childs = item->getChildItems();

    for (const SAItem *i : childs)
    {
        write_saitem_to_xml(xml, i);
    }
    xml.writeEndElement();
}


/**
 * @brief 应答token
 * @param socket
 * @param header
 * @param pid
 * @param appid
 * @return
 */
bool SA::reply_token_xml(SATcpSocket *socket, const SAProtocolHeader& header, int pid, const QString& appid)
{
    QString token = SA::make_token(pid, appid);

    SAXMLProtocolParser reply;

    reply.setClassID(SA::ProtocolTypeXml);
    reply.setFunctionID(SA::ProtocolFunReplyToken);
    reply.setValue("token", token);
    return (write_xml_protocol(socket, &reply, SA::ProtocolFunReplyToken, header.sequenceID, header.extendValue));
}


/**
 * @brief 解析token请求的xml
 * @param xml
 * @param token
 * @return
 */
bool SA::receive_reply_token_xml(const SAXMLProtocolParser *xml, QString& token)
{
    token = xml->getDefaultGroupValue("token").toString();
    return (true);
}


/**
 * @brief 处理心跳请求
 * @param socket
 * @param header
 * @return
 */
bool SA::reply_heartbreat_xml(SATcpSocket *socket, const SAProtocolHeader& header)
{
    SAProtocolHeader replyheader;

    replyheader.init();
    replyheader.sequenceID = header.sequenceID;
    replyheader.dataSize = 0;
    replyheader.protocolTypeID = SA::ProtocolTypeHeartbreat;
    replyheader.protocolFunID = SA::ProtocolFunReplyHeartbreat;
    replyheader.extendValue = 0; // 心跳返回给客户端，此时值为0
    return (write(replyheader, QByteArray(), socket));
}


/**
 * @brief 请求2维数据的统计描述
 * @param socket socket
 * @param arrs 待计算的点序列
 * @param key 标致，返回的reply中会带着此key，用于区别请求的回复
 * @param sortcount 返回排序的前后n个值
 */
bool SA::request_2d_points_describe_xml(SATcpSocket *socket, const QVector<QPointF>& arrs, uint key, int sortcount)
{
    SAXMLProtocolParser xml;

    xml.setClassID(SA::ProtocolTypeXml);
    xml.setFunctionID(SA::ProtocolFunReq2DPointsDescribe);
    xml.setValue("key", key);
    xml.setValue("points", QVariant::fromValue<QVector<QPointF> >(arrs));
    xml.setValue("sort-count", sortcount);
#ifdef SA_SERVE_DEBUG_PRINT
    qDebug().noquote() << xml.toString();
#endif
    return (write_xml_protocol(socket, &xml, SA::ProtocolFunReq2DPointsDescribe, key, 0));
}


/**
 * @brief 异常的回复
 * @param socket
 * @param requestHeader
 * @param msg
 * @param errcode
 * @return
 */
bool SA::reply_error_xml(SATcpSocket *socket, const SAProtocolHeader& requestHeader, const QString& msg, int errcode)
{
    return (SA::reply_error_xml(socket, requestHeader.sequenceID, requestHeader.extendValue, msg, errcode));
}


/**
 * @brief 异常的回复
 * @param socket
 * @param sequenceID
 * @param extendValue
 * @param msg 异常信息
 * @param errcode 错误码
 * @return
 */
bool SA::reply_error_xml(SATcpSocket *socket, int sequenceID, int extendValue, const QString& msg, int errcode)
{
    SAXMLProtocolParser xml;

    xml.setClassID(SA::ProtocolTypeXml);
    xml.setFunctionID(ProtocolFunErrorOcc);
    xml.setValue("msg", msg);
    xml.setValue("errcode", errcode);
    return (write_xml_protocol(socket, &xml, ProtocolFunErrorOcc, sequenceID, extendValue));
}


/**
 * @brief 接收到错误信息
 * @param xml
 * @param sequenceID
 * @param extendValue
 * @param msg
 * @param errcode
 * @return
 */
bool SA::receive_error_xml(const SAXMLProtocolParser *xml, QString& msg, int& errcode)
{
    Q_CHECK_PTR(xml);
    msg = xml->getDefaultGroupValue("msg").toString();
    errcode = xml->getDefaultGroupValue("errcode").toInt();
    return (true);
}


/**
 * @brief 回复2维数组描述
 * @param socket
 * @param requestHeader
 * @param sum
 * @param mean
 * @param var
 * @param stdVar
 * @param skewness
 * @param kurtosis
 * @param min
 * @param max
 * @param mid
 * @param peak2peak
 * @param minPoint
 * @param maxPoint
 * @param midPoint
 * @param tops
 * @param lows
 * @return
 */
bool SA::reply_2d_points_describe_xml(SATcpSocket *socket, const SAProtocolHeader& requestHeader, double sum, double mean, double var, double stdVar, double skewness, double kurtosis, double min, double max, double mid, double peak2peak, const QPointF& minPoint, const QPointF& maxPoint, const QPointF& midPoint, const QVector<QPointF>& tops, const QVector<QPointF>& lows)
{
    SAXMLProtocolParser xml;

    xml.setClassID(SA::ProtocolTypeXml);
    xml.setFunctionID(ProtocolFunReply2DPointsDescribe);
    xml.setValue("sum", sum);
    xml.setValue("mean", mean);
    xml.setValue("var", var);
    xml.setValue("stdVar", stdVar);
    xml.setValue("skewness", skewness);
    xml.setValue("kurtosis", kurtosis);
    xml.setValue("min", min);
    xml.setValue("max", max);
    xml.setValue("mid", mid);
    xml.setValue("peak2peak", peak2peak);
    xml.setValue("min-point", minPoint);
    xml.setValue("max-point", maxPoint);
    xml.setValue("mid-point", midPoint);
    xml.setValue("sorted-lows", QVariant::fromValue(tops));
    xml.setValue("sorted-lows", QVariant::fromValue(lows));
    return (write_xml_protocol(socket, &xml, ProtocolFunReply2DPointsDescribe, requestHeader.sequenceID, requestHeader.extendValue));
}


/**
 * @brief 解析2维数组描述的回复
 * @param xml
 * @param sum
 * @param mean
 * @param var
 * @param stdVar
 * @param skewness
 * @param kurtosis
 * @param min
 * @param max
 * @param mid
 * @param peak2peak
 * @param minPoint
 * @param maxPoint
 * @param midPoint
 * @param tops
 * @param lows
 * @return
 */
bool SA::receive_reply_2d_points_describe_xml(const SAXMLProtocolParser *xml,
    double& sum, double& mean, double& var, double& stdVar,
    double& skewness, double& kurtosis,
    double& min, double& max, double& mid, double& peak2peak,
    QPointF& minPoint, QPointF& maxPoint, QPointF& midPoint,
    QVector<QPointF>& tops, QVector<QPointF>& lows)
{
    sum = xml->getDefaultGroupValue("sum").toDouble();
    mean = xml->getDefaultGroupValue("mean").toDouble();
    var = xml->getDefaultGroupValue("var").toDouble();
    stdVar = xml->getDefaultGroupValue("stdVar").toDouble();
    skewness = xml->getDefaultGroupValue("skewness").toDouble();
    kurtosis = xml->getDefaultGroupValue("kurtosis").toDouble();
    min = xml->getDefaultGroupValue("min").toDouble();
    max = xml->getDefaultGroupValue("max").toDouble();
    mid = xml->getDefaultGroupValue("mid").toDouble();
    peak2peak = xml->getDefaultGroupValue("peak2peak").toDouble();
    minPoint = xml->getDefaultGroupValue("minPoint").toPointF();
    maxPoint = xml->getDefaultGroupValue("maxPoint").toPointF();
    midPoint = xml->getDefaultGroupValue("midPoint").toPointF();
    tops = xml->getDefaultGroupValue("midPoint").value<QVector<QPointF> >();
    lows = xml->getDefaultGroupValue("midPoint").value<QVector<QPointF> >();
    return (true);
}
