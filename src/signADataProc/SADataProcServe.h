#ifndef SADATAPROCSERVE_H
#define SADATAPROCSERVE_H
#include <QObject>
#include <QLocalSocket>
#include <QSet>
#include <QTimer>
#include "SALocalServeSocketServeParse.h"
#include "SAThreadProcessPool.h"
#include "SATree.h"
class QLocalServer;
class SADataProcessVectorPointF;
///
/// \brief 数据处理服务
///
/// 负责数据的处理和一些比较耗时的操作，通过xml字符返回结果
///
class SADataProcServe : public QObject
{
    Q_OBJECT
public:
    SADataProcServe(QObject *parent = nullptr);
    uint getPid() const;
    void setPid(const uint &pid);
private slots:
    Q_SLOT void onLocalServeNewConnection();
    //错误发生
    Q_SLOT void errorOccurred(QLocalSocket::LocalSocketError err);
    //接收到客户端发的点数组
    Q_SLOT void onReceive2DPointFs(const QVector<QPointF> &datas, uint key);
    //接收到客户端的文字
    Q_SLOT void onReceivedString(const QString& str,uint key);
    //
    Q_SLOT void onDisconnected();
    //检查是否需要结束
    Q_SLOT void onCheckLive();

private slots:
    //返回处理的结果
    void onProcessResult(const QVariant& res,uint id,int type);
    //线程结束
    void onProcessFinish(uint id);
    //线程有进度返回
    void onProcessProgress(short present,uint id);
private:
    //根据处理id获取对应提交的socket
    SALocalServeSocketServeParse* getSocketByProcessID(uint id) const;
    //把id键值删除
    void removeSocketToProcessID(uint id);
private:
    QSet<QLocalSocket*> m_connectList;///< 连接的列表
    QMap<QLocalSocket*,SALocalServeSocketServeParse*> m_socketOptDict;
    QMap<uint,SALocalServeSocketServeParse*> m_tokenOptDict;
    QMap<uint,SALocalServeSocketServeParse*> m_processID2socket;
    QMap<uint,uint> m_processIDToKeyID;///< 记录计算的id对应的keyid
private:
    SAThreadProcessPool m_process;///< 处理线程的线程池

    uint m_pid;
    QLocalServer* m_localServer;///< 本地服务器
    bool m_willBeQuit;
    QTimer m_liveChecker;///< 定时判断是否需要结束的定时器
    uint m_checkLiveTime;///< 检查生命的时间
};
Q_DECLARE_METATYPE(QVector<QPointF>)
#endif // SADATAPROCCLIENT_H
