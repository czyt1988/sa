#include "SATcpXMLSocketDelegate.h"
#include "SATcpSocket.h"
#include <QCryptographicHash>
#include "SAXMLProtocolParser.h"
#include "SAServerDefine.h"
#include "SACRC.h"




class SATcpXMLSocketDelegatePrivate
{
    SA_IMPL_PUBLIC(SATcpXMLSocketDelegate)
public:
    SATcpXMLSocketDelegatePrivate(SATcpXMLSocketDelegate* p);
    bool deal(const SAProtocolHeader &header, const SAXMLProtocolParser *xml);
    bool write(const SAXMLProtocolParser *xml, int sequenceID = 0, uint32_t extendValue = 0);
    QString makeToken(int pid,const QString& appID);
    bool requestToken(int pid, const QString &appid);
    bool dealRequestToken(const SAProtocolHeader &header,const SAXMLProtocolParser *xml);
    void dealReplyToken(const SAProtocolHeader &header,const SAXMLProtocolParser *xml);
public:
    QString m_reciveToken; ///< 记录接收到的token
};

SATcpXMLSocketDelegatePrivate::SATcpXMLSocketDelegatePrivate(SATcpXMLSocketDelegate *p)
    :q_ptr(p)
{

}

bool SATcpXMLSocketDelegatePrivate::deal(const SAProtocolHeader &header, const SAXMLProtocolParser *xml)
{
    int classid = xml->getClassID();
    int funid = xml->getFunctionID();
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

bool SATcpXMLSocketDelegatePrivate::write(const SAXMLProtocolParser *xml,int sequenceID,uint32_t extendValue)
{
    QByteArray data = xml->toByteArray();
    SAProtocolHeader header;
    header.init();
    header.dataSize = data.size();
    header.typeID = SA_PROTOCOL_TYPE_ID_XML_PROTOCOL;
    header.dataCrc32 = SACRC::crc32(data);
    header.sequenceID = sequenceID;
    header.extendValue = extendValue;
    return q_ptr->write(header,data);
}

/**
 * @brief 发送token请求
 * @param pid
 * @param appid
 * @return
 */
bool SATcpXMLSocketDelegatePrivate::requestToken(int pid, const QString &appid)
{
    RECORD_PRINT();
    //请求token
    SAXMLProtocolParser data;
    data.setClassID(SA_SERVER_CLASS_TOKEN);
    data.setFunctionID(SA_SERVER_FUN_TOKEN_REQ);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",pid);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid",appid);
    return write(&data);
}

/**
 * @brief 处理token的请求
 *
 * 此操作主要在服务端会触发，在接收到SA_SERVER_FUN_TOKEN_REQ时会调用
 * @param header
 * @param xml
 * @return
 */
bool SATcpXMLSocketDelegatePrivate::dealRequestToken(const SAProtocolHeader &header, const SAXMLProtocolParser *xml)
{
    RECORD_PRINT();
    int pid = xml->getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",0).toInt();
    QString appid = xml->getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid","").toString();
    QString token = makeToken(pid,appid);
    SAXMLProtocolParser data;
    data.setClassID(SA_SERVER_CLASS_TOKEN);
    data.setFunctionID(SA_SERVER_FUN_TOKEN_REPLY);
    data.setValue("token",token);
    return write(&data,header.sequenceID,header.extendValue);
}

void SATcpXMLSocketDelegatePrivate::dealReplyToken(const SAProtocolHeader &header, const SAXMLProtocolParser *xml)
{
    RECORD_PRINT();
    QString token = xml->getValue("token").toString();
    q_ptr->emitReplyToken(token,header.sequenceID);
}

QString SATcpXMLSocketDelegatePrivate::makeToken(int pid, const QString &appID)
{
    QByteArray pidraw;
    pidraw.setNum(pid);
    QByteArray mixcode = QCryptographicHash::hash(pidraw,QCryptographicHash::Md5)
            + QCryptographicHash::hash(appID.toUtf8(),QCryptographicHash::Sha3_256);
    QString res = QString(mixcode.toBase64()).toUtf8();
    return res;
}

SATcpXMLSocketDelegate::SATcpXMLSocketDelegate(SATcpSocket *socket):SATcpSocketDelegate(socket)
  ,d_ptr(new SATcpXMLSocketDelegatePrivate(this))
{

}

SATcpXMLSocketDelegate::~SATcpXMLSocketDelegate()
{

}

/**
 * @brief 处理数据
 * @param header
 * @param data
 * @return 返回false说明没有对此包数据进行处理
 */
bool SATcpXMLSocketDelegate::deal(const SAProtocolHeader &header, const QByteArray &data)
{
    if(!header.isValid())
    {
        return false;
    }
    if(header.typeID == SA_PROTOCOL_TYPE_ID_XML_PROTOCOL)
    {
        SAXMLProtocolParser p;
        if(p.fromByteArray(data))
        {
            return d_ptr->deal(header,&p);
        }
    }
    return false;
}

void SATcpXMLSocketDelegate::requestToken(int pid, const QString &appid)
{
    d_ptr->requestToken(pid,appid);
}

void SATcpXMLSocketDelegate::emitReplyToken(const QString &token, int sequenceID)
{
    emit replyToken(token,sequenceID);
}


