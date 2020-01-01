#ifndef SAABSTRACTPROTOCOLMAKER_H
#define SAABSTRACTPROTOCOLMAKER_H
#include <QVector>
#include <QPointF>
/**
 * @brief sa协议制造基类
 * @see SAAbstractProtocolParse
 */
class SAAbstractProtocolMaker
{
public:
    SAAbstractProtocolMaker();
    virtual ~SAAbstractProtocolMaker();
public:
    //制造2维点数组的协议
    virtual QByteArray make2DPointsStatistic(const QVector<QPointF>& points,uint sequenceID) = 0;
};

#endif // SAABSTRACTPROTOCOLMAKER_H
