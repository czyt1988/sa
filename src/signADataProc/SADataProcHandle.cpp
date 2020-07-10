#include "SADataProcHandle.h"
#include "SAServerDefine.h"
#include "SADataProcFunctions.h"
#include <QThreadPool>
#include "runnable/SADataStatisticRunable.h"

SADataProcHandle::SADataProcHandle(QObject *p) : SASocketHandle(p)
{

}

SADataProcHandle::~SADataProcHandle()
{

}


bool SADataProcHandle::dealXmlProtocol(const SAProtocolHeader &header, XMLDataPtr xml)
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
 * @brief 处理二维点的请求
 * @param header 协议头
 * @param xml xml信息
 * @return
 */
bool SADataProcHandle::deal2DPointsDescribe(const SAProtocolHeader &header, SASocketHandle::XMLDataPtr xml)
{
    qDebug() << QStringLiteral("开始执行deal2DPointsDescribe") << xml->toString();
    //SADataStatisticRunable* runnable = new SADataStatisticRunable(shared_from_this(),header,xml);
    //QThreadPool::globalInstance()->start(runnable);
    return true;
}

