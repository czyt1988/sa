#ifndef SADATAPROCSESSION_H
#define SADATAPROCSESSION_H
#include "SASession.h"
#include <QFutureWatcher>
/**
 * @brief 处理数据的session
 */
class SADataProcSession : public SASession
{
public:
    SADataProcSession(SATcpSocket *socket,QObject* p = nullptr);
    ~SADataProcSession();
    //处理收到的请求
    virtual bool deal(const SAProtocolHeader &header, const QByteArray &data);
    //处理xml相关请求
    virtual bool dealXmlProtocol(const SAProtocolHeader &header, SASession::XMLDataPtr xml);
protected slots:
    //异步处理完deal2DPointsDescribe
    Q_SLOT void onFinishedFuture();
protected:
    //处理2维点描述
    virtual bool deal2DPointsDescribe(const SAProtocolHeader &header, SASession::XMLDataPtr xml);
private:

};

/**
 * @brief 创建SADataProcSection的工厂函数
 * @param socket
 * @param p
 * @return
 */
SASession* createDataProcSession(SATcpSocket* socket,QObject* p);

#endif // SADATAPROCSECTION_H
