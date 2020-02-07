#ifndef SATCPSECTION_H
#define SATCPSECTION_H
#include "SAServeGlobal.h"
#include "SATcpSocket.h"
#include "SAServeHandleFun.h"
#include <QObject>
#include <functional>
SA_IMPL_FORWARD_DECL(SATcpSection)
/**
 * @brief 对服务器每个socket的处理封装
 */
class SASERVE_EXPORT SATcpSection : public QObject
{
    SA_IMPL(SATcpSection)
    Q_OBJECT
public:
    //socket的生命周期由section管理
    SATcpSection(SATcpSocket* socket,QObject* par = nullptr);
    ~SATcpSection();
    SATcpSocket* socket() const;
    virtual bool dealReceiveData(const SAProtocolHeader &header, const QByteArray &data);
    virtual bool dealResult(const SAProtocolHeader &header, const QVariantHash &data);
signals:
    void socketDisconnected();
private slots:
    void onReceivedSocketData(const SAProtocolHeader &header, const QByteArray &data);
    void onSocketDisconnected();
};

#endif // SATCPSECTION_H
