#ifndef SADATACLIENT_H
#define SADATACLIENT_H
#include "../global/SAGlobals.h"
#include <QObject>

SA_IMPL_FORWARD_DECL(SADataClient)

/**
 * @brief 作为客户端，负责和服务端连接
 */
class SADataClient : public QObject
{
    SA_IMPL(SADataClient)
    Q_OBJECT
public:
    SADataClient(QObject* p=nullptr);
    //连接到服务器
    void connectToServer();
signals:
    //显示消息
    void messageInfo(const QString& info,SA::MeaasgeType messageType = SA::NormalMessage);

public slots:
    //登录成功触发
    Q_SLOT void onLoginSucceed(int tokenID,uint key);
    //心跳丢失
    Q_SLOT void onRecHeartbeat(uint key);
    //接收到xml字符
    Q_SLOT void onReceivedString(const QString& str,uint key);
    //接收到到点数组
    Q_SLOT void onReceive2DPointFs(const QVector<QPointF>& arrs,uint key);
    //连接丢失
    Q_SLOT void onLocalSocketDisconnect();
    //定时心跳检测时间到达触发槽
    Q_SLOT void onHeartbeatCheckerTimerout();
private slots:
    Q_SLOT void onErrorOccure(int errcode);
private:
    //尝试连接服务器
    void tryToConnectServer();
    //尝试启动计算进程
    void tryToStartDataProc();
};

#endif // SADATACLIENT_H
