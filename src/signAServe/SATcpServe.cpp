#include "SATcpServe.h"
#include "SAServeGlobal.h"
#include <SATcpSocket.h>
#include <QList>
#include <QThread>
#include <QDateTime>
#include <QHostAddress>
#include "SAThreadPool.h"

SATcpSocket *create_default_ser_socket();

SATcpSocket *create_default_ser_socket()
{
#ifdef SA_SERVE_DEBUG_PRINT
    qDebug() << "create_default_socket";
#endif
    return (new SATcpSocket());
}


/**
 * @brief 存放客户端信息的结构体
 */
struct _client_info {
    _client_info();
    SATcpSocket *	socket;
    qintptr		socketDescriptor;
    QDateTime	createDatetime;
    QThread *	thread;
    QString		ip;
    qint16		port;
    bool operator <(const _client_info& other);
};

_client_info::_client_info() :
    socket(nullptr)
    , socketDescriptor(0)
    , createDatetime(QDateTime::currentDateTime())
    , thread(nullptr)
    , port(0)
{
}


bool _client_info::operator <(const _client_info& other)
{
    return ((int)(this->socket) < (int)(other.socket));
}


class SATcpServePrivate
{
    SA_IMPL_PUBLIC(SATcpServe)
public:
    SATcpServePrivate(SATcpServe *p);
    SATcpServe::FunPtrSocketFactory fpSocketFactory;
    QHash<SATcpSocket *, _client_info> socketToInfo;
};

SATcpServePrivate::SATcpServePrivate(SATcpServe *p) : q_ptr(p)
    , fpSocketFactory(nullptr)
{
    fpSocketFactory = create_default_ser_socket;
}


SATcpServe::SATcpServe(QObject *par) : QTcpServer(par)
    , d_ptr(new SATcpServePrivate(this))
{
}


SATcpServe::~SATcpServe()
{
    //如果服务器析构，对应的socket也断开
    QList<SATcpSocket *> sockets = getSockets();

    for (SATcpSocket *s : sockets)
    {
        //先断开之前的信号槽连接，避免重复析构
        closeSocket(s);
    }
}


/**
 * @brief 获取当前所有socket
 * @return
 * @note 由于每个socket在一个线程中，避免通过此函数来进行过多操作，会有无法预料的情况
 */
QList<SATcpSocket *> SATcpServe::getSockets() const
{
    return (d_ptr->socketToInfo.keys());
}


/**
 * @brief 关闭连接，清空内存
 * @param s
 */
void SATcpServe::closeSocket(SATcpSocket *s)
{
    //先断开之前的信号槽连接，避免重复析构
    disconnect(s, &SATcpSocket::disconnected, this, &SATcpServe::onSocketDisconnected);
    //主动断开连接
    s->disconnectFromHost();
    if ((s->state() == QAbstractSocket::UnconnectedState) ||
        s->waitForDisconnected(5000)) {
        s->deleteLater();
    }else {
        //断开异常，强制delete
        delete s;
    }
    d_ptr->socketToInfo.remove(s);
}


/**
 * @brief 注册socket工厂
 * @param fp
 */
void SATcpServe::registSocketFactory(SATcpServe::FunPtrSocketFactory fp)
{
    d_ptr->fpSocketFactory = fp;
}


void SATcpServe::incomingConnection(qintptr socketDescriptor)
{
    FUNCTION_RUN_PRINT();
#ifdef SA_SERVE_DEBUG_PRINT
    qDebug() << "incomingConnection:"<<socketDescriptor;
#endif
    if (d_ptr->socketToInfo.size() >= maxPendingConnections()) {
        //连接过多自动断开
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
#ifdef SA_SERVE_DEBUG_PRINT
        qDebug() << "too much connections:"<<maxPendingConnections()<<" ,disconnect";
#endif
        return;
    }
    _client_info clinfo;
    QThread *thread = SAThreadPool::getThread();
    while (thread == QThread::currentThread())
    {
        //找到一个和当前线程不一致的线程作为socket的线程
        thread = SAThreadPool::getThread();
    }

    FunPtrSocketFactory fp = d_ptr->fpSocketFactory;
    clinfo.socketDescriptor = socketDescriptor;
    std::unique_ptr<SATcpSocket> socket(fp());
    if (!(socket->setSocketDescriptor(socketDescriptor))) {
#ifdef SA_SERVE_DEBUG_PRINT
        qDebug() << "setSocketDescriptor err,socketDescriptor is "<<socketDescriptor;
#endif
        return;
    }
    QString ip = socket->peerAddress().toString();
    qint16 port = socket->peerPort();
    //对socket断开的处理
    connect(socket.get(), &SATcpSocket::disconnected, this, &SATcpServe::onSocketDisconnected);
    //把socket移动到线程中，不和此线程同一个
    socket->moveToThread(thread);
    //记录信息
    _client_info info;
    info.socketDescriptor = socketDescriptor;
    info.socket = socket.release(); // 此时智能指针释放控制权，注意这里一定要release，否则指针会被智能指针删除了
    info.thread = thread;
    info.ip = ip;
    info.port = port;

    d_ptr->socketToInfo[info.socket] = info;
#ifdef SA_SERVE_DEBUG_PRINT
    qDebug() << "create socket success:ip"<<ip<<",port:"<<port;
#endif
    addPendingConnection(info.socket);
    emit newConnection();
}


bool SATcpServe::hasPendingConnections() const
{
    return (QTcpServer::hasPendingConnections());
}


/**
 * @brief socket断开触发的槽
 */
void SATcpServe::onSocketDisconnected()
{
    SATcpSocket *socket = qobject_cast<SATcpSocket *>(sender());

    //连接断开，清理信息
    d_ptr->socketToInfo.remove(socket);
    //
    socket->deleteLater();
}
