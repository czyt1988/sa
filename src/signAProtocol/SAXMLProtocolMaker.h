#ifndef SAXMLPROTOCOLMAKER_H
#define SAXMLPROTOCOLMAKER_H
#include "SAAbstractProtocolMaker.h"

/**
 * @brief 组成sa xml协议的实现类
 * 对应
 */
class SAXMLProtocolMaker : public SAAbstractProtocolMaker
{
public:
    SAXMLProtocolMaker();
    ~SAXMLProtocolMaker();
    //制造2维点数组的协议
    virtual QByteArray make2DPointsStatistic(const QVector<QPointF>& points,uint sequenceID) override;
    //组包
    static QByteArray makePackage(uint sequenceID,const QString& xml);
};

#endif // SAXMLPROTOCOLMAKER_H
