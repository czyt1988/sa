#include "SATcpClient.h"
#include <QHostAddress>
#include <QCryptographicHash>
#include <QDateTime>
#include <QTimer>
#include <memory>
#include "SAServeShareMemory.h"
#include "SATcpSocket.h"
#include "SACRC.h"
#include "SAXMLProtocolParser.h"
#include "SAServerDefine.h"
#include "SAServeHandleFun.h"

const int HEARTBREAT_TIME_OUT_INTV_S = 5;

class SATcpClientPrivate{
    SA_IMPL_PUBLIC(SATcpClient)
public:
    SATcpClientPrivate(SATcpClient* p);
    void startHeartbreakCheck(int checkFreS);
    void stopHeartbreakCheck();
    //心跳检测的间隔，如果为0代表没有开启，最小1
    int getHeartbreakInterval() const;

public:
    SATcpSocket* m_socket;
    QDateTime m_lastRecHeartbreakDatetime;
    std::unique_ptr<QTimer> m_timer;
};

SATcpClientPrivate::SATcpClientPrivate(SATcpClient *p):q_ptr(p)
  ,m_socket(nullptr)
  ,m_lastRecHeartbreakDatetime(QDateTime::currentDateTime())
  ,m_timer(nullptr)
{

}

void SATcpClientPrivate::startHeartbreakCheck(int checkFreS)
{
    checkFreS *= 1000;
    if(checkFreS < 1000)
    {
        checkFreS = 1000;
    }
    if(nullptr == m_timer)
    {
        m_timer = std::make_unique<QTimer>();
        m_timer->connect(m_timer.get(),&QTimer::timeout,q_ptr,&SATcpClient::onHeartbreatCheckTimeout);
    }
    m_timer->setInterval(checkFreS);
    m_timer->start();
    m_lastRecHeartbreakDatetime = QDateTime::currentDateTime();
}

void SATcpClientPrivate::stopHeartbreakCheck()
{
    if(m_timer)
    {
        m_timer->stop();
        m_timer.reset(nullptr);
    }
}

int SATcpClientPrivate::getHeartbreakInterval() const
{
    if(m_timer)
    {
        return (m_timer->interval()) / 1000;
    }
    return 0;
}


SATcpClient::SATcpClient(QObject* par):QObject(par)
  ,d_ptr(new SATcpClientPrivate(this))
{

}

SATcpClient::~SATcpClient()
{

}

bool SATcpClient::write(const SAProtocolHeader &header, const QByteArray &data)
{
    if(d_ptr->m_socket)
    {
        return SA::write(header,data,d_ptr->m_socket);
    }
    return false;
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
    SA::FunHandle fun = SA::get_serve_funciton_handle(header.protocolFunID);
    if(nullptr == fun)
    {
        return false;
    }
    QVariantHash res;
    bool stat = fun(header,data,d_ptr->m_socket,&res);
    if(!stat)
    {
        return false;
    }
    dealResult(header,res);
    return true;
}

bool SATcpClient::dealResult(const SAProtocolHeader &header, const QVariantHash &data)
{
    switch (header.protocolFunID)
    {
    case SA::ProtocolFunReplyHeartbreat:
        d_ptr->m_lastRecHeartbreakDatetime = QDateTime::currentDateTime();
        return true;
    case SA::ProtocolFunReplyToken:
        {
            QString token = data.value("token").toString();
            if(!token.isEmpty())
            {
                emit replyToken(token,header.sequenceID);
                return true;
            }
        }
        break;
    default:
        break;
    }
    return false;
}


/**
 * @brief 获取socket指针
 * @return
 */
SATcpSocket *SATcpClient::getSocket() const
{
    return d_ptr->m_socket;
}

/**
 * @brief 连接服务器
 * @param timeout 如果socket被打开会关闭原来的socket
 * @param isRecreateSocket 原来的socket会删除，重新创建一个socket
 * @return
 */
