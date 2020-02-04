#include "SATcpSocketDelegate.h"
#include <QMutexLocker>
#include <QDebug>
#include "SATcpSocket.h"
class SATcpSocketDelegatePrivate
{
    SA_IMPL_PUBLIC(SATcpSocketDelegate)
public:
    SATcpSocketDelegatePrivate(SATcpSocketDelegate* p,SATcpSocket* s);
    bool ensureWrite(const char *data, qint64 len, short maxtry=200);
    bool ensureWrite(const QByteArray& data);
    SATcpSocket* m_socket;
};

SATcpSocketDelegatePrivate::SATcpSocketDelegatePrivate(SATcpSocketDelegate *p, SATcpSocket *s)
    :q_ptr(p)
    ,m_socket(s)
{

}


bool SATcpSocketDelegatePrivate::ensureWrite(const char *data, qint64 len,short maxtry)
{
    int count = 0;
    qint64 wed = 0;
    do
    {
        wed += (m_socket->write(data+wed,len-wed));
        ++count;
    }while(wed < len && count < maxtry);
    return (wed == len);
}

bool SATcpSocketDelegatePrivate::ensureWrite(const QByteArray &data)
{
    qint64 wed = m_socket->write(data);
    if(wed < data.size())
    {
        //没写完继续写
        return ensureWrite(data.data()+wed,data.size()-wed);
    }
    return true;
}

SATcpSocketDelegate::SATcpSocketDelegate(SATcpSocket *socket)
    :QObject(qobject_cast<QObject*>(socket))
  ,d_ptr(new SATcpSocketDelegatePrivate(this,socket))
{
}

SATcpSocketDelegate::~SATcpSocketDelegate()
{

}

bool SATcpSocketDelegate::write(const SAProtocolHeader &header, const QByteArray &data)
{
    bool stat = false;
    stat = d_ptr->ensureWrite((const char*)(&header),sizeof(SAProtocolHeader));
    stat &= (d_ptr->ensureWrite(data));
    return stat;
}

SATcpSocket *SATcpSocketDelegate::socket()
{
    return d_ptr->m_socket;
}

const SATcpSocket *SATcpSocketDelegate::socket() const
{
    return d_ptr->m_socket;
}




