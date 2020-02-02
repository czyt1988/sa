#include "SATcpServe.h"
#include <SATcpSocket.h>
#include <QList>
#include <QThread>

class SATCPServePrivate
{
    SA_IMPL_PUBLIC(SATCPServe)
public:
    SATCPServePrivate(SATCPServe* p);
    void recordSocket(QThread* p,SATcpSocket* s);
    void removeSocketByThread(QThread* p);
    QList<SATcpSocket*> m_socketList;
    QMap<QThread*,SATcpSocket*> m_thread2socket;
};

SATCPServePrivate::SATCPServePrivate(SATCPServe *p):q_ptr(p)
{
    
}

void SATCPServePrivate::recordSocket(QThread *p, SATcpSocket *s)
{
    m_thread2socket[p] = s; 
}

void SATCPServePrivate::removeSocketByThread(QThread *p)
{
    SATcpSocket* s = m_thread2socket.value(p,nullptr);
    m_thread2socket.remove(p);
    m_socketList.removeOne(s);
}

SATCPServe::SATCPServe(QObject *par):QTcpServer(par)
{
    
}

SATCPServe::~SATCPServe()
{
    
}

/**
 * @brief 获取当前所有socket
 * @return 
 * @note 由于每个socket在一个线程中，避免通过此函数来进行过多操作，会有无法预料的情况
 */
QList<SATcpSocket *> SATCPServe::getSockets() const
{
    return d_ptr->m_socketList;
}

void SATCPServe::incomingConnection(qintptr socketDescriptor)
{
    SATcpSocket *socket = new SATcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor))
    {
        return;
    }
    QThread* pt = new QThread();
    d_ptr->recordSocket(pt,socket);
    socket->moveToThread(pt);
    connect(pt, &QThread::finished, this, &SATCPServe::onThreadFinished);
    connect(pt, &QThread::finished, socket, &QObject::deleteLater);
    connect(pt, &QThread::finished, pt, &QObject::deleteLater);
    //断开自动结束线程
    connect(socket, &QAbstractSocket::disconnected, pt, &QThread::quit);
    pt->start();
    emit newConnection();
}

bool SATCPServe::hasPendingConnections() const
{
    return QTcpServer::hasPendingConnections();
}

QTcpSocket *SATCPServe::nextPendingConnection()
{
    if(d_ptr->m_socketList.isEmpty())
        return nullptr;
    return d_ptr->m_socketList.back();
}

/**
 * @brief 处理线程结束触发的槽，用于清理变量
 */
void SATCPServe::onThreadFinished()
{
    QThread* p = qobject_cast<QThread*>(sender());
    d_ptr->removeSocketByThread(p);
}


