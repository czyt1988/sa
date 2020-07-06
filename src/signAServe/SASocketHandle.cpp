#include "SASocketHandle.h"
#include <QMap>
#include "SAServeHandleFun.h"
#include "SAServerDefine.h"
#include "SAXMLProtocolParser.h"



class SASocketHandlePrivate
{
public:
    SA_IMPL_PUBLIC(SASocketHandle)
    SASocketHandlePrivate(SASocketHandle* p);
    bool dealXmlRequestToken(const SAProtocolHeader &header,SASocketHandle::XMLDataPtr xml );
public:
    SATcpSocket* mSocket;
};

SASocketHandlePrivate::SASocketHandlePrivate(SASocketHandle *p):q_ptr(p)
  ,mSocket(nullptr)
{
}

bool SASocketHandlePrivate::dealXmlRequestToken(const SAProtocolHeader &header, SASocketHandle::XMLDataPtr xml)
{
    int pid = xml->getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",0).toInt();
    QString appid = xml->getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid","").toString();
    QString token = SA::make_token(pid,appid);
    //回复
    SAXMLProtocolParser reply;
    reply.setClassID(SA::ProtocolTypeXml);
    reply.setFunctionID(SA::ProtocolFunReplyToken);
    reply.setValue("token",token);
    return SA::write_xml_protocol(mSocket,&reply,SA::ProtocolFunReplyToken,header.sequenceID,header.extendValue);
}



SAAbstractSocketHandle::SAAbstractSocketHandle(QObject *par):QObject(par)
{

}

SAAbstractSocketHandle::~SAAbstractSocketHandle()
{

}


/**
 * @brief socket的生命周期由section管理,且socket的父对象会设置为section
 * @param socket
 * @param par
 */
SASocketHandle::XMLDataPtr SASocketHandle::makeXMLDataPtr()
{
    return std::make_shared<SAXMLProtocolParser>();
}

SASocketHandle::SASocketHandle(QObject *par):SAAbstractSocketHandle(par)
  ,d_ptr(new SASocketHandlePrivate(this))
{

}

SASocketHandle::~SASocketHandle()
{

}

/**
 * @brief 设置socket
 * @param s
 */
void SASocketHandle::setSocket(SATcpSocket *s)
{
    if(d_ptr->mSocket)
    {
        disconnect(s,&SATcpSocket::receivedData,this,&SASocketHandle::onRecSocketData);
        disconnect(s,&QAbstractSocket::disconnected, this, &SASocketHandle::onSocketDisconnected);
    }
    d_ptr->mSocket = s;
    connect(s,&SATcpSocket::receivedData,this,&SASocketHandle::onRecSocketData);
    connect(s,&QAbstractSocket::disconnected, this, &SASocketHandle::onSocketDisconnected);
}


/**
 * @brief 向socket安全写数据，此函数会保证数据完整写入
 * @param header
 * @param data
 * @return
 */
void SASocketHandle::ensureWrite(const SAProtocolHeader &header, const QByteArray &data)
{
    if(getSocket())
    {
        getSocket()->ensureWrite(header,data);
    }
}

SATcpSocket *SASocketHandle::getSocket() const
{
    return d_ptr->mSocket;
}

/**
 * @brief 处理收到的数据
 * @param header 协议头
 * @param data 数据
 * @return 如果成功处理返回true，如果处理失败返回false，继承此函数可以通过父类的返回来判断是否进行了处理
 *
 * 返回的状态不受dealResult影响
 *
 * @note 此函数默认会调用dealResult，重写时，需要注意
 */
void SASocketHandle::onRecSocketData(const SAProtocolHeader &header, const QByteArray &data)
{
    switch (header.protocolTypeID) {
    case SA::ProtocolTypeHeartbreat:
    {
        //处理心跳请求
        SAProtocolHeader replyheader;
        replyheader.init();
        replyheader.sequenceID = header.sequenceID;
        replyheader.dataSize = 0;
        replyheader.protocolTypeID = SA::ProtocolTypeHeartbreat;
        replyheader.protocolFunID = SA::ProtocolFunReplyHeartbreat;
        replyheader.extendValue = 0; // 心跳返回给客户端，此时值为0
        ensureWrite(replyheader,QByteArray());
        break;
    }
    case SA::ProtocolTypeXml:
    {
        //解析xml协议
        XMLDataPtr xml = makeXMLDataPtr();
        if(!xml->fromByteArray(data))
        {
            emit error(ErrorInvalidXmlProtocol,tr("Invalid Xml Protocol"));
            return;
        }
        dealXmlProtocol(header,xml);
        return;
    }
    default:
        break;
    }
}

/**
 * @brief 处理收到的xml协议请求
 * @param header
 * @param xml
 * @return
 */
bool SASocketHandle::dealXmlProtocol(const SAProtocolHeader &header, XMLDataPtr xml)
{
    switch (header.protocolFunID)
    {
    case SA::ProtocolFunReqToken:
        return d_ptr->dealXmlRequestToken(header,xml);
    default:
        break;
    }
    return false;
}


void SASocketHandle::onSocketDisconnected()
{
}



