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


class SATcpClientPrivate{
    SA_IMPL_PUBLIC(SATcpClient)
public:
    SATcpClientPrivate(SATcpClient* p);
    void startHeartbreakCheck();
    void stopHeartbreakCheck();
    //心跳检测的间隔，如果为0代表没有开启，最小1
    int getHeartbreakInterval() const;
    //设置心跳检测的间隔
    void setHeartbreatInterval(int ms);
public:
    int mHeartbreatMs;///< 心跳检测的频率ms 默认20s
    SATcpSocket* m_socket;
    QDateTime m_lastRecHeartbreakDatetime;
    QDateTime m_lastRequestHeartbreakDatetime; ///< 请求心跳的时间
    std::unique_ptr<QTimer> m_timer;
};

SATcpClientPrivate::SATcpClientPrivate(SATcpClient *p):q_ptr(p)
  ,mHeartbreatMs(20000)
  ,m_socket(nullptr)
  ,m_lastRecHeartbreakDatetime(QDateTime::currentDateTime())
  ,m_lastRequestHeartbreakDatetime(QDateTime::currentDateTime())
  ,m_timer(nullptr)
{

}

void SATcpClientPrivate::startHeartbreakCheck()
{
    if(nullptr == m_timer)
    {
        m_timer = std::make_unique<QTimer>();
        m_timer->connect(m_timer.get(),&QTimer::timeout,q_ptr,&SATcpClient::onHeartbreatCheckTimeout);
    }
    m_timer->setInterval(mHeartbreatMs);
    m_timer->start();
    m_lastRecHeartbreakDatetime = QDateTime::currentDateTime();
    q_ptr->requestHeartbreat();
}

void SATcpClientPrivate::stopHeartbreakCheck()
{
    if(m_timer)
    {
        m_timer->stop();
        m_timer.reset(nullptr);
    }
}

/**
 * @brief 获取心跳间隔
 * @return 单位为s
 */
int SATcpClientPrivate::getHeartbreakInterval() const
{
    if(m_timer)
    {
        return (m_timer->interval()) / 1000;
    }
    return 0;
}

/**
 * @brief 设置心跳检测的间隔
 * @param ms 默认为20000ms，既20s
 */
void SATcpClientPrivate::setHeartbreatInterval(int ms)
{
    mHeartbreatMs = ms;
}



SATcpClient::SATcpClient(QObject* par):QObject(par)
  ,d_ptr(new SATcpClientPrivate(this))
{

}

SATcpClient::~SATcpClient()
{

}

/**
 * @brief 向socket安全写数据，此函数会保证数据完整写入
 * @param header
 * @param data
 * @return
 */
bool SATcpClient::write(const SAProtocolHeader &header, const QByteArray &data)
{
    if(d_ptr->m_socket)
    {
        return SA::write(header,data,getSocket());
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
    switch (header.protocolTypeID)
    {
    case SA::ProtocolTypeHeartbreat:
    {
        //收到心跳，记录最后时间
        d_ptr->m_lastRecHeartbreakDatetime = QDateTime::currentDateTime();
        return true;
    }
    case SA::ProtocolTypeXml:
    {
        //解析xml协议
        XMLDataPtr xml = std::make_shared<SAXMLProtocolParser>();
        if(!xml->fromByteArray(data))
        {
            emit clientError(InvalidXmlProtocol);
            return false;
        }
        return dealXmlProtocol(header,xml);
    }
    default:
        break;
    }
    SA::FunHandle fun = SA::get_serve_funciton_handle(header.protocolTypeID,header.protocolFunID);
    if(nullptr == fun)
    {
        return false;
    }
    return fun(header,data,getSocket(),this);
}

bool SATcpClient::dealXmlProtocol(const SAProtocolHeader &header, XMLDataPtr xml)
{
    switch (header.protocolFunID)
    {
    case SA::ProtocolFunReplyHeartbreat:
        d_ptr->m_lastRecHeartbreakDatetime = QDateTime::currentDateTime();
        return true;
    case SA::ProtocolFunReplyToken:
        {
            QString token = xml->getDefaultGroupValue("token").toString();
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
    ssm.updateFromMem();
    if(!ssm.isReady())
    {
        qDebug() << tr("share mem not ready:") << ssm.describe();
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
        qDebug() << tr("success connect to LocalHost:") << port;
        return true;
    }
    qDebug() << tr("connect time out");
    emit clientError(ConnectTimeout);
    return false;
}

void SATcpClient::disconnectFromServe()
{
    if(nullptr == getSocket())
    {
        return;
    }
    getSocket()->disconnectFromHost();
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
void SATcpClient::setHeartbreakCheck(bool enable)
{
    if(enable)
    {
        d_ptr->startHeartbreakCheck();
    }
    else
    {
        d_ptr->stopHeartbreakCheck();
    }
}

/**
 * @brief 客户端-发出token请求
 *
 * 发送协议：
 * default group:
 * "pid": int->程序pid
 * "appid": string->程序id
 *
 * @param pid
 * @param appid
 */
void SATcpClient::requestToken(int pid, const QString &appid)
{
    SA::request_token_xml(pid,appid,getSocket());
}

void SATcpClient::requestHeartbreat()
{
    d_ptr->m_lastRequestHeartbreakDatetime = QDateTime::currentDateTime();
    SA::request_heartbreat(getSocket());
}

void SATcpClient::onSocketConnected()
{
    //连接成功先请求一个心跳
    setHeartbreakCheck(true);
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
    int sec = d_ptr->m_lastRequestHeartbreakDatetime.secsTo(d_ptr->m_lastRecHeartbreakDatetime);
    if(abs(sec) >= abs(intverals))
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
    connect(d_ptr->m_socket,&QAbstractSocket::connected,this,&SATcpClient::onSocketConnected);
    connect(d_ptr->m_socket,&QAbstractSocket::disconnected,this,&SATcpClient::disconnected);
    connect(d_ptr->m_socket,static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
            ,this,&SATcpClient::socketError);
    connect(d_ptr->m_socket,&SATcpSocket::receivedData,this,&SATcpClient::onReceivedData);
    connect(d_ptr->m_socket,&QIODevice::aboutToClose,this,&SATcpClient::aboutToClose);
}

void SATcpClient::disconnectSocket()
{
    disconnect(d_ptr->m_socket,&QAbstractSocket::connected,this,&SATcpClient::connected);
    disconnect(d_ptr->m_socket,&QAbstractSocket::disconnected,this,&SATcpClient::disconnected);
    disconnect(d_ptr->m_socket,static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
            ,this,&SATcpClient::socketError);
    disconnect(d_ptr->m_socket,&SATcpSocket::receivedData,this,&SATcpClient::onReceivedData);
    disconnect(d_ptr->m_socket,&QIODevice::aboutToClose,this,&SATcpClient::aboutToClose);
}




