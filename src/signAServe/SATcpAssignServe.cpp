#include "SATcpAssignServe.h"
#include "SATcpServe.h"
#include "SATcpSocket.h"
#include <QMap>
#include <QCryptographicHash>
#include <QThread>

class SATCPAssignServePrivate
{
    SA_IMPL_PUBLIC(SATCPAssignServe)
public:
    SATCPAssignServePrivate(SATCPAssignServe* p);
    ~SATCPAssignServePrivate();
    SATcpServe& serve();
    const SATcpServe &serve() const;
private:
    SATcpServe m_serve;
};


SATCPAssignServePrivate::SATCPAssignServePrivate(SATCPAssignServe* p):q_ptr(p)
{
}

SATCPAssignServePrivate::~SATCPAssignServePrivate()
{

}

SATcpServe &SATCPAssignServePrivate::serve()
{
    return m_serve;
}

const SATcpServe &SATCPAssignServePrivate::serve() const
{
    return m_serve;
}


SATCPAssignServe::SATCPAssignServe(QObject *par)
    :QObject(par),SAAbstractServe()
  ,d_ptr(new SATCPAssignServePrivate(this))
{

}

SATCPAssignServe::~SATCPAssignServe()
{

}

bool SATCPAssignServe::listen(int port, const QHostAddress &address)
{
    return d_ptr->serve().listen(address,port);
}

void SATCPAssignServe::close()
{
    d_ptr->serve().close();
}


/**
 * @brief 获取所有的连接
 * @return
 */
QList<QAbstractSocket *> SATCPAssignServe::getAllConnection() const
{
    QList<QAbstractSocket *> res;
    QList<SATcpSocket*> ss = d_ptr->serve().getSockets();
    for(SATcpSocket* p : ss)
    {
        res.append(p);
    }
    return std::move(res);
}

void SATCPAssignServe::_init()
{
    //信号转发
    connect(&(d_ptr->serve()),&QTcpServer::acceptError,this,&SATCPAssignServe::acceptError);
}




