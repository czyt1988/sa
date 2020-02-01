#include "SATcpSocket.h"
#include "SAProtocolHeader.h"
#include "SAXMLProtocolParser.h"
class SATcpSocketPrivate
{
    SA_IMPL_PUBLIC(SATcpSocket)
public:
    SATcpSocketPrivate(SATcpSocket* p);
    void deal(const SAProtocolHeader& header,const QByteArray& data);
    SAProtocolHeader m_mainHeader;
    bool m_isReadedMainHeader;
    uint m_dataSize;
    uint m_index;
    QByteArray m_buffer;
};

SATcpSocketPrivate::SATcpSocketPrivate(SATcpSocket *p):q_ptr(p)
  ,m_isReadedMainHeader(false)
  ,m_dataSize(0)
  ,m_index(0)
{

}

void SATcpSocketPrivate::deal(const SAProtocolHeader &header, const QByteArray &data)
{

}

SATcpSocket::SATcpSocket(QObject *par)
{
    connect(this,&QIODevice::readyRead,this,&SATcpSocket::onReadyRead);
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
    int readLen = 0,index = 0;
    bool ret = false;
    do
    {
        readLen = read((char*)p+index,n);
        if(readLen <= 0)
        {
            break;
        }
        index += readLen;
        n -= readLen;
    }while( readLen && n );
    if(0 == n)
    {
        ret = true;
    }
    return ret;
}

void SATcpSocket::onReadyRead()
{
    const static unsigned int s_headerSize = sizeof(SAProtocolHeader);
    if(d_ptr->m_isReadedMainHeader)
    {
        //此时已经读取了头
        if(bytesAvailable() >= d_ptr->m_mainHeader.dataSize)
        {
            //说明数据接收完
#ifdef _DEBUG_PRINT
            qDebug() << " rec Data:"<<bytesAvailable()<< " bytes "
                     <<"\n header:"<<d_ptr->m_mainHeader
                        ;
#endif
            if(d_ptr->m_buffer.size() < d_ptr->m_mainHeader.dataSize)
            {
                d_ptr->m_buffer.resize(d_ptr->m_mainHeader.dataSize);
            }
            if(!readFromSocket(d_ptr->m_buffer.data()+d_ptr->m_index,d_ptr->m_mainHeader.dataSize-d_ptr->m_index))
            {
                d_ptr->m_isReadedMainHeader = false;
                abort();
                qDebug() << "socket abort!!!  can not read from socket io!";
                return;
            }
#ifdef _DEBUG_PRINT
            printQByteArray(d_ptr->m_buffer);
#endif
            d_ptr->m_index += d_ptr->m_mainHeader.dataSize;
            d_ptr->m_isReadedMainHeader = false;
            if(bytesAvailable() >= s_headerSize)
            {
                onReadyRead();
            }
        }
    }
    else if(bytesAvailable() >= s_headerSize)
    {
        //说明包头数据接收完
#ifdef _DEBUG_PRINT
        qDebug() <<"main header may receive:"
                << "\r\n byte available:"<<bytesAvailable()
                    ;
#endif
        d_ptr->m_index = 0;
        if(!readFromSocket((void*)(&(d_ptr->m_mainHeader)),s_headerSize))
        {
            qDebug() << "can not read from socket" << __LINE__;
            d_ptr->m_isReadedMainHeader = false;
            abort();
            return;
        }
#ifdef _DEBUG_PRINT
        qDebug() << "readed header from socket:"
                 << d_ptr->m_mainHeader
                 ;
#endif
        d_ptr->m_index = 0;
        if(!(d_ptr->m_mainHeader.isValid()))
        {
            d_ptr->m_isReadedMainHeader = false;
            qDebug() << "receive unknow header[1]"
                     << "\n" << d_ptr->m_mainHeader;
            abort();
            return;
        }
        if(d_ptr->m_mainHeader.dataSize > 0)
        {
            //说明文件头之后有数据
            d_ptr->m_isReadedMainHeader = true;
        }
        else
        {
            //说明文件头之后无数据
            d_ptr->deal(d_ptr->m_mainHeader,QByteArray());
            d_ptr->m_isReadedMainHeader = false;
        }

        if(bytesAvailable() > 0)
        {
#ifdef _DEBUG_PRINT
        qDebug() << "have avaliable data,size:"<<bytesAvailable()
                        ;
#endif
            onReadyRead();
        }
    }
}


