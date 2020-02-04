#ifndef SATCPSOCKETDELEGATE_H
#define SATCPSOCKETDELEGATE_H
#include "SAServeGlobal.h"
#include "SAProtocolHeader.h"
class SATcpSocket;
class SATcpSocketDelegatePrivate;
/**
 * @brief 这是处理socket的私有类，对数据的处理通过此类来实现
 *
 * 通过重写SATcpSocketDelegate::deal函数可以改变delegate的处理数据内容
 *
 * @note 由于satcpsocket在多线程运行，一般不会和正常处理在一个线程，因此在信号槽的关联中要注意使用Qt::QueuedConnection，
 * 创建SATcpSocketDelegate，需要使用SATcpSocket::makeDelegate函数进行创建，避免在不同线程创建object
 */
class SASERVE_EXPORT SATcpSocketDelegate : public QObject
{
    SA_IMPL(SATcpSocketDelegate)
    Q_OBJECT
public:
    SATcpSocketDelegate(SATcpSocket* socket);
    ~SATcpSocketDelegate();
    //处理数据
    virtual bool deal(const SAProtocolHeader& header,const QByteArray& data) = 0;
    //写入数据
    virtual bool write(const SAProtocolHeader& header,const QByteArray& data);
    //
    SATcpSocket* socket();
    const SATcpSocket* socket() const;
public slots:
    //请求token
    virtual void requestToken(int pid,const QString& appid) = 0;
signals:
    /**
     * @brief 应答返回token
     * @param token token值
     * @param sequenceID 流水id
     * @return
     */
    Q_SIGNAL void replyToken(const QString& token,int sequenceID);
};

#endif // SATCPSECTION_PRIVATE_H
