#include "SADataProcClient.h"
#include "SALocalServerDefine.h"
#include "SALocalServeBaseHeader.h"
#include <QCoreApplication>
#include <QMessageBox>
SADataProcClient::SADataProcClient(QObject *parent):QObject(parent)
  ,m_pid(0)
{
    connect(&m_socket,&QIODevice::readyRead
            ,this,&SADataProcClient::onReadyRead);
    m_socket.connectToServer(SA_LOCAL_SERVER_DATA_PROC_NAME);
}

void SADataProcClient::shakeHand()
{
    if(!m_socket.isValid())
    {
        return;
    }
    if(!m_socket.isWritable())
    {
        return;
    }
    SALocalServeBaseHeader data;
    data.setKey(13);
    data.setType(2);
    data.setSendedPid(QCoreApplication::applicationPid());
    QDataStream out(&m_socket);
    out << data;
    m_socket.waitForBytesWritten();
}

void SADataProcClient::errorOccurred(QLocalSocket::LocalSocketError err)
{
    QMessageBox::information(nullptr,"SADataProcClient",m_socket.errorString());
}


void SADataProcClient::onReadyRead()
{
    if(!m_socket.isValid())
    {
        return;
    }
    SALocalServeBaseHeader header;
    QDataStream io(&m_socket);
    io >> header;

    QMessageBox::information(nullptr,"SADataProcClient",QString("key:%1,type:%2,pid:%3")
                             .arg(header.getKey()).arg(header.getType()).arg(header.getSendedPid())
                             );
}

uint SADataProcClient::getPid() const
{
    return m_pid;
}

void SADataProcClient::setPid(const uint &pid)
{
    m_pid = pid;
}
