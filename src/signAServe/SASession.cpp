#include "SASession.h"
#include <QMap>
#include "SAServeHandleFun.h"
#include "SAServerDefine.h"
#include "SAXMLProtocolParser.h"

class SASessionPrivate
{
public:
    SA_IMPL_PUBLIC(SASession)
    SASessionPrivate(SASession* p,SATcpSocket *socket);
    bool dealXmlRequestToken(const SAProtocolHeader &header,SASession::XMLDataPtr xml );
public:
    SATcpSocket* m_socket;
};

SASessionPrivate::SASessionPrivate(SASession *p, SATcpSocket *socket):q_ptr(p)
  ,m_socket(socket)
{
    if (p != socket->parent())
    {
        socket->setParent(p);
    }
}

bool SASessionPrivate::dealXmlRequestToken(const SAProtocolHeader &header, SASession::XMLDataPtr xml)
{
    int pid = xml->getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",0).toInt();
    QString appid = xml->getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid","").toString();
    QString token = SA::make_token(pid,appid);
    //回复
    SAXMLProtocolParser reply;
    reply.setClassID(SA::ProtocolTypeXml);
    reply.setFunctionID(SA::ProtocolFunReplyToken);
    reply.setValue("token",token);
    return SA::write_xml_protocol(m_socket,&reply,SA::ProtocolFunReplyToken,header.sequenceID,header.extendValue);
}

/**
 * @brief socket的生命周期由section管理,且socket的父对象会设置为section
 * @param socket
 * @param par
 */
SASession::XMLDataPtr SASession::makeXMLDataPtr()
{
    return std::make_shared<SAXMLProtocolParser>();
}

SASession::SASession(SATcpSocket *socket, QObject *par):QObject(par)
  ,d_ptr(new SASessionPrivate(this,socket))
{
    connect(socket,&SATcpSocket::receivedData,this,&SASession::onReceivedSocketData);
    connect(socket,&QAbstractSocket::disconnected, this, &SASession::onSocketDisconnected);
}

SASession::~SASession()
{

}


/**
 * @brief 向socket安全写数据，此函数会保证数据完整写入
 * @param header
 * @param data
 * @return
 */
bool SASession::write(const SAProtocolHeader &header, const QByteArray &data)
{
    if(getSocket())
    {
        return SA::write(header,data,getSocket());
    }
    return false;
}

SATcpSocket *SASession::getSocket() const
{
    return d_ptr->m_socket;
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
bool SASession::deal(const SAProtocolHeader &header, const QByteArray &data)
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
        return write(replyheader,QByteArray());
    }
    case SA::ProtocolTypeXml:
    {
        //解析xml协议
        XMLDataPtr xml = makeXMLDataPtr();
        if(!xml->fromByteArray(data))
        {
            emit sectionError(InvalidXmlProtocol,tr("Invalid Xml Protocol"));
            return false;
        }
        return dealXmlProtocol(header,xml);
    }
    default:
        break;
    }
    return false;
}

/**
 * @brief 处理收到的xml协议请求
 * @param header
 * @param xml
 * @return
 */
bool SASession::dealXmlProtocol(const SAProtocolHeader &header, XMLDataPtr xml)
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


void SASession::onReceivedSocketData(const SAProtocolHeader &header, const QByteArray &data)
{
    //生产者
    deal(header,data);
}

void SASession::onSocketDisconnected()
{
    emit socketDisconnected();
}


