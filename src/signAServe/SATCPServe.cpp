#include "SATCPServe.h"
#include <QTcpServer>
#include <QMap>
#include <QCryptographicHash>
#include <QThread>
#include "SATCPSection_private.h"

class SATCPServePrivate
{
public:
    SA_IMPL_PUBLIC(SATCPServe)
    SATCPServePrivate(SATCPServe* p);
    ~SATCPServePrivate();
    QTcpServer& serve();
    const QTcpServer& serve() const;
    QString makeToken(QTcpSocket* socket,int pid,const QString& appID);
    QString makeToken(int pid,const QString& appID);
    //通过socket获取token 如果没有返回-1
    QString getToken(QTcpSocket* socket) const;
    //通过token获取socket 如果没有返回nullptr
    QTcpSocket* getSocket(const QString& token) const;
    //把socket分配到对应的section
    void assignSection(const QString& token, QTcpSocket* socket);
    //创建一个多线程的section
    SATCPSection* createSection(const QString &token);
private:
    QTcpServer m_serve;
    QMap<QTcpSocket*,QString> m_socket2token;
    QMap<QString,QTcpSocket*> m_token2socket;
    QHash<QString,SATCPSection*> m_token2section;
};


SATCPServePrivate::SATCPServePrivate(SATCPServe* p):q_ptr(p)
{

}

SATCPServePrivate::~SATCPServePrivate()
{

}

QTcpServer &SATCPServePrivate::serve()
{
    return m_serve;
}

const QTcpServer &SATCPServePrivate::serve() const
{
    return m_serve;
}

/**
 * @brief 根据socket的创建token
 *
 * 如果socket已经存在，会返回原来记录的token
 * @param socket
 * @param pid 进程id
 * @param appID appid,为了防止以后有其他进程，添加appid，sa的appid为"sa"
 * @return 返回token
 */
QString SATCPServePrivate::makeToken(QTcpSocket *socket,int pid,const QString& appID)
{
    if(m_socket2token.contains(socket))
    {
        return m_socket2token[socket];
    }
    QString token = makeToken(pid,appID);
    m_socket2token[socket] = token;
    m_token2socket[token] = socket;
}

QString SATCPServePrivate::makeToken(int pid, const QString &appID)
{
    QByteArray pidraw;
    pidraw.setNum(pid);
    QByteArray mixcode = QCryptographicHash::hash(pidraw,QCryptographicHash::Md5)
            + QCryptographicHash::hash(appID,QCryptographicHash::Sha3_256);
    QString res = QString(mixcode.toBase64()).toUtf8();
    return res;
}

/**
 * @brief 通过socket获取token
 * @param socket
 * @return 如果没有返回-1
 */
QString SATCPServePrivate::getToken(QTcpSocket *socket) const
{
    return m_socket2token.value(socket);
}

/**
 * @brief 把socket分配到对应的section
 * @param tocken
 * @param socket
 */
void SATCPServePrivate::assignSection(const QString &token, QTcpSocket *socket)
{
    SATCPSection* section = m_token2section.value(token,nullptr);
    if(nullptr == section)
    {

    }
}

SATCPSection *SATCPServePrivate::createSection(const QString &token)
{
    SATCPSection* section = new SATCPSection(q_ptr);
    //记录section
    m_token2section[token] = section;
    //给section分配线程
    QThread* thread = new QThread();
    section->moveToThread(thread);
}

/**
 * @brief 通过token获取socket 如果没有返回nullptr
 * @param token
 * @return 如果没有返回nullptr
 */
QTcpSocket *SATCPServePrivate::getSocket(int token) const
{
    return m_token2socket.value(token,nullptr);
}


SATCPServe::SATCPServe(QObject *par):SAAbstractServe(par)
  ,d_ptr(new SATCPServePrivate(this))
{

}

SATCPServe::~SATCPServe()
{

}

bool SATCPServe::listen(int port, const QHostAddress &address)
{
    return d_ptr->serve().listen(address,port);
}

void SATCPServe::close()
{
    d_ptr->serve().close();
}

bool SATCPServe::hasPendingConnections() const
{
    return d_ptr->serve().hasPendingConnections();
}

QAbstractSocket *SATCPServe::nextPendingConnection()
{
    return d_ptr->serve().nextPendingConnection();
}

/**
 * @brief 获取所有的连接
 * @return
 */
QList<QAbstractSocket *> SATCPServe::getAllConnection() const
{
    return d_ptr->serve().findChildren<QAbstractSocket*>();
}

/**
 * @brief 新的连接到来
 */
void SATCPServe::onNewConnection()
{
    //连接到来
    QTcpSocket* socket = static_cast<QTcpSocket*>(nextPendingConnection());
    //
    if(nullptr == socket)
    {
        return;
    }
    //把token发送给请求者
}

void SATCPServe::_init()
{
    //本地处理
    connect(&(d_ptr->serve()),&QTcpServer::newConnection,this,&SATCPServe::onNewConnection);
    //信号转发
    connect(&(d_ptr->serve()),&QTcpServer::acceptError,this,&SATCPServe::acceptError);
}




