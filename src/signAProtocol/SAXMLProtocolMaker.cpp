#include "SAXMLProtocolMaker.h"
#include "SAProtocolHeader.h"
#include "SAXMLProtocolReadWriter.h"
#include "SACRC.h"
SAXMLProtocolMaker::SAXMLProtocolMaker()
{
    
}

SAXMLProtocolMaker::~SAXMLProtocolMaker()
{
    
}

/**
 * @brief 组成2d数据点描述，类似pandas.DataFrame.describe
 * @param points
 * @param sequenceID
 * @return
 */
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

/**
 * @brief 组成2d数据点描述的结果
 * @param res
 * @return
 */
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
