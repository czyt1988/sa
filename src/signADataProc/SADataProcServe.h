#ifndef SADATAPROCSERVE_H
#define SADATAPROCSERVE_H
#include <QObject>
#include <QLocalSocket>
#include <QSet>
#include <QTimer>
#include "SATree.h"
#include "SATcpServe.h"
///
/// \brief 数据处理服务
///
/// 负责数据的处理和一些比较耗时的操作，通过xml字符返回结果
///
class SADataProcServe : public SATcpServe
{
    Q_OBJECT
public:
    //idealTimeSecond 超过30秒没有连接就退出
    SADataProcServe(QObject *parent = nullptr, int idealTimeSecond = 30);
    ~SADataProcServe();
    uint getPid() const;
    void setPid(const uint& pid);

private slots:
    //检测存活周期到
    void onCkeckLiveTimeout();

private:
    uint m_pid;
    bool m_willBeQuit;
    QTimer m_liveChecker;   ///< 定时判断是否需要结束的定时器
    uint m_checkLiveTime;   ///< 检查生命的时间
};
Q_DECLARE_METATYPE(QVector<QPointF>)
#endif // SADATAPROCCLIENT_H
