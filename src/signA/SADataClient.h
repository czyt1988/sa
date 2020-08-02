#ifndef SADATACLIENT_H
#define SADATACLIENT_H
#include <QObject>
#include <QDateTime>
#include <QAbstractSocket>
#include "../global/SAGlobals.h"
#include "SAProtocolHeader.h"
#include "SATcpClient.h"
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
    SADataClient(QObject *p = nullptr);
    ~SADataClient();

signals:

    /**
     * @brief 关闭客户端
     */
    void closeClient();

    /**
     * @brief 通知消息投递
     * @param info 消息内容
     * @param messageType 消息类型
     */
    void messageInfo(const QString& info, SA::MeaasgeType messageType = SA::NormalMessage);

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

    /**
     * @brief 接收到2d点描述
     * @param header 通讯头
     * @param res 描述的协议
     */
    /**
     * @brief 返回数据点描述结果
     * @param res 结果以satree来进行描述
     */
    void receive2DPointsDescribe(double sum
        , double mean
        , double var
        , double stdVar
        , double skewness
        , double kurtosis
        , double min
        , double max
        , double mid
        , double peak2peak
        , const QPointF& minPoint
        , const QPointF& maxPoint
        , const QPointF& midPoint
        , const QVector<QPointF>& tops
        , const QVector<QPointF>& lows
        , int sequenceID
        , unsigned int extendValue);

    /**
     * @brief 请求2维数据的统计描述
     * @param arrs
     * @param key
     */
    void req2DPointsDescribe(const QVector<QPointF>& arrs, int sequenceID, int sortcount);

public slots:
    //尝试连接服务器，此函数失败会继续重连，由于失败会继续，因此会阻塞
    Q_SLOT void tryConnectToServe(int retrycount = 5, int timeout = 5000);

    //请求二维点描述，会触发@sa req2DPointsDescribe 信号
    Q_SLOT void request2DPointsDescribe(const QVector<QPointF>& arrs, int sequenceID, int sortcount = 20);

private slots:
    //连接成功槽
    Q_SLOT void onSocketConnected(QAbstractSocket *socket);

    //连接失败槽
    Q_SLOT void onSocketDisconnected();

    //定时心跳检测时间到达触发槽
    Q_SLOT void onHeartbeatCheckerTimerout(const QDateTime& lastdate);

    //错误发生
    Q_SLOT void onSocketErrorOccure(int socketError);

    //客户错误发生
    Q_SLOT void onClientErrorOccure(int clientError);

    //重新连接服务器
    Q_SLOT void reconnectToServe();

private:
    //初始化
    void _init();


private:
    SATcpClient *m_client;          ///< 客户端实例
    QThread *m_thread;
    int m_maxConnectRetryCount;     ///< 记录重试连接的最大次数
    int m_connectRetryCount;        ///< 记录重试连接的次数
    int m_timeout;                  ///< 连接服务器超时时间
};

#endif // SADATACLIENT_H
