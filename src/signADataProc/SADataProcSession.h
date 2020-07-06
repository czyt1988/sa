#ifndef SADATAPROCSESSION_H
#define SADATAPROCSESSION_H
#include "SASocketHandle.h"
#include <memory>
#include <QFutureWatcher>
#include <QMutex>

/**
 * @brief 处理数据的session
 */
class SADataProcSession : public SASocketHandle,
        public std::enable_shared_from_this<SADataProcSession> //为了把this通过weak_ptr传递到线程中
{
public:
    SADataProcSession(SATcpSocket *socket,QObject* p = nullptr);
    ~SADataProcSession();
    //处理收到的请求
    virtual bool deal(const SAProtocolHeader &header, const QByteArray &data);
    //处理xml相关请求
    virtual bool dealXmlProtocol(const SAProtocolHeader &header, SASocketHandle::XMLDataPtr xml);
    //安全写socket
    bool safe_write(const SAProtocolHeader& header,const QByteArray& data);
protected:
    //处理2维点描述
    virtual bool deal2DPointsDescribe(const SAProtocolHeader &header, SASocketHandle::XMLDataPtr xml);
private:
    QMutex m_writemutex;///< 对应safe_write的锁
};

/**
 * @brief 创建SADataProcSection的工厂函数
 * @param socket
 * @param p
 * @return
 */
SASocketHandle* createDataProcSession(SATcpSocket* socket,QObject* p);

#endif // SADATAPROCSECTION_H
