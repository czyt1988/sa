#ifndef SADATAPROCSERVE_H
#define SADATAPROCSERVE_H
#include <QObject>
#include <QLocalSocket>
#include "SALocalServeReader.h"
#include "SALocalServeWriter.h"
#include "SALocalServeFigureItemProcessHeader.h"
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
private slots:
    Q_SLOT void onLocalServeNewConnection();
private:
    QThread* m_socketThread;///< 处理连接的线程
    uint m_pid;
    QLocalServer* m_localServer;///< 本地服务器
};

#endif // SADATAPROCCLIENT_H
