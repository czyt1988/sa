#include "SALocalServeReader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include <QLocalSocket>
#include <QTextCodec>

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
{
    setSocket(localSocket);
}

SALocalServeReader::SALocalServeReader(QObject *parent):QObject(parent)
  ,m_socket(nullptr)
  ,m_isReadedMainHeader(false)
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



void SALocalServeReader::dealVectorDoubleDataProcData(const QByteArray &datas)
{
    QDataStream io(datas);
    qDebug() << "void SALocalServeReader::dealVectorDoubleDataProcData(const QByteArray &datas)";
    SALocalServeFigureItemProcessHeader header;
    io >> header;
    QVector<QPointF> points;
    points.reserve(header.getDataVectorNum());
    double x,y;
    const size_t doubleSize = sizeof(double);
    while(!io.atEnd())
    {
        io.readRawData((char*)&x,doubleSize);
        io.readRawData((char*)&y,doubleSize);
        points.append(QPointF(x,y));
    }
    emit receivedVectorPointFData(header,points);
}



void SALocalServeReader::dealString(const QByteArray &datas)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString str = codec->toUnicode(datas);
    emit receivedString(str);
}


///
/// \brief SALocalServeReader::onReadyRead
///
void SALocalServeReader::onReadyRead()
{
    if(m_isReadedMainHeader)
    {
        if(m_socket->bytesAvailable() >= m_mainHeader.getDataSize())
        {
            //说明数据接收完
#ifdef _DEBUG_PRINT
            qDebug() << "rec Data:"<<m_socket->bytesAvailable()
                     << " bytes \r\n header.getDataSize:"<<m_mainHeader.getDataSize()
                     << "\r\n header.type:"<<m_mainHeader.getType()
                        ;
#endif
            QByteArray datas;
            datas.resize(m_mainHeader.getDataSize());
            if(!readFromSocket(datas.data(),m_mainHeader.getDataSize()))
            {
                emit errorOccure(tr("receive data error"));
                m_isReadedMainHeader = false;
                m_socket->reset();
#ifdef _DEBUG_PRINT
                qDebug() << "can not read from socket io!"
                            ;
#endif
                return;
            }
            switch(m_mainHeader.getType())
            {
            case SALocalServeBaseHeader::TypeVectorPointFData:
                dealVectorDoubleDataProcData(datas);
                break;
            case SALocalServeBaseHeader::TypeString:
                dealString(datas);
                break;
            }
            m_isReadedMainHeader = false;
            if(m_socket->bytesAvailable() >= SALocalServeBaseHeader::sendSize())
            {

                onReadyRead();
            }
        }
    }
    else if(m_socket->bytesAvailable() >= SALocalServeBaseHeader::sendSize())
    {
        //说明包头数据接收完
        QByteArray datas;
        datas.resize(SALocalServeBaseHeader::sendSize());
        if(!readFromSocket(datas.data(),SALocalServeBaseHeader::sendSize()))
        {
            emit errorOccure(tr("receive unknow header"));
            m_isReadedMainHeader = false;
            m_socket->reset();
            return;
        }
        QDataStream io(datas);
        io >> m_mainHeader;
        if(!m_mainHeader.isValid())
        {
            m_isReadedMainHeader = false;
            emit errorOccure(tr("receive unknow header"));
            m_socket->reset();
            return;
        }
#ifdef _DEBUG_PRINT
    qDebug() << "header.getDataSize:"<<m_mainHeader.getDataSize()
             << "\r\n header.key:"<<m_mainHeader.getKey()
             << "\r\n header.type:"<<m_mainHeader.getType()
                ;
#endif
        if(m_mainHeader.getDataSize() > 0)
        {
            //说明文件头之后有数据
            m_isReadedMainHeader = true;
        }
        else
        {
            //说明文件头之后无数据
            switch(m_mainHeader.getType())
            {
            case SALocalServeBaseHeader::TypeShakeHand://握手协议
                emit receivedShakeHand(m_mainHeader);
            }
            m_isReadedMainHeader = false;
        }

        if(m_socket->bytesAvailable() > 0)
        {
            onReadyRead();
        }
    }
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
void SALocalServeReader::setSocket(QLocalSocket *socket)
{
    if(m_socket)
    {
        disconnect(m_socket,&QLocalSocket::readyRead,this,&SALocalServeReader::onReadyRead);
    }
    m_socket = socket;
    if(m_socket)
    {
        connect(m_socket,&QLocalSocket::readyRead,this,&SALocalServeReader::onReadyRead);
    }
}
