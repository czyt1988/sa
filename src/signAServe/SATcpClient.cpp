#include "SATcpClient.h"
#include <QHostAddress>
#include "SAServeShareMemory.h"
#include "SATcpSocket.h"
#include "SATcpSocketDelegate.h"
#include "SATcpXMLSocketDelegate.h"
#include "SACRC.h"
#include "SAXMLProtocolParser.h"
#include "SAServerDefine.h"

class SATcpClientPrivate{
    SA_IMPL_PUBLIC(SATcpClient)
public:
    SATcpClientPrivate(SATcpClient* p);
    bool ensureWrite(const char *data, qint64 len, short maxtry=200);
    bool ensureWrite(const QByteArray& data);
    bool write(const SAProtocolHeader &header, const QByteArray &data);
    bool writeXml(const SAXMLProtocolParser *xml, int sequenceID = 0, uint32_t extendValue = 0);
    bool dealXmlProtocol(const SAProtocolHeader &header, const QByteArray &data);
    bool dealXmlRequestToken(const SAProtocolHeader &header,SAXMLProtocolParser& xml);
    bool dealXmlReplyToken(const SAProtocolHeader &header,SAXMLProtocolParser& xml);
    bool requestToken(int pid,const QString& appid);
    SATcpSocket* m_socket;
};

SATcpClientPrivate::SATcpClientPrivate(SATcpClient *p):q_ptr(p)
  ,m_socket(new SATcpSocket(p))
{

}

bool SATcpClientPrivate::ensureWrite(const char *data, qint64 len, short maxtry)
{
    int count = 0;
    qint64 wed = 0;
    do
    {
        wed += (m_socket->write(data+wed,len-wed));
        ++count;
    }while(wed < len && count < maxtry);
    return (wed == len);
}

bool SATcpClientPrivate::ensureWrite(const QByteArray &data)
{
    qint64 wed = m_socket->write(data);
    if(wed < data.size())
    {
        //没写完继续写
        return ensureWrite(data.data()+wed,data.size()-wed);
    }
    return true;
}

bool SATcpClientPrivate::write(const SAProtocolHeader &header, const QByteArray &data)
{
    bool stat = false;
    stat = ensureWrite((const char*)(&header),sizeof(SAProtocolHeader));
    stat &= ensureWrite(data);
    return stat;
}

bool SATcpClientPrivate::writeXml(const SAXMLProtocolParser *xml, int sequenceID, uint32_t extendValue)
{
    QByteArray data = xml->toByteArray();
    SAProtocolHeader header;
    header.init();
    header.dataSize = data.size();
    header.typeID = SA_PROTOCOL_TYPE_ID_XML_PROTOCOL;
    header.dataCrc32 = SACRC::crc32(data);
    header.sequenceID = sequenceID;
    header.extendValue = extendValue;
    return write(header,data);
}

bool SATcpClientPrivate::dealXmlProtocol(const SAProtocolHeader &header, const QByteArray &data)
{
    SAXMLProtocolParser xml;
    if(!xml.fromByteArray(data))
    {
        return false;
    }
    int classid = xml.getClassID();
    int funid = xml.getFunctionID();
    switch (classid) {
    case SA_SERVER_CLASS_TOKEN:
        switch (funid) {
        case SA_SERVER_FUN_TOKEN_REQ:
            //收到token请求，生成token并答复
            return dealRequestToken(header,xml);
            break;
        case SA_SERVER_FUN_TOKEN_REPLY:
            //收到生成的token，并发送信号
            dealReplyToken(header,xml);
            return true;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return false;
}

bool SATcpClientPrivate::dealXmlRequestToken(const SAProtocolHeader &header, SAXMLProtocolParser &xml)
{

}

bool SATcpClientPrivate::dealXmlReplyToken(const SAProtocolHeader &header, SAXMLProtocolParser &xml)
{

}

bool SATcpClientPrivate::requestToken(int pid, const QString &appid)
{
    FUNCTION_RUN_PRINT();
    //请求token
    SAXMLProtocolParser data;
    data.setClassID(SA_SERVER_CLASS_TOKEN);
    data.setFunctionID(SA_SERVER_FUN_TOKEN_REQ);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",pid);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid",appid);
    return writeXml(&data);
}

SATcpClient::SATcpClient(QObject* par):QObject(par)
  ,d_ptr(new SATcpClientPrivate(this))
{
    init();
}

SATcpClient::~SATcpClient()
{

}

bool SATcpClient::write(const SAProtocolHeader &header, const QByteArray &data)
{
    return d_ptr->write(header,data);
}

/**
 * @brief 处理接收数据的虚函数
 * @param header 文件头
 * @param data 数据包
 * @return 返回true代表已经处理此包
 * @note 此函数会执行，任何继承此类重写此函数都需要调用if(SATcpClient::deal(header,data)){ return true;}
 * 来判断是否需要重新写处理的函数
 */
bool SATcpClient::deal(const SAProtocolHeader &header, const QByteArray &data)
{
    if(!header.isValid())
    {
        return false;
    }
    if(header.typeID == SA_PROTOCOL_TYPE_ID_XML_PROTOCOL)
    {
        return d_ptr->dealXmlProtocol(header,&p);
    }
    return false;
}

bool SATcpClient::connectToServe(int timeout)
{
    SAServeShareMemory& ssm = SAServeShareMemory::getInstance();
    if(!ssm.isValid())
    {
        return false;
    }
    int port = ssm.getPort();
    if(port <= 0)
    {
        return false;
    }
    d_ptr->m_socket->connectToHost(QHostAddress::LocalHost,port);
    if(d_ptr->m_socket->waitForConnected(timeout))
    {
        return true;
    }
    return false;
}

void SATcpClient::requestToken(int pid, const QString &appid)
{
    d_ptr->requestToken(pid,appid);
}

void SATcpClient::socketConnected()
{

}

void SATcpClient::socketDisconnected()
{

}

void SATcpClient::onReceivedData(const SAProtocolHeader &header, const QByteArray &data)
{
    deal(header,data);
}

void SATcpClient::init()
{
    connect(&(d_ptr->m_socket),&QAbstractSocket::connected,this,&SATcpClient::socketConnected);
    connect(&(d_ptr->m_socket),&QAbstractSocket::disconnected,this,&SATcpClient::socketDisconnected);
    connect(&(d_ptr->m_socket),&SATcpSocket::receivedData,this,&SATcpClient::onReceivedData);
}


