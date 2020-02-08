#include "SATcpServe.h"
#include "SAServeGlobal.h"
#include <SATcpSocket.h>
#include <QList>
#include <QThread>
#include "SATcpSection.h"

class SATcpServePrivate
{
    SA_IMPL_PUBLIC(SATcpServe)
public:
    SATcpServePrivate(SATcpServe* p);
    void recordSection(QThread* p,SATcpSection* s);
    void removeSection(SATcpSection* s);
    QList<SATcpSocket*> m_socketList;
    QMap<SATcpSection*,QThread*> m_section2thread;

};

SATcpServePrivate::SATcpServePrivate(SATcpServe *p):q_ptr(p)
{
    
}

void SATcpServePrivate::recordSection(QThread *p, SATcpSection *s)
{
    m_section2thread[s] = p;
}

void SATcpServePrivate::removeSection(SATcpSection* s)
{
    m_section2thread.remove(s);
    m_socketList.removeOne(s->socket());
}

SATcpServe::SATcpServe(QObject *par):QTcpServer(par)
{
    
}

SATcpServe::~SATcpServe()
{
    auto e=d_ptr->m_section2thread.end();
    for(auto i=d_ptr->m_section2thread.begin();i!=e;++i)
    {
        i.value()->quit();
        i.value()->wait(3000);
    }
}

/**
 * @brief 获取当前所有socket
 * @return 
 * @note 由于每个socket在一个线程中，避免通过此函数来进行过多操作，会有无法预料的情况
 */
QList<SATcpSocket *> SATcpServe::getSockets() const
{
    return d_ptr->m_socketList;
}

void SATcpServe::incomingConnection(qintptr socketDescriptor)
{
    FUNCTION_RUN_PRINT();
    SATcpSocket *socket = new SATcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor))
    {
        return;
    }
    SATcpSection* section = new SATcpSection(socket);
    QThread* pt = new QThread();
    section->moveToThread(pt);
    connect(pt, &QThread::finished, section, &QObject::deleteLater);
    connect(pt, &QThread::finished, pt, &QObject::deleteLater);
    //断开自动结束线程
    connect(section, &SATcpSection::socketDisconnected, this, &SATcpServe::onSectionFinished);
    connect(section, &SATcpSection::socketDisconnected, pt, &QThread::quit);
    d_ptr->recordSection(pt,section);
    pt->start();
    emit newConnection();
}

bool SATcpServe::hasPendingConnections() const
{
    return QTcpServer::hasPendingConnections();
}

QTcpSocket *SATcpServe::nextPendingConnection()
{
    if(d_ptr->m_socketList.isEmpty())
        return nullptr;
    return d_ptr->m_socketList.back();
}


void SATcpServe::onSectionFinished()
{
    SATcpSection* p = qobject_cast<SATcpSection*>(sender());
    d_ptr->removeSection(p);
}