bool SATcpClient::connectToServe(int timeout)
{
    qDebug() << tr("start connect to serve,timeout set to ") << timeout << " ms";
    SAServeShareMemory ssm;
    if(!ssm.isReady())
    {
        qDebug() << tr("share mem not ready");
        emit clientError(SharedMemoryNotReadyError);
        return false;
    }
    int port = ssm.getPort();
    if(port <= 0)
    {
        qDebug() << tr("get invalid serve port:") << port;
        emit clientError(SharedMemoryGetPortError);
        return false;
    }
    if(d_ptr->m_socket)
    {
        destroySocket();
    }
    createSocket();
    d_ptr->m_socket->connectToHost(QHostAddress::LocalHost,port);
    if(d_ptr->m_socket->waitForConnected(timeout))
    {
        return true;
    }
    qDebug() << tr("connect time out");
    emit clientError(ConnectTimeout);
    return false;
}

void SATcpClient::disconnectFromServe()
{
    if(nullptr == d_ptr->m_socket)
    {
        return;
    }
    d_ptr->m_socket->disconnectFromHost();
}

/**
 * @brief 心跳检查设置
 *
 * 如果开启，定时会向服务器发送requestHeartbreat，并处理收到的心跳回复，如果心跳在设定时间没有回复
 * 将会发射信号heartbreatTimeout
 * @param enable 是否开启心跳检测
 * @param checkFreS 检测频率，单位为秒S,默认30秒
 * @see heartbreatTimeout
 */
void SATcpClient::setHeartbreakCheck(bool enable, int checkFreS)
{
    if(enable)
    {
        d_ptr->startHeartbreakCheck(checkFreS);
    }
    else
    {
        d_ptr->stopHeartbreakCheck();
    }
}

void SATcpClient::requestToken(int pid, const QString &appid)
{
    SA::request_token_xml(pid,appid,d_ptr->m_socket);
}

void SATcpClient::requestHeartbreat()
{
    SA::request_heartbreat(d_ptr->m_socket);
}

void SATcpClient::onSocketConnected()
{
    //连接成功先请求一个心跳
    requestHeartbreat();
    emit connected();
}

void SATcpClient::cloese()
{
    if(d_ptr->m_socket)
    {
        d_ptr->m_socket->close();
    }
}

void SATcpClient::onReceivedData(const SAProtocolHeader &header, const QByteArray &data)
{
    deal(header,data);
}

void SATcpClient::onHeartbreatCheckTimeout()
{
    int intverals = d_ptr->getHeartbreakInterval();
    QDateTime cur = QDateTime::currentDateTime();
    int sec = cur.secsTo(d_ptr->m_lastRecHeartbreakDatetime);
    intverals = (abs(sec)-abs(intverals));
    if(intverals > HEARTBREAT_TIME_OUT_INTV_S)
    {
        emit heartbreatTimeout(d_ptr->m_lastRecHeartbreakDatetime);
    }
    requestHeartbreat();
}

void SATcpClient::createSocket()
{
    d_ptr->m_socket = new SATcpSocket(this);
    connectSocket();
}

void SATcpClient::destroySocket()
{
    delete d_ptr->m_socket;
    d_ptr->m_socket = nullptr;
}

void SATcpClient::connectSocket()
{
    connect(d_ptr->m_socket,&QAbstractSocket::connected,this,&SATcpClient::connected);
    connect(d_ptr->m_socket,&QAbstractSocket::disconnected,this,&SATcpClient::disconnected);
    connect(d_ptr->m_socket,static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
            ,this,&SATcpClient::error);
    connect(d_ptr->m_socket,&SATcpSocket::receivedData,this,&SATcpClient::onReceivedData);
    connect(d_ptr->m_socket,&QIODevice::aboutToClose,this,&SATcpClient::aboutToClose);
}

void SATcpClient::disconnectSocket()
{
    disconnect(d_ptr->m_socket,&QAbstractSocket::connected,this,&SATcpClient::connected);
    disconnect(d_ptr->m_socket,&QAbstractSocket::disconnected,this,&SATcpClient::disconnected);
    disconnect(d_ptr->m_socket,static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
            ,this,&SATcpClient::error);
    disconnect(d_ptr->m_socket,&SATcpSocket::receivedData,this,&SATcpClient::onReceivedData);
    disconnect(d_ptr->m_socket,&QIODevice::aboutToClose,this,&SATcpClient::aboutToClose);
}




