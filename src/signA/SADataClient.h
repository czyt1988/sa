#ifndef SADATACLIENT_H
#define SADATACLIENT_H
#include <QObject>
#include <QDateTime>
#include <QAbstractSocket>
#include "../global/SAGlobals.h"
class SATcpDataProcessClient;
class QThread;

//对于不暴露的接口，不使用impl方式

/**
 * @brief 作为客户端，负责和服务端连接
 *
 * @code
 * SADataClient m_client;
 *
 * ...
 *
 *
 * @endcode
 */
class SADataClient : public QObject
{
    Q_OBJECT
public:
    SADataClient(QObject* p=nullptr);
    ~SADataClient();

signals:
    /**
     * @brief 通知消息投递
     * @param info 消息内容
     * @param messageType 消息类型
     */
    void messageInfo(const QString& info,SA::MeaasgeType messageType = SA::NormalMessage);
    /**
     * @brief 心跳超时
     * 心跳超时是根据HEART_BREAK_COUNT_AS_DISCONNECT统计的累积超时才会发出此消息，期间有任何在链接情况都会打断此触发
     */
    void heartbeatCheckerTimerout();
    /**
     * @brief 连接服务的结果，此信号是在tryToConnectToServe后触发，tryToConnectToServe连接成功后会发射此信号
     */
    void connectedServeResult(bool state);
    /**
     * @brief 连接服务器发射的信号
     * @param timeout 超时时间
     */
    void startConnectToServe(int timeout);
public slots:
    //尝试连接服务器，此函数失败会继续重连，由于失败会继续，因此会阻塞
    void tryConnectToServe(int retrycount = 5, int timeout = 5000);
private slots:
    //连接成功槽
    Q_SLOT void onSocketConnected();
    //连接失败槽
    Q_SLOT void onSocketDisconnected();
    //连接服务超时
    Q_SLOT void onConnectServeTimeout();
    //定时心跳检测时间到达触发槽
    Q_SLOT void onHeartbeatCheckerTimerout(const QDateTime& lastdate);
    //错误发生
    Q_SLOT void onErrorOccure(QAbstractSocket::SocketError socketError);
private:
    SATcpDataProcessClient* m_client;
    QThread* m_thread;
    bool m_isCanConnectServe;///< 标记是否能连接服务器
    int m_maxConnectRetryCount;///< 记录重试连接的最大次数
    int m_connectRetryCount;///< 记录重试连接的次数
};

#endif // SADATACLIENT_H
