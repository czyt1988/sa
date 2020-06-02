#include "SATcpDataProcessClient.h"
#include "SAServeHandleFun.h"
#include "SAServerDefine.h"
#include "SAXMLProtocolParser.h"

class SATcpDataProcessClientPrivate
{
    SA_IMPL_PUBLIC(SATcpDataProcessClient)
public:
    SATcpDataProcessClientPrivate(SATcpDataProcessClient* p);
};

SATcpDataProcessClientPrivate::SATcpDataProcessClientPrivate(SATcpDataProcessClient *p):q_ptr(p)
{

}

SATcpDataProcessClient::SATcpDataProcessClient(QObject *par):SATcpClient(par)
{

}

SATcpDataProcessClient::~SATcpDataProcessClient()
{

}

/**
 * @brief 把点序列转换为variant
 * @param arrs 点序列
 * @return
 */
QVariant SATcpDataProcessClient::vectorpointsToVariant(const QVector<QPointF> &arrs)
{
    QVariantList varlist;
    varlist.reserve(arrs.size());
    for(const QPointF& v : arrs)
    {
        varlist.append(v);
    }
    return QVariant(varlist);
}
/**
 * @brief variant转为QVector<QPointF>
 * @param var
 * @return
 */
QVector<QPointF> SATcpDataProcessClient::variantToVectorpoints(const QVariant &var)
{
    QVector<QPointF> arr;
    QVariantList varlist = var.toList();
    for(const QVariant& v : varlist)
    {
        if(v.canConvert<QPointF>())
        {
            arr.append(v.toPointF());
        }
    }
    return arr;
}

/**
 * @brief 处理xml协议
 * @param header
 * @param xml
 * @return
 */
bool SATcpDataProcessClient::dealXmlProtocol(const SAProtocolHeader &header, SAXMLProtocolParserPtr xml)
{
    if(SATcpClient::dealXmlProtocol(header,xml))
    {
        return true;
    }
    //SATcpClient::dealXmlProtocol返回false说明有没有处理的协议
    switch (header.protocolFunID) {
    case SA::ProtocolFunReply2DPointsDescribe:
        dealReply2DPointsDescribe(header,xml);
        break;
    default:
        break;
    }
    return false;
}

/**
 * @brief SATcpDataProcessClient::dealReply2DPointsDescribe
 * @param header
 * @param xml
 * @return
 */
bool SATcpDataProcessClient::dealReply2DPointsDescribe(const SAProtocolHeader &header,
                                                       SAXMLProtocolParserPtr xml)
{
    emit reply2DPointsDescribe(header,xml);
    return true;
}

/**
 * @brief 请求2维数据的统计描述
 * @param arrs 待计算的点序列
 * @param key 标致，返回的reply中会带着此key，用于区别请求的回复
 * @param sortcount 返回排序的前后n个值
 */
bool SATcpDataProcessClient::request2DPointsDescribe(const QVector<QPointF> &arrs, uint key, int sortcount)
{
    SAXMLProtocolParser data;
    data.setClassID(SA::ProtocolTypeXml);
    data.setFunctionID(SA::ProtocolFunReq2DPointsDescribe);
    data.setValue("key",key);
    data.setValue("points",vectorpointsToVariant(arrs));
    data.setValue("sort-count",sortcount);
    return SA::write_xml_protocol(getSocket(),&data,SA::ProtocolFunReq2DPointsDescribe,key,0);
}


