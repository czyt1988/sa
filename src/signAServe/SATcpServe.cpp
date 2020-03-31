#include "SATcpServe.h"
#include "SAServeGlobal.h"
#include <SATcpSocket.h>
#include <QList>
#include <QThread>
#include "SASession.h"

class SATcpServePrivate
{
    SA_IMPL_PUBLIC(SATcpServe)
public:
    SATcpServePrivate(SATcpServe* p);
    void recordSectionThread(SASession* s,QThread* p);
    void removeSection(SASession* s);
    QList<SATcpSocket*> m_socketList;
    QMap<SASession*,QThread*> m_section2thread;
    SATcpServe::FactroyFunPtr m_factoryFun;

};

SATcpServePrivate::SATcpServePrivate(SATcpServe *p):q_ptr(p),
    m_factoryFun(nullptr)
{
    
}

void SATcpServePrivate::recordSectionThread(SASession* s,QThread* p)
{
    m_section2thread[s] = p;
}

void SATcpServePrivate::removeSection(SASession* s)
{
    m_section2thread.remove(s);
    m_socketList.removeOne(s->getSocket());
}

SATcpServe::SATcpServe(QObject *par):QTcpServer(par)
  ,d_ptr(new SATcpServePrivate(this))
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

/**
 * @brief 注册section创建函数指针
 *
 * 如果需要使用特殊的section，调用此函数注册特殊的socket 处理 section
 * @param factoryPtr section创建函数指针
 *
 */
void SATcpServe::registerSectionFactory(SATcpServe::FactroyFunPtr factoryPtr)
{
    d_ptr->m_factoryFun = factoryPtr;
}

void SATcpServe::incomingConnection(qintptr socketDescriptor)
{
    FUNCTION_RUN_PRINT();
    qDebug() << "incomingConnection:"<<socketDescriptor;

    SATcpSocket *socket = new SATcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor))
    {
        return;
    }
    SASession* section = nullptr;
    if(d_ptr->m_factoryFun)
    {
        //存在创建线程不一致
        section = d_ptr->m_factoryFun(socket,this);
    }
    else
    {
        //没有指定就用默认section
        section = new SASession(socket);
    }
    QThread* pt = new QThread();
    section->moveToThread(pt);
    connect(pt, &QThread::finished, section, &QObject::deleteLater);
    connect(pt, &QThread::finished, pt, &QObject::deleteLater);
    //断开自动结束线程
    connect(section, &SASession::socketDisconnected, this, &SATcpServe::onSectionFinished);
    connect(section, &SASession::socketDisconnected, pt, &QThread::quit);
    d_ptr->recordSectionThread(section,pt);
    pt->start();
    addPendingConnection(socket);
    emit newConnection();
}

bool SATcpServe::hasPendingConnections() const
{
    return QTcpServer::hasPendingConnections();
}


void SATcpServe::onSectionFinished()
{
    SASession* p = qobject_cast<SASession*>(sender());
    d_ptr->removeSection(p);
}


