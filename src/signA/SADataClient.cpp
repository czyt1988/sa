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
  ,m_maxConnectRetryCount(5)
  ,m_connectRetryCount(0)
  ,m_timeout(5000)
{
    m_client = new SATcpDataProcessClient();
    m_thread = new QThread();
    m_client->moveToThread(m_thread);
    connect(m_thread,&QThread::finished,m_thread,&QThread::deleteLater);
    connect(m_thread,&QThread::finished,m_client,&SATcpDataProcessClient::deleteLater);
    connect(m_client,&SATcpDataProcessClient::aboutToClose,m_thread,&QThread::quit);
    //非线程相关的信号槽
    connect(this,&SADataClient::startConnectToServe,m_client,&SATcpDataProcessClient::connectToServe);
    connect(m_client,&SATcpDataProcessClient::clientError,this,&SADataClient::onClientErrorOccure);
    connect(m_client,&SATcpDataProcessClient::heartbreatTimeout,this,&SADataClient::onHeartbeatCheckerTimerout);
    connect(m_client,&SATcpDataProcessClient::connected,this,&SADataClient::onSocketConnected);
    connect(m_client,&SATcpDataProcessClient::disconnected,this,&SADataClient::onSocketDisconnected);
    connect(m_client,&SATcpDataProcessClient::socketError,this,&SADataClient::onSocketErrorOccure);

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
    m_connectRetryCount = 0;
    m_maxConnectRetryCount = retrycount;
    m_timeout = timeout;
    //开始连接服务器，连接成功会触发onSocketConnected，从而触发connectedServeResult(true)
    emit startConnectToServe(timeout);
}

/**
 * @brief 请求2维数据的统计描述
 * @param arrs
 * @param key
 */
void SADataClient::request2DPointsDescribe(const QVector<QPointF> &arrs, uint key)
{
    m_client->request2DPointsDescribe(arrs,key);
}

/**
 * @brief 重试连接服务器
 */
void SADataClient::reconnectToServe()
{
    if(m_connectRetryCount >= m_maxConnectRetryCount)
    {
        qDebug()<< "connect retry " << m_connectRetryCount << " times,but can not success";
        emit connectedServeResult(false);
        return;
    }
    ++m_connectRetryCount;
    emit startConnectToServe(m_timeout);
}



/**
 * @brief 客户端连接成功
 */
void SADataClient::onSocketConnected()
{
    m_connectRetryCount = 0;
    emit connectedServeResult(true);
    emit messageInfo(tr("connect calc serve success"));
}
/**
 * @brief 客户端连接丢失
 */
void SADataClient::onSocketDisconnected()
{
    m_connectRetryCount = 0;
    emit messageInfo(tr("calc serve disconnect"),SA::WarningMessage);
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
 * @brief 客户端错误
 * @param clientError
 */
void SADataClient::onClientErrorOccure(int clientError)
{
    switch (clientError) {
    case SATcpClient::ConnectTimeout:
        emit connectedServeResult(false);
        break;
    case SATcpClient::SharedMemoryNotReadyError:
        //说明共享内存还没准备好，过1s再进行连接
        qDebug() << tr("shared memory not ready,will reconnect after 1 second");
        QTimer::singleShot(1000,this,&SADataClient::reconnectToServe);
        break;
    case SATcpClient::SharedMemoryGetPortError:
        //说明共享内存里的端口号异常
        emit messageInfo(tr("data process serve return invalid port"),SA::ErrorMessage);
        break;
    default:
        break;
    }
}



/**
 * @brief 错误发生
 * @param errcode
 */
void SADataClient::onSocketErrorOccure(int socketError)
{
    emit messageInfo(tr("data client error,code:%1").arg(socketError),SA::ErrorMessage);
}





