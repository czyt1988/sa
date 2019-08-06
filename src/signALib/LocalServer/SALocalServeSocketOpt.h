#ifndef SALOCALSERVE_H
#define SALOCALSERVE_H
#include <QObject>
#include <QPointF>
#include "SALibGlobal.h"
#include <QVector>
#include "SALocalServeBaseHeader.h"
#include <QLocalSocket>
#include <QTimer>
class QLocalSocket;
class SALocalServeSocketOptPrivate;

class SALIB_EXPORT SALocalServeSocketOpt : public QObject
{
    Q_OBJECT
    SA_IMPL(SALocalServeSocketOpt)
public:
    SALocalServeSocketOpt(QLocalSocket* localSocket,QObject* parent = nullptr);
    SALocalServeSocketOpt(QObject* parent = nullptr);
    ~SALocalServeSocketOpt();
public:
    QLocalSocket *getSocket() const;
    void setSocket(QLocalSocket *socket);
    //获取token
    int getToken() const;
    //判断是否具有可写条件
    bool isEnableToWrite() const;
protected:
    //设置token - 仅用于服务端
    void setToken(uint token);
    //处理socket数据
    bool readFromSocket(void *p,int n);
    //根据类型分发协议
    virtual void deal(const SALocalServeBaseHeader& head,const QByteArray& datas);
public slots:
    //开始心跳
    Q_SLOT void startAutoHeartbeat(int freTime=5000);
    //发送内容
    Q_SLOT void send(SALocalServeBaseHeader header, const QByteArray& data);
    //登录，成功会返回token
    Q_SLOT void sendLoginSucceed(uint key = 0);
    //发送握手协议
    Q_SLOT void sendHeartbeat(uint key = 0);
    //发送数组
    Q_SLOT void send2DPointFs(const QVector<QPointF>& datas,uint key=0);
    //发送文字
    Q_SLOT void sendString(const QString &str, uint key = 0);
    //发送错误
    Q_SLOT void sendError(const int errCode, uint key = 0);
    //发送二进制文件
    Q_SLOT void sendBin(const QByteArray& bin, uint key = 0);
signals:
    /**
     * @brief 登录成功 返回登录后的tokenID
     * @param 登录分配的tokenid
     * @param 标识，默认为0
     */
    void loginSucceed(int tokenID,uint key);
    /**
     * @brief 有错误发生
     * @param info 错误描述
     */
    void errorOccure(int errcode);
    /**
     * @brief 断开连接
     */
    void disconnectFromServe();
    /**
     * @brief 接收到心跳
     */
    void recHeartbeat(uint key);
    /**
     * @brief 接收到2d字符串
     * @param 接收的字符串
     * @param 标识默认为0
     */
    void rec2DPointFs(const QVector<QPointF>& arrs,uint key);
    /**
     * @brief 接收到字符串
     * @param 接收的字符串
     * @param 标识，默认为0
     */
    void recString(const QString& str,uint key);
    /**
     * @brief 接收到二进制
     * @param bin 接收的二进制
     * @param key 标识
     */
    void recBin(const QByteArray& bin,uint key);
private:
    //处理登录1-1
    void dealLoginSucceed(const QByteArray& datas,uint key);
    //处理握手2-1
    void dealHeartbeat(uint key);
    //处理2d点序列
    void deal2DPointFs(const QByteArray& datas, uint key);
    //处理字符串
    void dealString(const QByteArray& datas, uint key);
    //处理二进制
    void dealBin(const QByteArray& datas, uint key);
private slots:
    Q_SLOT void onAutoHeartbeatTimeout();
    Q_SLOT void onReadyRead();
    Q_SLOT void onDisconnected();
    Q_SLOT void onError(QLocalSocket::LocalSocketError socketError);
};

#endif // SALOCALSERVE_H
