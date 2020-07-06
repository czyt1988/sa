#include "SADataProcSession.h"
#include "SAServerDefine.h"
#include "SADataProcFunctions.h"
#include <QThreadPool>
#include "runnable/SADataStatisticRunable.h"

SASocketHandle *createDataProcSession(SATcpSocket *socket, QObject *p)
{
    return new SADataProcSession(socket,p);
}

SADataProcSession::SADataProcSession(SATcpSocket *socket, QObject *p) : SASocketHandle(socket,p)
{

}

SADataProcSession::~SADataProcSession()
{

}

bool SADataProcSession::deal(const SAProtocolHeader &header, const QByteArray &data)
{
    if(SASocketHandle::deal(header,data))
    {
        return true;
    }
    //对应SADataProcSection的处理
    return false;
}

bool SADataProcSession::dealXmlProtocol(const SAProtocolHeader &header, XMLDataPtr xml)
{
    if(SASocketHandle::dealXmlProtocol(header,xml))
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

/**
 * @brief 相对安全的写函数
 * @param header
 * @param data
 * @return
 */
bool SADataProcSession::safe_write(const SAProtocolHeader &header, const QByteArray &data)
{
    QMutexLocker lock(&m_writemutex);
    return SASocketHandle::write(header,data);
}




/**
 * @brief 处理二维点的请求
 * @param header 协议头
 * @param xml xml信息
 * @return
 */
bool SADataProcSession::deal2DPointsDescribe(const SAProtocolHeader &header, SASocketHandle::XMLDataPtr xml)
{
    qDebug() << QStringLiteral("开始执行deal2DPointsDescribe") << xml->toString();
    SADataStatisticRunable* runnable = new SADataStatisticRunable(shared_from_this(),header,xml);
    QThreadPool::globalInstance()->start(runnable);
    return true;
}

