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

SATcpSocket *create_default_socket();

SATcpSocket *create_default_socket()
{
#ifdef SA_SERVE_DEBUG_PRINT
    qDebug() << "create_default_socket";
#endif
    return (new SATcpSocket());
}


class SATcpClientPrivate {
    SA_IMPL_PUBLIC(SATcpClient)
public:
    SATcpClientPrivate(SATcpClient *p);
    void startHeartbreakCheck();
    void stopHeartbreakCheck();

    //心跳检测的间隔，如果为0代表没有开启，最小1
    int getHeartbreakInterval() const;

    //设置心跳检测的间隔
    void setHeartbreatInterval(int ms);

public:
    int mHeartbreatMs;///< 心跳检测的频率ms 默认20s
    std::unique_ptr<SATcpSocket> m_socket;
    QDateTime m_lastRecHeartbreakDatetime;
    QDateTime m_lastRequestHeartbreakDatetime; ///< 请求心跳的时间
    std::unique_ptr<QTimer> m_timer;
    SATcpClient::FunPtrSocketFactory m_fpSocketFactory;
};

SATcpClientPrivate::SATcpClientPrivate(SATcpClient *p) : q_ptr(p)
    , mHeartbreatMs(20000)
    , m_socket(nullptr)
    , m_lastRecHeartbreakDatetime(QDateTime::currentDateTime())
    , m_lastRequestHeartbreakDatetime(QDateTime::currentDateTime())
    , m_timer(nullptr)
{
    m_fpSocketFactory = create_default_socket;
}


void SATcpClientPrivate::startHeartbreakCheck()
{
    if (nullptr == m_timer) {
        m_timer.reset(new QTimer());
        m_timer->connect(m_timer.get(), &QTimer::timeout, q_ptr, &SATcpClient::onHeartbreatCheckTimeout);
    }
    m_timer->setInterval(mHeartbreatMs);
    m_timer->start();
    m_lastRecHeartbreakDatetime = QDateTime::currentDateTime();
    q_ptr->requestHeartbreat();
}


