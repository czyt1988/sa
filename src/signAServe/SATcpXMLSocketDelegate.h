#ifndef SATCPXMLSOCKETDELEGATE_H
#define SATCPXMLSOCKETDELEGATE_H
#include "SAServeGlobal.h"
#include "SATcpSocketDelegate.h"
class SATcpXMLSocketDelegatePrivate;
/**
 * @brief 基于xml协议的协议解析
 */
class SASERVE_EXPORT SATcpXMLSocketDelegate : public SATcpSocketDelegate
{
    SA_IMPL(SATcpXMLSocketDelegate)
    Q_OBJECT
public:
    SATcpXMLSocketDelegate(SATcpSocket* socket);
    ~SATcpXMLSocketDelegate();
    //处理数据
    virtual bool deal(const SAProtocolHeader& header,const QByteArray& data) override;
public slots:
    //请求token
    virtual void requestToken(int pid,const QString& appid) override;
private:
    void emitReplyToken(const QString& token,int sequenceID);
};

#endif // SATCPXMLSOCKETDELEGATE_H
