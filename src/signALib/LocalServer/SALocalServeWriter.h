#ifndef SALOCALSERVEWRITER_H
#define SALOCALSERVEWRITER_H

#include <QObject>
#include <QPointF>
#include "SALibGlobal.h"
#include <QVector>
#include "SALocalServeBaseHeader.h"
#include <QTimer>
class QLocalSocket;
class SALIB_EXPORT SALocalServeWriter : public QObject
{
    Q_OBJECT
public:
    SALocalServeWriter(QLocalSocket* localSocket,QObject* parent = nullptr);
    SALocalServeWriter(QObject* parent = nullptr);

    QLocalSocket *getSocket() const;
    void setSocket(QLocalSocket *socket);
    //判断是否具有可写条件
    bool isEnableToWrite() const;
    //设置token
    int getToken() const;
    void setToken(uint token);

public slots:
    //开始心跳
    Q_SLOT void startHeartbeat(int freTime=5000);
    //发送内容
    Q_SLOT void send(const SALocalServeBaseHeader& header,const QByteArray& data);
    //登录，成功会返回token
    Q_SLOT void sendLoginSucceed(uint key = 0);
    //发送握手协议
    Q_SLOT void sendShakeHand(uint key = 0);
    //发送数组
    Q_SLOT void send2DPointFs(const QVector<QPointF>& datas,uint key=0);
    //发送文字
    Q_SLOT void sendString(const QString &str, uint key = 0);
    //发送错误
    Q_SLOT void sendError(const int errCode, uint key = 0);
private slots:
    void onShakeHandeTimeout();
private:
    QLocalSocket* m_socket;
    int m_token;
    QTimer m_timer;
};

#endif // SALOCALSERVEWRITER_H
