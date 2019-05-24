#ifndef SALOCALSERVEPROTOCOL_H
#define SALOCALSERVEPROTOCOL_H
#include <QDataStream>
#include "SALibGlobal.h"
#include "SALocalServeBaseHeader.h"
#include <QVector>
#include <QPointF>
#include "SALocalServeReader.h"
#include <QTimer>
#include <QElapsedTimer>

/**
 * @brief 用于解析sa local serve的协议
 */

class SALIB_EXPORT SALocalServeProtocol : public SALocalServeReader
{
    Q_OBJECT
public:
    SALocalServeProtocol(QLocalSocket* localSocket,QObject* parent = nullptr);
    SALocalServeProtocol(QObject* parent = nullptr);
    int getMaxHeartbeatSpanTime() const;
    void setMaxHeartbeatSpanTime(int maxShakeHandSpanTime);
    //握手丢失最大间隔次数，达到此次数会触发shakeHandTimeOut
    int getMaxHeartbeatMissTimes() const;
    void setMaxHeartbeatMissTimes(int maxShakeHandMissTimes);

    int getHeartbeatCheckTime() const;
    void setHeartbeatCheckTime(int shakeHandCheckTime);
public slots:
    //停止握手检查，在接收到shakeHandTimeOut后应该调用此函数停止握手检查，否则会一直触发此信号
    Q_SLOT void stopHeartbeatCheck();
private slots:
    //处理接收的数据
    void onReceive(const SALocalServeBaseHeader& head,const QByteArray& datas);
    //
    void onHeartbeatCheckTimeOut();
private:
    //处理登录1-1
    void dealLoginSucceed(const QByteArray& datas,uint key);
    //处理握手2-1
    void dealHeartbeat(uint key);
    //停止心跳
    void stopHeartbeatStatusAndTimer();
    //处理2d点序列
    void deal2DPointFs(const QByteArray& datas, uint key);
    //处理字符串
    void dealString(const QByteArray& datas, uint key);
signals:
    /**
     * @brief 登录成功 返回登录后的tokenID
     * @param 登录分配的tokenid
     * @param 标识，默认为0
     */
    void loginSucceed(int tokenID,uint key);
    /**
     * @brief 握手超时
     * @param 超时的次数
     * @param 标识，默认为0
     */
    void heartbeatTimeOut(int misstimes,int tokenID,uint key);
    /**
     * @brief 接收到2d字符串
     * @param 接收的字符串
     * @param 标识默认为0
     */
    void receive2DPointFs(const QVector<QPointF>& arrs,uint key);
    /**
     * @brief 接收到字符串
     * @param 接收的字符串
     * @param 标识，默认为0
     */
    void receiveString(const QString& str,uint key);

private:
    int m_heartbeatCheckTime;///< 握手检查间隔
    int m_maxHeartbeatSpanTime;///< 每次握手最大间隔时间
    int m_maxHeartbeatMissTimes;///< 握手丢失最大间隔次数，达到此次数会触发shakeHandTimeOut
    int m_missHeartbeatCount;///< 记录握手失败次数
    QElapsedTimer m_heartbeatTick;
    QTimer m_timer;
    bool m_haveRecHeartbeat;///< 首次握手标记
    uint m_heartBreakKey;///< 心跳的key值
    int m_tokenID;///< 记录token
};


#endif // SALOCALSERVEBASEPROTOCOL_H
