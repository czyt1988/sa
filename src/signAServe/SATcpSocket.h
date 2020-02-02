#ifndef SATCPSOCKET_H
#define SATCPSOCKET_H
#include <QTcpSocket>
#include "SAServeGlobal.h"
#include "SATcpSocketDelegate.h"
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
    //获取处理数据的代理
    SATcpSocketDelegate* getDelegate() const;
    //设置新代理，原来代理会被删除
    void setupDelegate(SATcpSocketDelegate* delegate);
    //创建delegate
    template<typename T, typename... _Args>
    SATcpSocketDelegate* makeDelegate(_Args&&... __args) const;
private slots:
    void onReadyRead();
    
};

template<typename T, typename... _Args>
SATcpSocketDelegate *SATcpSocket::makeDelegate(_Args &&... __args) const
{
    return new T(this,std::forward<_Args>(__args)...);
}

#endif // SATCPSOCKET_H
