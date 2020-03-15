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

SATcpDataProcessClientPrivate::SATcpDataProcessClientPrivate(SATcpDataProcessClient *p)
{

}

SATcpDataProcessClient::SATcpDataProcessClient(QObject *par):SATcpClient(par)
{

}

SATcpDataProcessClient::~SATcpDataProcessClient()
{

}

bool SATcpDataProcessClient::deal(const SAProtocolHeader &header, const QByteArray &data)
{
    if(SATcpClient::deal(header,data))
    {
        return true;
    }
    switch(header.protocolTypeID)
    {
    case SA::ProtocolTypeXml:
    {
        SAXMLProtocolParser xml;
        if(!xml.fromByteArray(data))
        {
            return false;
        }
        break;
    }
    default:
        break;
    }


//    SAXMLProtocolParser xml;
//    if(!xml.fromByteArray(data))
//    {
//        return false;
//    }
//    QVariant var = xml.getValueInDefaultGroup("points");
//    QVector<QPointF> arr = variant2vectorpoints(var);
//    if(arr.size() <= 0)
//    {

//    }
    return false;
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
 * @brief 请求2维数据的统计描述
 * @param arrs 待计算的点序列
 * @param key 标致，返回的reply中会带着此key，用于区别请求的回复
 */
void SATcpDataProcessClient::request2DPointsDescribe(const QVector<QPointF> &arrs, uint key)
{
    SAXMLProtocolParser data;
    data.setClassID(SA::ProtocolTypeXml);
    data.setFunctionID(SA::ProtocolFunReq2DPointsDescribe);
    data.setValueInDefaultGroup("key",key);
    data.setValueInDefaultGroup("points",vectorpointsToVariant(arrs));
    SA::write_xml_protocol(getSocket(),&data,SA::ProtocolFunReq2DPointsDescribe,key,0);
}


