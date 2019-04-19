#include "SALocalServeReader.h"

#include <QLocalSocket>
#include <QTextCodec>
#include <QDebug>
//#define _DEBUG_PRINT
#ifdef _DEBUG_PRINT
#include <QElapsedTimer>
#include <QDebug>
#include "SALog.h"
QElapsedTimer s__elaspade = QElapsedTimer();
#endif

SALocalServeReader::SALocalServeReader(QLocalSocket* localSocket,QObject *parent):QObject(parent)
  ,m_socket(nullptr)
  ,m_isReadedMainHeader(false)
  ,m_index(0)
{
    setSocket(localSocket);
}

SALocalServeReader::SALocalServeReader(QObject *parent):QObject(parent)
  ,m_socket(nullptr)
  ,m_isReadedMainHeader(false)
  ,m_index(0)
{

}

///
/// \brief 从socket中安全的读取文件
/// \param p 指针
/// \param n 读取的长度
///
bool SALocalServeReader::readFromSocket(void *p, int n)
{
    int readLen = 0,index = 0;
    bool ret = false;
    do
    {
        readLen = m_socket->read((char*)p+index,n);
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
///
/// \brief 根据类型分发协议
/// \param type
/// \param datas
///
void SALocalServeReader::deal(const SALocalServeBaseHeader &head, const QByteArray &datas)
{
    emit receive(head,datas);

    /*
    switch(head)
    {
    case SALocalServeBaseProtocol::TypeShakeHand://握手协议
        dealShakeHand(datas);
        break;
    case SALocalServeBaseProtocol::TypeVectorPointFData:
        dealVectorDoubleDataProcData(datas);
        break;
    case SALocalServeBaseProtocol::TypeString:
        dealString(datas);
        break;
    default:
    {
        qDebug() << "unknow protocol type!";
        emit errorOccure(tr("unknow protocol type!"));
        m_isReadedMainHeader = false;
        m_socket->reset();
        break;
    }
    }
    */
}



/////
///// \brief 处理线性数组的数据
///// \param datas
/////
//void SALocalServeReader::dealVectorDoubleDataProcData(const QByteArray &datas)
//{
//#ifdef _DEBUG_PRINT
//    qDebug() << "SALocalServeReader::dealVectorDoubleDataProcData";
//#endif
//    QDataStream io(datas);
//    SALocalServeVectorPointProtocol pl;
//    io >> pl;
//    emit receivedVectorPointFData(pl);
//}



//void SALocalServeReader::dealString(const QByteArray &datas)
//{
//    QDataStream io(datas);
//    SALocalServeStringProtocol pl;
//    io >> pl;
//    emit receivedString(pl);
//}


///
/// \brief SALocalServeReader::onReadyRead
///
void SALocalServeReader::onReadyRead()
{
    const static unsigned int s_headerSize = sizeof(SALocalServeBaseHeader);
    if(m_isReadedMainHeader)
    {
        if(m_socket->bytesAvailable() >= m_mainHeader.dataSize)
        {
            //说明数据接收完
#ifdef _DEBUG_PRINT
            qDebug() << "rec Data:"<<m_socket->bytesAvailable()
                     << " bytes \r\n header.getDataSize:"<<m_mainHeader.dataSize
                     << "\r\n header.type:"<<m_mainHeader.type
                     << "current index:" << m_index
                        ;
#endif
            if(m_buffer.size() < s_headerSize+m_mainHeader.dataSize)
            {
                m_buffer.resize(s_headerSize+m_mainHeader.dataSize);
            }
            if(!readFromSocket(m_buffer.data()+m_index,m_mainHeader.dataSize))
            {
                emit errorOccure(tr("receive data error"));
                m_isReadedMainHeader = false;
                m_socket->reset();
                qDebug() << __FILE__ <<"[" << __FUNCTION__ << "][" << __LINE__ << "]can not read from socket io!";
                return;
            }
            m_index += m_mainHeader.dataSize;
            deal(m_mainHeader,m_buffer);
            m_isReadedMainHeader = false;
            if(m_socket->bytesAvailable() >= s_headerSize)
            {
                onReadyRead();
            }
        }
    }
    else if(m_socket->bytesAvailable() >= s_headerSize)
    {
        //说明包头数据接收完
#ifdef _DEBUG_PRINT
    qDebug() <<"main header may receive:"
            << "\r\n byte available:"<<m_socket->bytesAvailable()
                ;
#endif
        m_index = 0;
        if(m_buffer.size() < s_headerSize)
        {
            m_buffer.resize(s_headerSize);
        }
        if(!readFromSocket(m_buffer.data(),s_headerSize))
        {
            emit errorOccure(tr("receive unknow header"));
            m_isReadedMainHeader = false;
            m_socket->reset();
            return;
        }
        m_index += s_headerSize;
        QDataStream io(m_buffer);
        io >> m_mainHeader;
        if(!m_mainHeader.isValid())
        {
            m_isReadedMainHeader = false;
            qDebug() << "receive unknow header";
            emit errorOccure(tr("receive unknow header"));
            m_socket->reset();
            return;
        }
#ifdef _DEBUG_PRINT
    qDebug() << "header.getDataSize:"<<m_mainHeader.dataSize
             << "\r\n header.key:"<<m_mainHeader.key
             << "\r\n header.type:"<<m_mainHeader.type
             << "\r\n byte available:"<<m_socket->bytesAvailable()
                ;
#endif
        if(m_mainHeader.dataSize > 0)
        {
            //说明文件头之后有数据
            m_isReadedMainHeader = true;
        }
        else
        {
            //说明文件头之后无数据
#ifdef _DEBUG_PRINT
            qDebug() << "!!!!mainHeader dataSize == 0: type:"<<m_mainHeader.type
                            ;
#endif
            deal(m_mainHeader,m_buffer);
            m_isReadedMainHeader = false;
        }

        if(m_socket->bytesAvailable() > 0)
        {
            onReadyRead();
        }
    }
}

void SALocalServeReader::onDisconnected()
{
    emit disconnectFromServe();
}

void SALocalServeReader::onError(QLocalSocket::LocalSocketError socketError)
{
    qDebug() << __FILE__<<":"<<__FUNCTION__ << "err code:"<<(int)socketError;
    qDebug() << __FILE__<<":"<<__FUNCTION__ << m_socket->errorString();
}

int SALocalServeReader::getToken() const
{
    return m_token;
}

void SALocalServeReader::setToken(int token)
{
    m_token = token;
}
///
/// \brief 获取套接字
/// \return
///
QLocalSocket *SALocalServeReader::getSocket() const
{
    return m_socket;
}
///
/// \brief 设置套接字
/// \param socket
///
void SALocalServeReader::setSocket(QLocalSocket *socket,bool autoDicConnect)
{
    if(autoDicConnect)
    {
        if(m_socket)
        {
            disconnect(m_socket,&QLocalSocket::readyRead,this,&SALocalServeReader::onReadyRead);
        }
    }
    m_socket = socket;
    if(m_socket)
    {
        connect(m_socket,&QLocalSocket::readyRead,this,&SALocalServeReader::onReadyRead);
        connect(m_socket,&QLocalSocket::disconnected,this,&SALocalServeReader::onDisconnected);
        connect(m_socket,static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error)
                ,this,&SALocalServeReader::onError);
    }
}
