#include "SAXMLProtocolMaker.h"
#include "SAProtocolHeader.h"
#include "SAXMLWriteHelper.h"
#include "SAXML
SAXMLProtocolMaker::SAXMLProtocolMaker()
{
    
}

SAXMLProtocolMaker::~SAXMLProtocolMaker()
{
    
}

QByteArray SAXMLProtocolMaker::make2DPointsStatistic(const QVector<QPointF> &points, uint sequenceID)
{
    SAXMLWriteHelper xml;
    xml.writeHeadValue("point-size",points.size());
    xml.writeHeadValue("sequenceID",sequenceID);
    //开始写入数据
    xml.writeContentValue("points",points);
    QString str = xml.toString();
    
}

QByteArray SAXMLProtocolMaker::makePackage(uint sequenceID,const QString &xml)
{
    SAProtocolHeader h;
    h.init();
    h.sequenceID = sequenceID;
    h.
}