void SATcpClientPrivate::stopHeartbreakCheck()
{
    if (m_timer) {
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
    if (m_timer) {
        return ((m_timer->interval()) / 1000);
    }
    return (0);
}


/**
 * @brief 设置心跳检测的间隔
 * @param ms 默认为20000ms，既20s
 */
void SATcpClientPrivate::setHeartbreatInterval(int ms)
{
    mHeartbreatMs = ms;
}


SATcpClient::SATcpClient(QObject *par) : QObject(par)
    , d_ptr(new SATcpClientPrivate(this))
{
}


SATcpClient::~SATcpClient()
{
}


/**
 * @brief 获取socket指针
 * @return
 */
SATcpSocket *SATcpClient::getSocket() const
{
    return (d_ptr->m_socket.get());
}


/**
 * @brief 注册socket工厂
 * @param fp
 */
void SATcpClient::registSocketFactory(SATcpClient::FunPtrSocketFactory fp)
{
    d_ptr->m_fpSocketFactory = fp;
}


/**
 * @brief 连接服务器
 * @param timeout 如果socket被打开会关闭原来的socket
 * @param isRecreateSocket 原来的socket会删除，重新创建一个socket
 * @return
 */
void SATcpClient::connectToServe(int timeout)
{
    qDebug() << tr("start connect to serve,timeout set to ") << timeout << " ms";
    SAServeShareMemory ssm;

    ssm.updateFromMem();
    if (!ssm.isReady()) {
        qDebug() << tr("share mem not ready:") << ssm.describe();
        emit clientError(SharedMemoryNotReadyError);
        return;
    }
    int port = ssm.getPort();

    if (port <= 0) {
        qDebug() << tr("get invalid serve port:") << port;
        emit clientError(SharedMemoryGetPortError);
        return;
    }
    if (d_ptr->m_socket) {
        destroySocket();
    }
    createSocket();
    d_ptr->m_socket->connectToHost(QHostAddress::LocalHost, port);
    if (d_ptr->m_socket->waitForConnected(timeout)) {
        qDebug() << tr("success connect to LocalHost:") << port;
        return;
    }
    qDebug() << tr("connect time out");
    emit clientError(ConnectTimeout);
}


void SATcpClient::disconnectFromServe()
{
    if (nullptr == getSocket()) {
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
    if (enable) {
        d_ptr->startHeartbreakCheck();
    }else {
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
void SATcpClient::requestToken(int pid, const QString& appid)
{
    getSocket()->requestToken(pid, appid);
}


/**
 * @brief 请求心跳
 */
void SATcpClient::requestHeartbreat()
{
    d_ptr->m_lastRequestHeartbreakDatetime = QDateTime::currentDateTime();
    getSocket()->requestHeartbreat();
}


void SATcpClient::onSocketConnected()
{
    //连接成功先请求一个心跳
    setHeartbreakCheck(true);
    emit connectedServe(qobject_cast<SATcpSocket *>(d_ptr->m_socket.get()));
}


void SATcpClient::close()
{
    if (d_ptr->m_socket) {
        d_ptr->m_socket->disconnectFromHost();
        if ((d_ptr->m_socket->state() == QAbstractSocket::UnconnectedState) ||
            d_ptr->m_socket->waitForDisconnected(5000)) {
            d_ptr->m_socket.reset(nullptr);
        }
    }
}


void SATcpClient::onHeartbreatCheckTimeout()
{
    int intverals = d_ptr->getHeartbreakInterval();
    int sec = d_ptr->m_lastRequestHeartbreakDatetime.secsTo(d_ptr->m_lastRecHeartbreakDatetime);

    if (abs(sec) >= abs(intverals)) {
        emit heartbreatTimeout(d_ptr->m_lastRecHeartbreakDatetime);
    }
    requestHeartbreat();
}


/**
 * @brief 接收到心跳应答
 */
void SATcpClient::onReceivedHeartbreat()
{
    d_ptr->m_lastRequestHeartbreakDatetime = QDateTime::currentDateTime();
    if (d_ptr->m_timer) {
        d_ptr->m_timer->start();
    }
}


void SATcpClient::createSocket()
{
    d_ptr->m_socket.reset(d_ptr->m_fpSocketFactory());
    connectSocket();
}


void SATcpClient::destroySocket()
{
    d_ptr->m_socket.reset();
}


void SATcpClient::connectSocket()
{
    connect(d_ptr->m_socket.get(), &QAbstractSocket::connected, this, &SATcpClient::onSocketConnected);
    connect(d_ptr->m_socket.get(), &QAbstractSocket::disconnected, this, &SATcpClient::disconnectedServe);
    connect(d_ptr->m_socket.get(), static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
        , this, &SATcpClient::socketError);
    connect(d_ptr->m_socket.get(), &QIODevice::aboutToClose, this, &SATcpClient::aboutToClose);
    connect(d_ptr->m_socket.get(), &SATcpSocket::receivedHeartbreat, this, &SATcpClient::onReceivedHeartbreat);
    connect(d_ptr->m_socket.get(), &SATcpSocket::receiveToken, this, &SATcpClient::receiveToken);
}


void SATcpClient::disconnectSocket()
{
    disconnect(d_ptr->m_socket.get(), &QAbstractSocket::connected, this, &SATcpClient::onSocketConnected);
    disconnect(d_ptr->m_socket.get(), &QAbstractSocket::disconnected, this, &SATcpClient::disconnectedServe);
    disconnect(d_ptr->m_socket.get(), static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
        , this, &SATcpClient::socketError);
    disconnect(d_ptr->m_socket.get(), &QIODevice::aboutToClose, this, &SATcpClient::aboutToClose);
    disconnect(d_ptr->m_socket.get(), &SATcpSocket::receivedHeartbreat, this, &SATcpClient::onReceivedHeartbreat);
    disconnect(d_ptr->m_socket.get(), &SATcpSocket::receiveToken, this, &SATcpClient::receiveToken);
}
