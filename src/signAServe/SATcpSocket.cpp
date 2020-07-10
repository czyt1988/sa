#include "SATcpSocket.h"
#include "SAProtocolHeader.h"
#include "SAXMLProtocolParser.h"
#include "SAServeHandleFun.h"
#include "SASocketHandle.h"

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
    SASocketHandle *h = new SASocketHandle(this);

    setupHandle(h);
}


SATcpSocket::~SATcpSocket()
{
    if (d_ptr->m_handle) {
        d_ptr->m_handle->deleteLater();
    }
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
 * @brief 重新设置socket的处理
 * @param handle socket的处理所有权归socket所有
 */
SAAbstractSocketHandle *SATcpSocket::setupHandle(SAAbstractSocketHandle *handle)
{
    if (handle == d_ptr->m_handle) {
        return (nullptr);
    }
    SAAbstractSocketHandle *old = d_ptr->m_handle;

    handle->setSocket(this);
    d_ptr->m_handle = handle;
    return (old);
}


/**
 * @brief 获取处理类
 * @return
 */
SAAbstractSocketHandle *SATcpSocket::getHandle() const
{
    return (d_ptr->m_handle);
}


/**
 * @brief 读取完整的数据后调用，此类会发射 @sa receivedData 信号
 * @param header
 * @param data
 */
void SATcpSocket::deal(const SAProtocolHeader& header, const QByteArray& data)
{
    emit receivedData(header, data);
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
 * @brief readyRead对应的槽函数，处理文件头和数据
 */
void SATcpSocket::onReadyRead()
{
    const static unsigned int s_headerSize = sizeof(SAProtocolHeader);

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
    }else if (bytesAvailable() >= s_headerSize)   {
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
