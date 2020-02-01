#ifndef SATCPSOCKET_H
#define SATCPSOCKET_H
#include <QTcpSocket>
#include "SAServeGlobal.h"
class SATcpSocketPrivate;
/**
 * @brief 针对sa tcp协议的socket封装
 */
class SASERVE_EXPORT SATcpSocket : public QTcpSocket
{
    SA_IMPL(SATcpSocket)
public:
    SATcpSocket(QObject *par = nullptr);
    ~SATcpSocket();
    bool readFromSocket(void *p, int n);
private slots:
    void onReadyRead();
    
};

#endif // SATCPSOCKET_H
