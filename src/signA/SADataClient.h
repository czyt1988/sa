#ifndef SADATACLIENT_H
#define SADATACLIENT_H
#include "../global/SAGlobals.h"
#include <QObject>

SA_IMPL_FORWARD_DECL(SADataClient)

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
    SA_IMPL(SADataClient)
    Q_OBJECT
public:
    SADataClient(QObject* p=nullptr);
    ~SADataClient();
    //连接到服务器
    void connectToServer();
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
public slots:
    //进行点集统计
    void callSeriesPointStatistic(const QVector<QPointF>& points,int key);
private slots:
    //登录成功触发
    Q_SLOT void onLoginSucceed(int tokenID,uint key);
    //心跳丢失
    Q_SLOT void onRecHeartbeat(uint key);
    //接收到xml字符
    Q_SLOT void onReceivedString(const QString& str,uint key);
    //接收到到点数组
    Q_SLOT void onReceive2DPointFs(const QVector<QPointF>& arrs,uint key);
    //定时心跳检测时间到达触发槽
    Q_SLOT void onHeartbeatCheckerTimerout();
    //错误发生
    Q_SLOT void onErrorOccure(int errcode);
    //连接丢失
    Q_SLOT void onLocalSocketDisconnect();
private:
    //尝试连接服务器
    void tryToConnectServer();
    //尝试启动计算进程
    void tryToStartDataProc();
};

#endif // SADATACLIENT_H
