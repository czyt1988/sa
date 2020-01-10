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
 * @brief 组成2d数据点统计包
 * @param points
 * @param sequenceID
 * @return
 */
QByteArray SAXMLProtocolMaker::make2DPointsDescribe(const QVector<QPointF> &points, uint sequenceID)
{
    SAXMLProtocolReadWriter xml;
    xml.changeToHeaderGroup();
    xml.writeValue("function",SA_XML_FUNTION_2D_POINTS_DESCRIBE);
    xml.changeToDefaultGroup();
    xml.writeValue("point-size",points.size());
    xml.writeValue("points",points);
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
