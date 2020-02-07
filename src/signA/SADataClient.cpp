#include "SADataClient.h"
#include <QLocalSocket>
#include <QApplication>
#include <QThread>
#include <memory>
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QProcess>
#include <QTimer>
#include "SATcpDataProcessClient.h"
#include "SATcpSocket.h"


SADataClient::SADataClient(QObject *p):QObject(p)
  ,m_isCanConnectServe(true)
{
    m_client = new SATcpDataProcessClient();
    m_thread = new QThread();
    m_client->moveToThread(m_thread);
    connect(m_thread,&QThread::finished,m_thread,&QThread::deleteLater);
    connect(m_thread,&QThread::finished,m_client,&SATcpDataProcessClient::deleteLater);
    connect(m_client,&SATcpDataProcessClient::aboutToClose,m_thread,&QThread::quit);
    //非线程相关的信号槽
    connect(this,&SADataClient::startConnectToServe,m_client,&SATcpDataProcessClient::connectToServe);

    connect(m_client,&SATcpDataProcessClient::connectTimeout,this,&SADataClient::onConnectServeTimeout);
    connect(m_client,&SATcpDataProcessClient::connected,this,&SADataClient::onSocketConnected);
    connect(m_client,&SATcpDataProcessClient::disconnected,this,&SADataClient::onSocketDisconnected);
    connect(m_client,&SATcpDataProcessClient::error,this,&SADataClient::onErrorOccure);

    //线程启动
    m_thread->start();
}

SADataClient::~SADataClient()
{

}
/**
 * @brief 尝试连接服务器，此函数失败会继续重连，成功链接会发射信号connectServeResult
 * @param retrycount 重连次数
 * @param timeout 每次连接的超时时间
 */
void SADataClient::tryConnectToServe(int retrycount,int timeout)
{
    if(!m_isCanConnectServe)
    {
        return;
    }
    m_connectRetryCount = 0;
    m_maxConnectRetryCount = retrycount;
    //开始连接服务器，连接成功会触发onSocketConnected，从而触发connectedServeResult(true)
    emit startConnectToServe(timeout);
}

/**
 * @brief 客户端连接成功
 */
void SADataClient::onSocketConnected()
{
    m_isCanConnectServe = true;
    m_connectRetryCount = 0;
    emit connectedServeResult(true);
    emit messageInfo(tr("connect calc serve success"));
}
/**
 * @brief 客户端连接丢失
 */
void SADataClient::onSocketDisconnected()
{
    m_isCanConnectServe = true;
    m_connectRetryCount = 0;
    emit messageInfo(tr("calc serve disconnect"),SA::WarningMessage);
}

/**
 * @brief 连接服务超时
 */
void SADataClient::onConnectServeTimeout()
{
    emit connectedServeResult(false);
}

/**
 * @brief 心跳超时
 * @param lastdate 最后一次接收到心跳数据的时间
 */
void SADataClient::onHeartbeatCheckerTimerout(const QDateTime &lastdate)
{
    Q_UNUSED(lastdate);
    emit heartbeatCheckerTimerout();
}

/**
 * @brief 错误发生
 * @param errcode
 */
void SADataClient::onErrorOccure(QAbstractSocket::SocketError socketError)
{
    emit messageInfo(tr("data client error,code:%1").arg(socketError),SA::ErrorMessage);
}





