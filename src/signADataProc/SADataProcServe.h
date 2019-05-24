#ifndef SADATAPROCSERVE_H
#define SADATAPROCSERVE_H
#include <QObject>
#include <QLocalSocket>
#include "SALocalServeSocketServeParse.h"
#include <QSet>
#include <QTimer>

class QThread;
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
private:
    ///
    /// \brief 私有信号，用于调用数据处理线程
    ///
    Q_SIGNAL void callVectorPointFProcess(const QVector<QPointF>& points
                                          ,const QHash<QString, QVariant> &args);

    void initCalcThread();
private slots:
    Q_SLOT void onLocalServeNewConnection();
    //错误发生
    Q_SLOT void errorOccurred(QLocalSocket::LocalSocketError err);
    //接收到客户端发的点数组
    Q_SLOT void onReceive2DPointFs(const QVector<QPointF> &datas, uint key);
    //接收到客户端的文字
    Q_SLOT void onReceivedString(const QString& str,uint key);


    //接收到点数组的计算结果
    Q_SLOT void onProcessVectorPointFResult(const QString& res
                                            , const QHash<QString, QVariant>& args);

    Q_SLOT void onDisconnected();
private:
    QSet<QLocalSocket*> m_connectList;///< 连接的列表
    QMap<QLocalSocket*,SALocalServeSocketServeParse*> m_socketOptDict;
    QMap<uint,SALocalServeSocketServeParse*> m_tokenOptDict;
private:
    QThread* m_calcThread;///< 处理计算的线程
    SADataProcessVectorPointF* m_pointFCalctor;///< 点集计算处理
    uint m_pid;
    QLocalServer* m_localServer;///< 本地服务器
    bool m_willBeQuit;
};
Q_DECLARE_METATYPE(QVector<QPointF>)
#endif // SADATAPROCCLIENT_H
