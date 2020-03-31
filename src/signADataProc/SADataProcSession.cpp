#include "SADataProcSession.h"
#include "SAServerDefine.h"
#include "SADataProcFunctions.h"
#include <QtConcurrent>
#include <QFuture>


SASession *createDataProcSession(SATcpSocket *socket, QObject *p)
{
    return new SADataProcSession(socket,p);
}

SADataProcSession::SADataProcSession(SATcpSocket *socket, QObject *p) : SASession(socket,p)
{

}

SADataProcSession::~SADataProcSession()
{

}

bool SADataProcSession::deal(const SAProtocolHeader &header, const QByteArray &data)
{
    if(SASession::deal(header,data))
    {
        return true;
    }
    //对应SADataProcSection的处理
    return false;
}

bool SADataProcSession::dealXmlProtocol(const SAProtocolHeader &header, XMLDataPtr xml)
{
    if(SASession::dealXmlProtocol(header,xml))
    {
        return true;
    }
    //对应SADataProcSection的处理
    switch (header.protocolFunID)
    {
    case SA::ProtocolFunReq2DPointsDescribe:
        return deal2DPointsDescribe(header,xml);
        break;
    default:
        break;
    }
    return false;
}

void SADataProcSession::onFinishedFuture()
{
    QFutureWatcher<SASession::XMLDataPtr>* w = qobject_cast<QFutureWatcher<SASession::XMLDataPtr>*>(sender());
    if(nullptr == w)
    {
        return;
    }
    SASession::XMLDataPtr res = w->result();
    write()
}


/**
 * @brief 处理二维点的请求
 * @param header 协议头
 * @param xml xml信息
 * @return
 */
bool SADataProcSession::deal2DPointsDescribe(const SAProtocolHeader &header, SASession::XMLDataPtr xml)
{
    QFutureWatcher<SASession::XMLDataPtr>* watcher = new QFutureWatcher<SASession::XMLDataPtr>(this);
    connect(watcher, &QFutureWatcher<SASession::XMLDataPtr>::finished, this, &SADataProcSession::onFinishedFuture);
    QFuture<SASession::XMLDataPtr> future = QtConcurrent::run(reply2DPointsDescribe,header,xml);
    watcher->setFuture(future);
    return true;
}

