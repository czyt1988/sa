#include "SATcpSocket.h"
#include "SAProtocolHeader.h"
#include "SAXMLProtocolParser.h"
#include "SAServeHandleFun.h"
#include "SAServerDefine.h"
#include "SACRC.h"

class SATcpSocketPrivate
{
    SA_IMPL_PUBLIC(SATcpSocket)
public:
    SATcpSocketPrivate(SATcpSocket *p);
    void resetBuffer();
    void mallocBuffer(size_t size);

    SAProtocolHeader m_mainHeader;
    bool m_isReadedMainHeader;
    uint m_dataSize;
    uint m_index;
    QByteArray m_buffer;
    SAAbstractSocketHandle *m_handle;
};

SATcpSocketPrivate::SATcpSocketPrivate(SATcpSocket *p) : q_ptr(p)
    , m_isReadedMainHeader(false)
    , m_dataSize(0)
    , m_index(0)
    , m_handle(nullptr)
{
}


void SATcpSocketPrivate::resetBuffer()
{
    m_isReadedMainHeader = false;
    m_dataSize = 0;
    m_index = 0;
}


void SATcpSocketPrivate::mallocBuffer(size_t size)
{
    m_buffer.resize(size);
    m_index = 0;
    m_dataSize = size;
}


SATcpSocket::SATcpSocket(QObject *par) : QTcpSocket(par)
    , d_ptr(new SATcpSocketPrivate(this))
{
    connect(this, &QIODevice::readyRead, this, &SATcpSocket::onReadyRead);
}


SATcpSocket::~SATcpSocket()
{
}


/**
 * @brief 从socket中安全的读取文件
 * @param p 指针
 * @param n 读取的长度
 * @return
 */
bool SATcpSocket::readFromSocket(void *p, int n)
{
    int readLen = 0, index = 0;
    bool ret = false;

    do
    {
        readLen = read((char *)p+index, n);
        if (readLen <= 0) {
            break;
        }
        index += readLen;
        n -= readLen;
    }while(readLen && n);
    if (0 == n) {
        ret = true;
    }
    return (ret);
}


/**
 * @brief 带重试的写socket
 * @param data
 */
void SATcpSocket::ensureWrite(const QByteArray& data)
{
    SA::ensure_write(data, this);
}


/**
 * @brief 带重试的写socket
 * @param header
 * @param data
 */
void SATcpSocket::ensureWrite(const SAProtocolHeader& header, const QByteArray& data)
{
    SA::write(header, data, this);
}


/**
 * @brief 写xml数据
 * @param xml
 * @param funid
 * @param sequenceID
 * @param extendValue
 */
void SATcpSocket::ensureWrite(const SAXMLProtocolParser& xml, int funid, int sequenceID, uint32_t extendValue)
{
    QByteArray data = xml.toByteArray();
    SAProtocolHeader header;

    header.init();
    header.protocolFunID = funid;
    header.dataSize = data.size();
    header.protocolTypeID = SA::ProtocolTypeXml;
    header.dataCrc32 = SACRC::crc32(data);
    header.sequenceID = sequenceID;
    header.extendValue = extendValue;
    ensureWrite(header, data);
}


/**
 * @brief 请求心跳
 */
void SATcpSocket::requestHeartbreat()
{
    SA::request_heartbreat(this);
}


/**
 * @brief 发出token请求
 *
 * 发送协议：
 * default group:
 * "pid": int->程序pid
 * "appid": string->程序id
 *
 * @param pid
 * @param appid
 */
void SATcpSocket::requestToken(int pid, const QString& appid)
{
    SA::request_token_xml(pid, appid, this);
}


/**
 * @brief 回复错误给对应端
 * @param sequenceID
 * @param extendValue
 * @param msg
 * @param errcode
 */
void SATcpSocket::replyError(int sequenceID, int extendValue, const QString& msg, int errcode)
{
    SA::reply_error_xml(this, sequenceID, extendValue, msg, errcode);
}


/**
 * @brief 回复错误给对应端
 * @param msg
 * @param errcode
 */
void SATcpSocket::replyError(const SAProtocolHeader& requestHeader, const QString& msg, int errcode)
{
    SA::reply_error_xml(this, requestHeader, msg, errcode);
}


/**
 * @brief readyRead对应的槽函数，处理文件头和数据
 */
