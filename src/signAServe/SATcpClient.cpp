#include "SATcpClient.h"
#include <QHostAddress>
#include <QCryptographicHash>
#include <QDateTime>
#include "SAServeShareMemory.h"
#include "SATcpSocket.h"
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
    // xml协议
    bool dealXmlProtocol(const SAProtocolHeader &header, const QByteArray &data);
    bool dealXmlRequestToken(const SAProtocolHeader &header,SAXMLProtocolParser& xml);
    bool dealXmlReplyToken(const SAProtocolHeader &header,SAXMLProtocolParser& xml);
    bool requestToken(int pid,const QString& appid);
    //心跳协议
    bool dealHeartbreatProtocol(const SAProtocolHeader &header, const QByteArray &data);
    bool requestHeartbreat();
public:
    SATcpSocket* m_socket;
    QDateTime m_lastRecHeartbreakDatetime;
};

SATcpClientPrivate::SATcpClientPrivate(SATcpClient *p):q_ptr(p)
  ,m_socket(new SATcpSocket(p))
  ,m_lastRecHeartbreakDatetime(QDateTime::currentDateTime())
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
    if(data.size()>0)
    {
        stat &= ensureWrite(data);
    }
    m_socket->flush();
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
            return dealXmlRequestToken(header,xml);
            break;
        case SA_SERVER_FUN_TOKEN_REPLY:
            //收到生成的token，并发送信号
            dealXmlReplyToken(header,xml);
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

/**
 * @brief 处理token的请求
 *
 * 此操作主要在服务端会触发，在接收到SA_SERVER_FUN_TOKEN_REQ时会调用
 * @param header
 * @param xml
 * @return
 */
bool SATcpClientPrivate::dealXmlRequestToken(const SAProtocolHeader &header, SAXMLProtocolParser &xml)
{
    FUNCTION_RUN_PRINT();
    int pid = xml.getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",0).toInt();
    QString appid = xml.getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid","").toString();
    QString token = SATcpClient::makeToken(pid,appid);
    SAXMLProtocolParser data;
    data.setClassID(SA_SERVER_CLASS_TOKEN);
    data.setFunctionID(SA_SERVER_FUN_TOKEN_REPLY);
    data.setValue("token",token);
    return writeXml(&data,header.sequenceID,header.extendValue);

}

bool SATcpClientPrivate::dealXmlReplyToken(const SAProtocolHeader &header, SAXMLProtocolParser &xml)
{
    FUNCTION_RUN_PRINT();
    QString token = xml.getValue("token").toString();
    q_ptr->emitReplyToken(token,header.sequenceID);
    return true;
}

bool SATcpClientPrivate::dealHeartbreatProtocol(const SAProtocolHeader &header, const QByteArray &data)
{
    Q_UNUSED(header);
    Q_UNUSED(data);
    m_lastRecHeartbreakDatetime = QDateTime::currentDateTime();
    return true;
}

/**
 * @brief 心跳请求
 * @return
 */
bool SATcpClientPrivate::requestHeartbreat()
{
    FUNCTION_RUN_PRINT();
    SAProtocolHeader header;
    header.init();
    header.dataSize = 0;
    header.typeID = SA_PROTOCOL_TYPE_ID_HEARTBREAT;
    header.extendValue = 1; // 心跳请求值为1
    return write(header,QByteArray());
}
/**
 * @brief 发送token请求
 * @param pid
 * @param appid
 * @return
 */
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
    switch (header.typeID)
    {
    case SA_PROTOCOL_TYPE_ID_HEARTBREAT:
        return d_ptr->dealHeartbreatProtocol(header,data);
    case SA_PROTOCOL_TYPE_ID_XML_PROTOCOL:
        return d_ptr->dealXmlProtocol(header,data);
    default:
        break;
    }
    return false;
}

/**
 * @brief 根据pid和appid创建token
 * @param pid
 * @param appID
 * @return
 */
QString SATcpClient::makeToken(int pid, const QString &appID)
{
    QByteArray pidraw;
    pidraw.setNum(pid);
    QByteArray mixcode = QCryptographicHash::hash(pidraw,QCryptographicHash::Md5)
            + QCryptographicHash::hash(appID.toUtf8(),QCryptographicHash::Sha3_256);
    QString res = QString(mixcode.toBase64()).toUtf8();
    return res;
}

/**
 * @brief 获取socket指针
 * @return
 */
SATcpSocket *SATcpClient::getSocket() const
{
    return d_ptr->m_socket;
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

void SATcpClient::requestHeartbreat()
{
    d_ptr->requestHeartbreat();
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
    connect(d_ptr->m_socket,&QAbstractSocket::connected,this,&SATcpClient::socketConnected);
    connect(d_ptr->m_socket,&QAbstractSocket::disconnected,this,&SATcpClient::socketDisconnected);
    connect(d_ptr->m_socket,&SATcpSocket::receivedData,this,&SATcpClient::onReceivedData);
}

void SATcpClient::emitReplyToken(const QString &token, int sequenceID)
{
    emit replyToken(token,sequenceID);
}


