#ifndef SADATAPROCSERVE_H
#define SADATAPROCSERVE_H
#include <QObject>
#include <QLocalSocket>
#include <QSet>
#include <QTimer>
#include "SATcpAssignServe.h"
#include "SAThreadProcessPool.h"
#include "SATree.h"
class QLocalServer;
class SADataProcessVectorPointF;
///
/// \brief 数据处理服务
///
/// 负责数据的处理和一些比较耗时的操作，通过xml字符返回结果
///
class SADataProcServe : public SATCPAssignServe
{
    Q_OBJECT
public:
    SADataProcServe(QObject *parent = nullptr);
    ~SADataProcServe();
    uint getPid() const;
    void setPid(const uint &pid);
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
