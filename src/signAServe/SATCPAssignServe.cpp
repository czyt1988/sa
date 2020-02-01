#include "SATCPAssignServe.h"
#include "SATCPServe.h"
#include "SATcpSocket.h"
#include <QMap>
#include <QCryptographicHash>
#include <QThread>
#include "SATCPSection_private.h"

class SATCPAssignServePrivate
{
    SA_IMPL_PUBLIC(SATCPAssignServe)
public:
    SATCPAssignServePrivate(SATCPAssignServe* p);
    ~SATCPAssignServePrivate();
    SATCPServe& serve();
    const SATCPServe &serve() const;
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
    SATCPServe m_serve;
    QMap<QTcpSocket*,QString> m_socket2token;
    QMap<QString,QTcpSocket*> m_token2socket;
    QHash<QString,SATCPSection*> m_token2section;
};


SATCPAssignServePrivate::SATCPAssignServePrivate(SATCPAssignServe* p):q_ptr(p)
{

}

SATCPAssignServePrivate::~SATCPAssignServePrivate()
{

}

SATCPServe &SATCPAssignServePrivate::serve()
{
    return m_serve;
}

const SATCPServe &SATCPAssignServePrivate::serve() const
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
QString SATCPAssignServePrivate::makeToken(QTcpSocket *socket,int pid,const QString& appID)
{
    if(m_socket2token.contains(socket))
    {
        return m_socket2token[socket];
    }
    QString token = makeToken(pid,appID);
    m_socket2token[socket] = token;
    m_token2socket[token] = socket;
}

QString SATCPAssignServePrivate::makeToken(int pid, const QString &appID)
{
    QByteArray pidraw;
    pidraw.setNum(pid);
    QByteArray mixcode = QCryptographicHash::hash(pidraw,QCryptographicHash::Md5)
            + QCryptographicHash::hash(appID.toUtf8(),QCryptographicHash::Sha3_256);
    QString res = QString(mixcode.toBase64()).toUtf8();
    return res;
}

/**
 * @brief 通过socket获取token
 * @param socket
 * @return 如果没有返回-1
 */
QString SATCPAssignServePrivate::getToken(QTcpSocket *socket) const
{
    return m_socket2token.value(socket);
}

/**
 * @brief 把socket分配到对应的section
 * @param tocken
 * @param socket
 */
void SATCPAssignServePrivate::assignSection(const QString &token, QTcpSocket *socket)
{
    SATCPSection* section = m_token2section.value(token,nullptr);
    if(nullptr == section)
    {

    }
}

SATCPSection *SATCPAssignServePrivate::createSection(const QString &token)
{
    SATCPSection* section = new SATCPSection(q_ptr);
    //记录section
    m_token2section[token] = section;
    //给section分配线程
    QThread* thread = new QThread();
    section->moveToThread(thread);
    q_ptr->connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    thread->start();

}
/**
 * @brief 通过token获取socket 如果没有返回nullptr
 * @param token
 * @return 如果没有返回nullptr
 */
QTcpSocket *SATCPAssignServePrivate::getSocket(const QString& token) const
{
    return m_token2socket.value(token,nullptr);
}


SATCPAssignServe::SATCPAssignServe(QObject *par)
    :QObject(par),SAAbstractServe()
  ,d_ptr(new SATCPAssignServePrivate(this))
{

}

SATCPAssignServe::~SATCPAssignServe()
{

}

bool SATCPAssignServe::listen(int port, const QHostAddress &address)
{
    return d_ptr->serve().listen(address,port);
}

void SATCPAssignServe::close()
{
    d_ptr->serve().close();
}


/**
 * @brief 获取所有的连接
 * @return
 */
QList<QAbstractSocket *> SATCPAssignServe::getAllConnection() const
{
    return d_ptr->serve().findChildren<QAbstractSocket*>();
}

/**
 * @brief 新的连接到来
 */
void SATCPAssignServe::onNewConnection()
{

}

void SATCPAssignServe::_init()
{
    //本地处理
    connect(&(d_ptr->serve()),&QTcpServer::newConnection,this,&SATCPAssignServe::onNewConnection);
    //信号转发
    connect(&(d_ptr->serve()),&QTcpServer::acceptError,this,&SATCPAssignServe::acceptError);
}




