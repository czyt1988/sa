#ifndef SATCPSOCKET_H
#define SATCPSOCKET_H
#include <QTcpSocket>
#include "SAServeGlobal.h"
#include "SAProtocolHeader.h"
class SATcpSocketPrivate;
class SAAbstractSocketHandle;

/**
 * @brief 针对sa tcp协议的socket封装
 */
class SASERVE_EXPORT SATcpSocket : public QTcpSocket
{
    SA_IMPL(SATcpSocket)
    Q_OBJECT
public:
    SATcpSocket(QObject *par = nullptr);
    ~SATcpSocket();
    //从socket读数据
    bool readFromSocket(void *p, int n);

    //设置处理类,如果之前已经设置，会返回之前设置的handle的管理全不规socket管
    SAAbstractSocketHandle *setupHandle(SAAbstractSocketHandle *handle);

    //获取处理类
    SAAbstractSocketHandle *getHandle() const;

signals:

    /**
     * @brief 数据到达
     * @param header 包头
     * @param data 数据
     */
    void receivedData(const SAProtocolHeader& header, const QByteArray& data);

public slots:
    //带重试的写socket
    void ensureWrite(const QByteArray& data);

    //带重试的写socket
    void ensureWrite(const SAProtocolHeader& header, const QByteArray& data);

private slots:
    void onReadyRead();

protected:
    //读取完整的数据后调用，此类会发射receivedData信号
    virtual void deal(const SAProtocolHeader& header, const QByteArray& data);
};

#endif // SATCPSOCKET_H
