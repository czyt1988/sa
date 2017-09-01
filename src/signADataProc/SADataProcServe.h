#ifndef SADATAPROCSERVE_H
#define SADATAPROCSERVE_H
#include <QObject>
#include <QLocalSocket>
#include "SALocalServeReader.h"
#include "SALocalServeWriter.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include <QSet>
#include <QTimer>
class SADataProcessVectorPointF;
class SADataFeatureItem;
class QThread;
class QLocalServer;
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
                                          ,QVariant args);

    void initCalcThread();

    SALocalServeWriter* getWriter(SALocalServeReader* reader);
private slots:
    Q_SLOT void onLocalServeNewConnection();
    //错误发生
    Q_SLOT void errorOccurred(QLocalSocket::LocalSocketError err);
    //接收到客户端发的点数组
    Q_SLOT void onReceivedVectorPointFData(const SALocalServeFigureItemProcessHeader& header,QVector<QPointF>& datas);
    //接收到客户端的文字
    Q_SLOT void onReceivedString(const QString& str);
    //接收到握手协议
    Q_SLOT void onRecShakeHand();

    //接收到点数组的计算结果
    Q_SLOT void onProcessVectorPointFResult(SADataFeatureItem* result
                                            , QVariant args);

    Q_SLOT void onDisconnected();
    //
    Q_SLOT void onLifeCheckTimeOut();
private:
    QSet<QLocalSocket*> m_connectList;///< 连接的列表
    QMap<QLocalSocket*,SALocalServeWriter*> m_writerDict;
    QMap<QLocalSocket*,SALocalServeReader*> m_readerDict;
private:
    QThread* m_calcThread;///< 处理计算的线程
    SADataProcessVectorPointF* m_pointFCalctor;///< 点集计算处理
    uint m_pid;
    QLocalServer* m_localServer;///< 本地服务器
    QTimer m_liveCheck;
    bool m_willBeQuit;
};
Q_DECLARE_METATYPE(QVector<QPointF>)
#endif // SADATAPROCCLIENT_H
