#include "SADataProcServe.h"
#include "SALocalServerDefine.h"
#include "SALocalServeBaseHeader.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QLocalServer>
#include <QThread>
#include "SALocalConnection.h"

SADataProcServe::SADataProcServe(QObject *parent):QObject(parent)
  ,m_pid(0)
  ,m_localServer(new QLocalServer(this))
{
    connect(m_localServer,&QLocalServer::newConnection
            ,this,&SADataProcServe::onLocalServeNewConnection);
    if(!m_localServer->listen(SA_LOCAL_SERVER_DATA_PROC_NAME))
    {
       qDebug() << tr("listern loacl server error");
    }
    qDebug() << "start DataProc Serve";
    m_socketThread = new QThread;
    connect(m_socketThread,&QThread::finished,m_socketThread,&QThread::deleteLater);
    m_socketThread->start();
}


void SADataProcServe::onLocalServeNewConnection()
{
    QLocalSocket* socket = m_localServer->nextPendingConnection();
    if(nullptr == socket)
    {
        qDebug() << "can not exec m_localServer->nextPendingConnection();";
        return;
    }
    qDebug() << "onLocalServeNewConnection new connection";
    SALocalConnection* connection = new SALocalConnection(socket,nullptr);
    connection->moveToThread(m_socketThread);
    connect(connection,&SALocalConnection::disconnected
            ,connection,&SALocalConnection::deleteLater);

}



uint SADataProcServe::getPid() const
{
    return m_pid;
}

void SADataProcServe::setPid(const uint &pid)
{
    m_pid = pid;
}
