#ifndef SATCPDATAPROCESSSOCKET_H
#define SATCPDATAPROCESSSOCKET_H
#include "SAServeGlobal.h"
#include "SATcpSocket.h"
#include "SAXMLProtocolParser.h"
#include <memory>
#include <SAItem.h>

SA_IMPL_FORWARD_DECL(SATcpDataProcessSocket)

/**
 * @brief 用于数据处理的客户端
 */
class SASERVE_EXPORT SATcpDataProcessSocket : public SATcpSocket
{
    Q_OBJECT
    SA_IMPL(SATcpDataProcessSocket)
public:
    SATcpDataProcessSocket(QObject *par = nullptr);
    ~SATcpDataProcessSocket();

protected:
    virtual bool dealXmlProtocol(const SAProtocolHeader& header, const SAXMLProtocolParser& xml) override;

private:
    bool dealReply2DPointsDescribe(const SAProtocolHeader& header, const SAXMLProtocolParser& xml);

public slots:
    //请求2维数据的统计描述
    bool request2DPointsDescribe(const QVector<QPointF>& arrs, int sequenceID, int sortcount = 20);

signals:

    /**
     * @brief 返回数据点描述结果
     * @param res 结果以satree来进行描述
     */
    void receive2DPointsDescribe(double sum
        , double mean
        , double var
        , double stdVar
        , double skewness
        , double kurtosis
        , double min
        , double max
        , double mid
        , double peak2peak
        , const QPointF& minPoint
        , const QPointF& maxPoint
        , const QPointF& midPoint
        , const QVector<QPointF>& tops
        , const QVector<QPointF>& lows
        , int sequenceID
        , unsigned int extendValue);
};
Q_DECLARE_METATYPE(QVector<QPointF>)

#endif // SADATAPROCESSCLIENT_H
