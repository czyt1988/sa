#include "SAServeHandleFun.h"
#include "SAXMLProtocolParser.h"
#include "SAServerDefine.h"
#include <QCryptographicHash>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include "SACRC.h"


static QMap<int,SA::FunHandle> fun_handle_map = QMap<int,SA::FunHandle>();
QMutex fun_handle_mutex;

bool SA::ensure_write(const char *data, qint64 len,SATcpSocket* socket,short maxtry)
{
    int count = 0;
    qint64 wed = 0;
    do
    {
        wed += (socket->write(data+wed,len-wed));
        ++count;
    }while(wed < len && count < maxtry);
    return (wed == len);
}
bool SA::ensure_write(const QByteArray &data,SATcpSocket* socket,short maxtry)
{
    qint64 wed = socket->write(data);
    if(wed < data.size())
    {
        //没写完继续写
        return ensure_write(data.data()+wed,data.size()-wed,socket,maxtry);
    }
    return true;
}

/**
 * @brief 写socket
 * @param header 协议头
 * @param data 协议数据
 * @param socket socket
 * @return
 */
bool SA::write(const SAProtocolHeader &header, const QByteArray &data, SATcpSocket *socket)
{
    bool stat = false;
    stat = ensure_write((const char*)(&header),sizeof(SAProtocolHeader),socket);
    if(data.size()>0)
    {
        stat &= ensure_write(data,socket);
    }
    socket->flush();
    return stat;
}

/**
 * @brief 写xml协议
 * @param socket
 * @param xml
 * @param sequenceID
 * @param extendValue
 * @return
 */
bool SA::write_xml_protocol(SATcpSocket *socket, const SAXMLProtocolParser *xml, int funid, int sequenceID, uint32_t extendValue)
{
    QByteArray data = xml->toByteArray();
    SAProtocolHeader header;
    header.init();
    header.protocolFunID = funid;
    header.dataSize = data.size();
    header.protocolTypeID = SA::ProtocolTypeXml;
    header.dataCrc32 = SACRC::crc32(data);
    header.sequenceID = sequenceID;
    header.extendValue = extendValue;
    return write(header,data,socket);
}

/**
 * @brief 注册处理函数
 *
 * serve的处理将通过注册的函数获取处理的方法，如果不注册将无法处理，通过此函数同时可以修改已有的处理方法
 * @param funid 函数id
 * @param fun 函数指针
 */
void SA::register_serve_funciton_handle(int funid, SA::FunHandle fun)
{
    QMutexLocker locker(&fun_handle_mutex);
    fun_handle_map[funid] = fun;
}


/**
 * @brief 获取处理的方法
 * @param funid 功能id
 * @return 对应处理方法指针，如果没有返回nullptr
 */
SA::FunHandle SA::get_serve_funciton_handle(int funid)
{
    QMutexLocker locker(&fun_handle_mutex);
    return fun_handle_map.value(funid,nullptr);
}

/**
 * @brief 初始化服务功能处理指针
 */
void SA::init_serve_funciotn_handle()
{
    QMutexLocker locker(&fun_handle_mutex);
    fun_handle_map[SA::ProtocolFunReqToken] = &deal_xml_request_token;
    fun_handle_map[SA::ProtocolFunReplyToken] = &deal_xml_reply_token;
    fun_handle_map[SA::ProtocolFunReqHeartbreat] = &deal_request_heartbreat;
}

/**
 * @brief 根据pid和appid创建token
 * @param pid
 * @param appID
 * @return
 */
QString SA::make_token(int pid, const QString &appID)
{
    QByteArray pidraw;
    pidraw.setNum(pid);
    QByteArray mixcode = QCryptographicHash::hash(pidraw,QCryptographicHash::Md5)
            + QCryptographicHash::hash(appID.toUtf8(),QCryptographicHash::Sha3_256);
    QString res = QString(mixcode.toBase64()).toUtf8();
    return res;
}

/**
 * @brief 客户端-发出token请求
 * @param pid
 * @param appid
 * @param socket
 * @return
 */
bool SA::request_token_xml(int pid, const QString &appid, SATcpSocket *socket, int sequenceID, uint32_t extendValue)
{
    FUNCTION_RUN_PRINT();
    //请求token
    SAXMLProtocolParser data;
    data.setClassID(SA::ProtocolTypeXml);
    data.setFunctionID(SA::ProtocolFunReqToken);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",pid);
    data.setValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid",appid);
    return write_xml_protocol(socket,&data,SA::ProtocolFunReqToken,sequenceID,extendValue);
}

/**
 * @brief 处理token的请求,此函数主要服务端使用
 * @param header
 * @param data
 * @param socket
 * @param res 无结果返回
 * @return 成功执行后会回复socket的token
 */
bool SA::deal_xml_request_token(const SAProtocolHeader &header, const QByteArray &data, SATcpSocket *socket, QVariantHash *res)
{
    FUNCTION_RUN_PRINT();
    Q_UNUSED(res);
    SAXMLProtocolParser xml;
    if(!xml.fromByteArray(data))
    {
        return false;
    }
    int pid = xml.getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"pid",0).toInt();
    QString appid = xml.getValue(SA_SERVER_VALUE_GROUP_SA_DEFAULT,"appid","").toString();
    QString token = make_token(pid,appid);

    //回复
    SAXMLProtocolParser reply;
    reply.setClassID(SA::ProtocolTypeXml);
    reply.setFunctionID(SA::ProtocolFunReplyToken);
    reply.setValueInDefaultGroup("token",token);
    return write_xml_protocol(socket,&reply,SA::ProtocolFunReplyToken,header.sequenceID,header.extendValue);
}



/**
 * @brief 处理token的返回
 * @param header
 * @param data
 * @param socket
 * @param res
 * @return
 * @note 函数不对指针的是否为null做检查，请确保指针的有效性
 */
bool SA::deal_xml_reply_token(const SAProtocolHeader &header, const QByteArray &data, SATcpSocket *socket, QVariantHash *res)
{
    FUNCTION_RUN_PRINT();
    SAXMLProtocolParser xml;
    if(!xml.fromByteArray(data))
    {
        return false;
    }
    QString token = xml.getValueInDefaultGroup("token").toString();
    res->insert("token",token);
    return true;
}
/**
 * @brief 请求心跳
 * @param socket
 * @return
 */
bool SA::request_heartbreat(SATcpSocket *socket)
{
    FUNCTION_RUN_PRINT();
    SAProtocolHeader header;
    header.init();
    header.dataSize = 0;
    header.protocolTypeID = SA::ProtocolTypeHeartbreat;
    header.protocolFunID = SA::ProtocolFunReqHeartbreat;
    header.extendValue = 1; // 心跳请求值为1
    return write(header,QByteArray(),socket);
}

/**
 * @brief 处理心跳的请求
 * @param header
 * @param data
 * @param socket
 * @param res
 * @return
 */
bool SA::deal_request_heartbreat(const SAProtocolHeader &header, const QByteArray &data, SATcpSocket *socket, QVariantHash *res)
{
    FUNCTION_RUN_PRINT();
    Q_UNUSED(data);
    Q_UNUSED(res);
    SAProtocolHeader replyheader;
    replyheader.init();
    replyheader.sequenceID = header.sequenceID;
    replyheader.dataSize = 0;
    replyheader.protocolTypeID = SA::ProtocolTypeHeartbreat;
    replyheader.protocolFunID = SA::ProtocolFunReplyHeartbreat;
    replyheader.extendValue = 0; // 心跳返回给客户端，此时值为0
    return write(replyheader,QByteArray(),socket);
}


