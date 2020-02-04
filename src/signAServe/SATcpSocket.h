#ifndef SATCPSOCKET_H
#define SATCPSOCKET_H
#include <QTcpSocket>
#include "SAServeGlobal.h"
#include "SAProtocolHeader.h"
class SATcpSocketPrivate;
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
    bool readFromSocket(void *p, int n);
protected:
    virtual void deal(const SAProtocolHeader &header, const QByteArray &data);
signals:
    /**
     * @brief 数据到达
     * @param header 包头
     * @param data 数据
     */
    void receivedData(const SAProtocolHeader &header, const QByteArray &data);
private slots:
    void onReadyRead();
    
};

#endif // SATCPSOCKET_H
