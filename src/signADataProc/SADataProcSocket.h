#ifndef SADATAPROCSESSION_H
#define SADATAPROCSESSION_H
#include "SATcpSocket.h"
#include <memory>
#include <QFutureWatcher>
#include <QMutex>

/**
 * @brief 处理数据的session
 */
class SADataProcSocket : public SATcpSocket
{
public:
    SADataProcSocket(QObject *p = nullptr);
    ~SADataProcSocket();

    //处理xml相关请求
    virtual bool dealXmlProtocol(const SAProtocolHeader& header, const SAXMLProtocolParser& xml) override;

protected:
    //处理2维点描述
    virtual bool deal2DPointsDescribe(const SAProtocolHeader& header, const SAXMLProtocolParser& xml);

private:
    //处理点序列的具体函数
    bool _deal2DPointsDescribe(const SAProtocolHeader& header, const SAXMLProtocolParser& xml);
};

#endif // SADATAPROCSECTION_H
