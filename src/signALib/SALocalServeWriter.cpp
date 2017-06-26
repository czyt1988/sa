#include "SALocalServeWriter.h"
#include <QLocalSocket>

SALocalServeWriter::SALocalServeWriter(QLocalSocket* localSocket,QObject *parent):QObject(parent)
  ,m_socket(nullptr)
{
    setSocket(localSocket);
}

SALocalServeWriter::SALocalServeWriter(QObject *parent):QObject(parent)
  ,m_socket(nullptr)
{

}

///
/// \brief 获取套接字
/// \return
///
QLocalSocket *SALocalServeWriter::getSocket() const
{
    return m_socket;
}
///
/// \brief 设置套接字
/// \param socket
///
void SALocalServeWriter::setSocket(QLocalSocket *socket)
{
    m_socket = socket;
}
///
/// \brief 发送握手协议
///
void SALocalServeWriter::sendShakeHand()
{

    if(nullptr == m_socket)
    {
        return;
    }
    if(!m_socket->isValid())
    {
        return;
    }
    if(!m_socket->isWritable())
    {
        return;
    }
    SALocalServeBaseHeader data;

    data.setKey(1);
    data.setType((int)SALocalServeBaseHeader::TypeShakeHand);

    QDataStream out(m_socket);
    out << data;
    m_socket->waitForBytesWritten();
}
