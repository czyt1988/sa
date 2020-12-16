#ifndef SATCPDATAPROCESSSOCKET_H
#define SATCPDATAPROCESSSOCKET_H
#include "SAServeGlobal.h"
#include "SATcpSocket.h"
#include "SAXMLProtocol.h"
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
    virtual bool dealXmlProtocol(const SAProtocolHeader& header, const SAXMLProtocol& xml) override;

private:
    bool dealReply2DPointsDescribe(const SAProtocolHeader& header, const SAXMLProtocol& xml);

public slots:
    //请求2维数据的统计描述
    bool request2DPointsDescribe(const QVector<QPointF>& arrs, int sequenceID, int sortcount = 20);

signals:

    /**
     * @brief 返回数据点描述结果
     * @param res 结果以satree来进行描述
     */
    void receive2DPointsDescribe(const SAPropertiesGroup& res,int sequenceID,unsigned int extendValue);
};
Q_DECLARE_METATYPE(QVector<QPointF>)
#endif // SADATAPROCESSCLIENT_H
