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
#ifndef __USE_FIFO__
  ,m_readedDataSize(0)
  ,m_readPos(0)
#endif
{
    resetFlags();
    setSocket(localSocket);
#ifndef __USE_FIFO__
    m_buffer.resize(SALocalServeBaseHeader::sendSize());
#endif
}

SALocalServeReader::SALocalServeReader(QObject *parent):QObject(parent)
  ,m_socket(nullptr)
  ,m_isReadedMainHeader(false)
#ifndef __USE_FIFO__
  ,m_readedDataSize(0)
  ,m_readPos(0)
#endif
{
    resetFlags();
#ifndef __USE_FIFO__
    m_buffer.resize(SALocalServeBaseHeader::sendSize());
#endif
}

SALocalServeReader::receiveData(const QByteArray &datas)
{
    const int recDataSize = datas.size();
#ifdef __USE_FIFO__
    for(int i=0;i<recDataSize;++i)
    {
        m_fifo.push(datas[i]);//把数据添加进fifo
    }
    dealRecDatas();
#else
    m_buffer.insert(m_readedDataSize,datas);
    m_readedDataSize += datas.size();
    dealRecDatas();
#endif
}
///
/// \brief 处理接收到的数据
///
void SALocalServeReader::dealRecDatas()
{
    if(!m_isReadedMainHeader)
    {
        //说明还没接收主包头，开始进行主包头数据解析
        dealMainHeaderData();
    }
#ifdef __USE_FIFO__
    if((m_fifo.size() > 0) && m_isReadedMainHeader){
#else
    if(((m_readedDataSize-m_readPos) > 0) && m_isReadedMainHeader){
#endif
        //文件头接收完毕，开始接收数据
        switch(m_mainHeader.getType())
        {
        case SALocalServeBaseHeader::TypeVectorPointFData:
            dealVectorDoubleDataProcData();
            break;
        case SALocalServeBaseHeader::TypeString:
            dealString();
            break;
        }
    }
}
///
/// \brief 处理主包头
///
void SALocalServeReader::dealMainHeaderData()
{
    const size_t mainHeaderSize = SALocalServeBaseHeader::sendSize();
#ifdef _DEBUG_PRINT
    #ifdef __USE_FIFO__
        qDebug() << "dealMainHeaderData mainHeaderSize:"<<mainHeaderSize
                 << "\n current fifo size:" << m_fifo.size()
                    ;
    #else
        qDebug() << "dealMainHeaderData mainHeaderSize:"<<mainHeaderSize
                 << "\n current buffer size:" << m_readedDataSize
                    << "\r\n curent read pos:" << m_readPos
                    ;
    #endif
#endif

#ifdef __USE_FIFO__
    if(m_fifo.size() < mainHeaderSize){
#else
    if((m_readedDataSize-m_readPos) < mainHeaderSize){
#endif

        //说明包头未接收完，继续等下一个
        return;
    }
#ifdef __USE_FIFO__
    else if(m_fifo.size() >= mainHeaderSize){
#else
    else if((m_readedDataSize-m_readPos) >= mainHeaderSize){
#endif
        //说明包头接收完
        QByteArray datas;
        getDataFromBuffer(datas,mainHeaderSize);
        QDataStream io(datas);
        io >> m_mainHeader;
        const SALocalServeBaseHeader& header = m_mainHeader;
#ifdef _DEBUG_PRINT
    qDebug() << "header.getDataSize:"<<header.getDataSize()
             << "\r\n header.key:"<<header.getKey()
             << "\r\n header.type:"<<header.getType()
                ;
#endif
        if(!header.isValid())
        {
            m_isReadedMainHeader = false;
            emit errorOccure(tr("receive unknow header"));
            return;
        }
        if(header.getDataSize() > 0)
        {
            //说明文件头之后有数据
            m_isReadedMainHeader = true;
#ifdef _DEBUG_PRINT
            s__elaspade.restart();
#endif
        }
        else
        {
            //说明文件头之后无数据
            switch(header.getType())
            {
            case SALocalServeBaseHeader::TypeShakeHand://握手协议
                emit receivedShakeHand(header);
            }
            m_isReadedMainHeader = false;
        }
    }
}

void SALocalServeReader::dealVectorDoubleDataProcData()
{
#ifdef __USE_FIFO__
    if(m_fifo.size() < m_mainHeader.getDataSize()){
#else
    if((m_readedDataSize-m_readPos) < m_mainHeader.getDataSize()){
#endif
        //说明数据未接收完，继续等下一个
        return;
    }

#ifdef __USE_FIFO__
    else if(m_fifo.size() >= m_mainHeader.getDataSize()){
#else
    else if((m_readedDataSize-m_readPos) >= m_mainHeader.getDataSize()){
#endif

#ifdef _DEBUG_PRINT
        QElapsedTimer t;
        t.start();
        int __tmp_el_1 = 0;
        int __tmp_el_2 = 0;
#ifdef __USE_FIFO__
    qDebug() << "dealVectorDoubleDataProcData size:"<<m_mainHeader.getDataSize()
             << "\r\n rec VectorDoubleData curent fifo size:" << m_fifo.size()
                ;
#else
    qDebug() << "dealVectorDoubleDataProcData size:"<<m_mainHeader.getDataSize()
             << "\r\n rec VectorDoubleData curent buffer size:" << m_readedDataSize
                << "\r\n curent read pos:" << m_readPos
                ;
#endif
#endif

        //说明包头接收完
        const size_t headerSize = SALocalServeFigureItemProcessHeader::sendSize();
        SALocalServeFigureItemProcessHeader header;

        {
            QByteArray headerDatas;
            getDataFromBuffer(headerDatas,headerSize);
            QDataStream ioHeader(headerDatas);
            ioHeader >> header;
        }
#ifdef _DEBUG_PRINT
        __tmp_el_2 = t.elapsed();
        qDebug() << "get sub header cost:" << __tmp_el_2 - __tmp_el_1;
        __tmp_el_1 = __tmp_el_2;
#endif
        QByteArray datasBuffer;
        getDataFromBuffer(datasBuffer,m_mainHeader.getDataSize()-headerSize);
#ifdef _DEBUG_PRINT
        __tmp_el_2 = t.elapsed();
        qDebug() << "get getDataFromFifo cost:" << __tmp_el_2 - __tmp_el_1;
        __tmp_el_1 = __tmp_el_2;
#endif
        if(!header.isValid())
        {
#ifdef _DEBUG_PRINT
    qDebug() << "receive unknow vector double data:"
             <<"\r\n getDataLength:" << header.getDataLength()
               <<"\r\n getDataType:" << header.getDataType()
                 <<"\r\n getDataVectorNum:" << header.getDataVectorNum()
               ;
#endif
            emit errorOccure(tr("receive unknow vector double data"));
            return;
        }

        QDataStream ioData(datasBuffer);
        QVector<QPointF> points;
        points.reserve(header.getDataVectorNum());
        double x,y;
        const size_t doubleSize = sizeof(double);
        while(!ioData.atEnd())
        {
            ioData.readRawData((char*)&x,doubleSize);
            ioData.readRawData((char*)&y,doubleSize);
            points.append(QPointF(x,y));
        }

#ifdef _DEBUG_PRINT
        __tmp_el_2 = t.elapsed();
        qDebug() << "get points cost:" << __tmp_el_2-__tmp_el_1;
        __tmp_el_1 = __tmp_el_2;
#endif

#ifdef _DEBUG_PRINT
    qDebug() << "points size:"<<points.size();
    qDebug() << "dealVectorDoubleDataProcData fun cost:" << t.elapsed();
    qDebug() << "total dealVectorDoubleDataProcData cost:" << s__elaspade.elapsed();
#endif

        //数据接收完，重置标记位
        resetFlags();
        emit receivedVectorPointFData(header,points);
    }
}

void SALocalServeReader::dealString()
{
#ifdef __USE_FIFO__
    if(m_fifo.size() < m_mainHeader.getDataSize()){
#else
    if((m_readedDataSize-m_readPos) < m_mainHeader.getDataSize()){
#endif
        //说明数据未接收完，继续等下一个
        return;
    }
#ifdef __USE_FIFO__
    if(m_fifo.size() >= m_mainHeader.getDataSize()){
#else
    else if((m_readedDataSize-m_readPos) >= m_mainHeader.getDataSize()){
#endif

#ifdef _DEBUG_PRINT
#ifdef __USE_FIFO__
    qDebug() << "data size:"<<m_mainHeader.getDataSize()
             << "\r\n dealString curent fifo size:" << m_fifo.size()
                ;
#else
    qDebug() << "data size:"<<m_mainHeader.getDataSize()
             << "\r\n dealString curent buffer size:" << m_readedDataSize
             << "\r\n curent read pos:" << m_readPos
                ;
#endif
#endif
        QByteArray datasBuffer;
        getDataFromBuffer(datasBuffer,m_mainHeader.getDataSize());
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QString str = codec->toUnicode(datasBuffer);
        //数据接收完，重置标记位
#ifdef _DEBUG_PRINT
    qDebug() << "read str:"  << str;
    qDebug() << "total dealString cost:" << s__elaspade.elapsed();
#endif
        resetFlags();
        emit receivedString(str);
    }

}

void SALocalServeReader::getDataFromBuffer(QByteArray &data, int dataLen)
{
#ifdef __USE_FIFO__
    data.resize(dataLen);
    for(int i=0;i<dataLen;++i)
    {
        data[i] = m_fifo.front();
        m_fifo.pop();
    }

#else
    data.resize(dataLen);
    memcpy(data.data(),m_buffer.data()+m_readPos,dataLen);
    //把剩下的数据移动到前面
    m_readPos += dataLen;
#endif
}
///
/// \brief 重置标志位
///
void SALocalServeReader::resetFlags()
{
    m_isReadedMainHeader = false;
#ifdef __USE_FIFO__
    if(m_fifo.empty())
    {
        std::queue<char> tt;
        m_fifo.swap(tt);//清空内存
    }
#else
    if(m_readPos < m_readedDataSize)
    {
        QByteArray tmp;
        getDataFromBuffer(tmp,m_readedDataSize-m_readPos);
        m_buffer.swap(tmp);
        if(m_buffer.size() < SALocalServeBaseHeader::sendSize())
        {
            m_buffer.resize(SALocalServeBaseHeader::sendSize());
        }
        m_readPos = 0;
        m_readedDataSize = m_buffer.size();
    }
    else
    {
        m_buffer.resize(SALocalServeBaseHeader::sendSize());
        m_readPos = 0;
        m_readedDataSize = 0;
    }
#endif
}
///
/// \brief SALocalServeReader::onReadyRead
///
void SALocalServeReader::onReadyRead()
{
    QByteArray arr =  m_socket->readAll();
    receiveData(arr);
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
