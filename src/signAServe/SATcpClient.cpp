#include "SATcpClient.h"
#include "SAServeShareMemory.h"
#include "SATcpSocket.h"
#include <QHostAddress>
#include "SATcpSocketDelegate.h"
#include "SATcpXMLSocketDelegate.h"
#include "SACRC.h"
class SATcpClientPrivate{
    SA_IMPL_PUBLIC(SATcpClient)
public:
    SATcpClientPrivate(SATcpClient* p);
    bool write(const SAProtocolHeader &header, const QByteArray &data);
    SATcpSocket m_socket;
};

SATcpClientPrivate::SATcpClientPrivate(SATcpClient *p):q_ptr(p)
{

}

bool SATcpClientPrivate::write(const SAProtocolHeader &header, const QByteArray &data)
{
    return m_socket.getDelegate()->write(header,data);
}

SATcpClient::SATcpClient(QObject* par):QObject(par)
  ,d_ptr(new SATcpClientPrivate(this))
{
    init();
}

SATcpClient::~SATcpClient()
{

}

bool SATcpClient::write(const SAProtocolHeader &header, const QByteArray &data)
{
    return d_ptr->write(header,data);
}

/**
 * @brief 处理接收数据的虚函数
 * @param header 文件头
 * @param data 数据包
 * @return 返回true代表已经处理此包
 * @note 此函数会执行，任何继承此类重写此函数都需要调用if(SATcpClient::deal(header,data)){ return true;}
 * 来判断是否需要重新写处理的函数
 */
bool SATcpClient::deal(const SAProtocolHeader &header, const QByteArray &data)
{

}

bool SATcpClient::connectToServe(int timeout)
{
    SAServeShareMemory& ssm = SAServeShareMemory::getInstance();
    if(!ssm.isValid())
    {
        return false;
    }
    int port = ssm.getPort();
    if(port <= 0)
    {
        return false;
    }
    d_ptr->m_socket.connectToHost(QHostAddress::LocalHost,port);
    if(d_ptr->m_socket.waitForConnected(timeout))
    {
        return true;
    }
    return false;
}

void SATcpClient::requestToken(int pid, const QString &appid)
{
    d_ptr->m_socket.getDelegate()->requestToken(pid,appid);
}

void SATcpClient::socketConnected()
{
    //请求token
    
}

void SATcpClient::socketDisconnected()
{

}

void SATcpClient::onReceivedData(const SAProtocolHeader &header, const QByteArray &data)
{
    deal(header,data);
}

void SATcpClient::init()
{
    connect(&(d_ptr->m_socket),&QAbstractSocket::connected,this,&SATcpClient::socketConnected);
    connect(&(d_ptr->m_socket),&QAbstractSocket::disconnected,this,&SATcpClient::socketDisconnected);
    connect(&(d_ptr->m_socket),&SATcpSocket::receivedData,this,&SATcpClient::onReceivedData);
    //转接所有delegate的信号


    SATcpSocketDelegate* delegate = d_ptr->m_socket.getDelegate();
    connect(delegate,&SATcpSocketDelegate::replyToken,this,&SATcpClient::replyToken);

}