void SATcpSocket::onReadyRead()
{
    const static unsigned int s_headerSize = sizeof(SAProtocolHeader);

    qDebug() << __LINE__;
    if (d_ptr->m_isReadedMainHeader) {
        //此时已经读取了头
        if (bytesAvailable() >= d_ptr->m_mainHeader.dataSize) {
            //说明数据接收完
#ifdef SA_SERVE_DEBUG_PRINT
            qDebug()	<< " rec Data:"<<bytesAvailable()<< " bytes "
                    <<"\n header:"<<d_ptr->m_mainHeader
            ;
#endif
            if ((uint32_t)d_ptr->m_buffer.size() < d_ptr->m_mainHeader.dataSize) {
                d_ptr->m_buffer.resize(d_ptr->m_mainHeader.dataSize);
            }
            if (!readFromSocket(d_ptr->m_buffer.data()+d_ptr->m_index, d_ptr->m_mainHeader.dataSize-d_ptr->m_index)) {
                d_ptr->resetBuffer();
                abort();
                qDebug() << "socket abort!!!  can not read from socket io!";
                return;
            }
#ifdef SA_SERVE_DEBUG_PRINT
            printQByteArray(d_ptr->m_buffer);
#endif
            deal(d_ptr->m_mainHeader, d_ptr->m_buffer);
            d_ptr->resetBuffer();
            if (bytesAvailable() >= s_headerSize) {
                onReadyRead();
            }
        }else {
            //说明一次没把数据接收完
        }
    }else if (bytesAvailable() >= s_headerSize) {
        //说明包头还未接收
        //但socket收到的数据已经满足包头数据需要的数据
#ifdef SA_SERVE_DEBUG_PRINT
        qDebug() <<"main header may receive:byte available:"<<bytesAvailable();
#endif
        if (!readFromSocket((void *)(&(d_ptr->m_mainHeader)), s_headerSize)) {
            qDebug() << "can not read from socket" << __LINE__;
            d_ptr->m_isReadedMainHeader = false;
            abort();
            return;
        }
#ifdef SA_SERVE_DEBUG_PRINT
        qDebug()	<< "readed header from socket"
                << ",type:" << d_ptr->m_mainHeader.protocolTypeID
                << ",fun:" << d_ptr->m_mainHeader.protocolFunID
                << ",size:" << d_ptr->m_mainHeader.dataSize
                << ",sequenceID:" << d_ptr->m_mainHeader.sequenceID
        ;
#endif
        d_ptr->m_index = 0; // 不需要mallocBuffer会设置此值
        if (!(d_ptr->m_mainHeader.isValid())) {
            d_ptr->resetBuffer();
            qDebug()	<< "receive unknow header[1]"
                    << "\n" << d_ptr->m_mainHeader;
            abort();
            return;
        }

        if (d_ptr->m_mainHeader.dataSize > 0) {
            //说明文件头之后有数据
            d_ptr->m_isReadedMainHeader = true;
            //分配好内存
            d_ptr->mallocBuffer(d_ptr->m_mainHeader.dataSize);
        }else {
            //说明文件头之后无数据
            deal(d_ptr->m_mainHeader, QByteArray());
            d_ptr->resetBuffer();
            d_ptr->m_isReadedMainHeader = false;
        }

        if (bytesAvailable() > 0) {
#ifdef SA_SERVE_DEBUG_PRINT
            qDebug() << "have avaliable data,size:"<<bytesAvailable()
            ;
#endif
            onReadyRead();
        }
    }
}


/**
 * @brief 读取完整的数据后调用，此类会发射 @sa receivedData 信号
 * @param header
 * @param data
 * @return 返回false代表数据没有处理，返回true代表数据已经被处理
 */
bool SATcpSocket::deal(const SAProtocolHeader& header, const QByteArray& data)
{
#ifdef SA_SERVE_DEBUG_PRINT
    qDebug() << "deal:" << header << " data:" << data;
#endif
    switch (header.protocolTypeID)
    {
    case SA::ProtocolTypeHeartbreat:
    {
        //处理心跳请求
        switch (header.protocolFunID)
        {
        case SA::ProtocolFunReplyHeartbreat:
            //接收到心跳应答，发射信号
            emit receivedHeartbreat(header);
            return (true);

        case SA::ProtocolFunReqHeartbreat:
            //接收到心跳请求，发送心跳应答
            SA::reply_heartbreat_xml(this, header);
            return (true);

        default:
            break;
        }
        return (false);
    }

    case SA::ProtocolTypeXml:
    {
        //解析xml协议
        SAXMLProtocolParser xml;
        if (!xml.fromByteArray(data)) {
            emit error_sa(ErrorInvalidXmlProtocol, tr("Invalid Xml Protocol"));
            return (false);
        }
        return (dealXmlProtocol(header, xml));
    }

    default:
        break;
    }
    return (false);
}


/**
 * @brief 处理收到的xml协议请求
 * @param header
 * @param xml
 * @return 返回false代表数据没有处理，返回true代表数据已经被处理
 */
bool SATcpSocket::dealXmlProtocol(const SAProtocolHeader& header, const SAXMLProtocolParser& xml)
{
    switch (header.protocolFunID)
    {
    case SA::ProtocolFunErrorOcc:
    {
        //处理错误信息
        QString msg;
        int errcode;
        SA::receive_error_xml(&xml, msg, errcode);
        emit receiveError(msg, errcode, header.sequenceID);
        return (true);
    }

    case SA::ProtocolFunReqToken:
    {
        //接收到获取token请求
        int pid;
        QString appid;
        SA::receive_request_token_xml(&xml, pid, appid);
        return (SA::reply_token_xml(this, header, pid, appid));
    }

    case SA::ProtocolFunReplyToken:
    {
        //接收到token应答，发射token内容
        QString token;
        SA::receive_reply_token_xml(&xml, token);
        if (!token.isEmpty()) {
            emit receiveToken(token, header.sequenceID);
            return (true);
        }
        return (false);
    }

    default:
        break;
    }
    return (false);
}
