#include "SATcpDataProcessClient.h"
#include "SAServeHandleFun.h"
#include "SAServerDefine.h"
#include "SAXMLProtocolParser.h"

class SATcpDataProcessClientPrivate
{
    SA_IMPL_PUBLIC(SATcpDataProcessClient)
public:
    SATcpDataProcessClientPrivate(SATcpDataProcessClient *p);
};

SATcpDataProcessClientPrivate::SATcpDataProcessClientPrivate(SATcpDataProcessClient *p) : q_ptr(p)
{
}


SATcpDataProcessClient::SATcpDataProcessClient(QObject *par) : SATcpClient(par)
{
}


SATcpDataProcessClient::~SATcpDataProcessClient()
{
}



/**
 * @brief 处理xml协议
 * @param header
 * @param xml
 * @return
 */
bool SATcpDataProcessClient::dealXmlProtocol(const SAProtocolHeader& header, SAXMLProtocolParserPtr xml)
{
    if (SATcpClient::dealXmlProtocol(header, xml)) {
        return (true);
    }
    //SATcpClient::dealXmlProtocol返回false说明有没有处理的协议
    switch (header.protocolFunID)
    {
    case SA::ProtocolFunReply2DPointsDescribe:
        dealReply2DPointsDescribe(header, xml);
        break;

    default:
        break;
    }
    return (false);
}


/**
 * @brief SATcpDataProcessClient::dealReply2DPointsDescribe
 * @param header
 * @param xml
 * @return
 */
bool SATcpDataProcessClient::dealReply2DPointsDescribe(const SAProtocolHeader& header,
    SAXMLProtocolParserPtr xml)
{
    emit reply2DPointsDescribe(header, xml);

    return (true);
}


/**
 * @brief 请求2维数据的统计描述
 * @param arrs 待计算的点序列
 * @param key 标致，返回的reply中会带着此key，用于区别请求的回复
 * @param sortcount 返回排序的前后n个值
 */
bool SATcpDataProcessClient::request2DPointsDescribe(const QVector<QPointF>& arrs, uint key, int sortcount)
{
    if (!getSocket()) {
        return (false);
    }
    SAXMLProtocolParser data;

    data.setClassID(SA::ProtocolTypeXml);
    data.setFunctionID(SA::ProtocolFunReq2DPointsDescribe);
    data.setValue("key", key);
    data.setValue("points", QVariant::fromValue<QVector<QPointF>>(arrs));
    data.setValue("sort-count", sortcount);
#ifdef SA_SERVE_DEBUG_PRINT
    qDebug().noquote() << data.toString();
#endif
    return (SA::write_xml_protocol(getSocket(), &data, SA::ProtocolFunReq2DPointsDescribe, key, 0));
}
