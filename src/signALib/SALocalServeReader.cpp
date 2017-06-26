#include "SALocalServeReader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include <QLocalSocket>
SALocalServeReader::SALocalServeReader(QLocalSocket* localSocket,QObject *parent):QObject(parent)
  ,m_socket(nullptr)
  ,m_isReadedMainHeader(false)
  ,m_dataLength(0)
{
    resetFlags();
    setSocket(localSocket);
}

SALocalServeReader::SALocalServeReader(QObject *parent):QObject(parent)
  ,m_socket(nullptr)
  ,m_isReadedMainHeader(false)
  ,m_dataLength(0)
{
    resetFlags();
}

SALocalServeReader::receiveData(const QByteArray &datas)
{
    const int recDataSize = datas.size();
    for(int i=0;i<recDataSize;++i)
    {
        m_fifo.append(datas[i]);//把数据添加进fifo
    }
    dealRecDatas();
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
        return;
    }
    else
    {
        //文件头接收完毕，开始接收数据
        switch(m_mainHeader.getType())
        {
        case SALocalServeBaseHeader::TypeVectorDoubleDataProc:
            dealVectorDoubleDataProcData();
            break;
        }
    }
}
///
/// \brief 处理主包头
///
void SALocalServeReader::dealMainHeaderData()
{
    const size_t mainHeaderSize = sizeof(SALocalServeBaseHeader);
    if(m_fifo.size() < mainHeaderSize)
    {
        //说明包头未接收完，继续等下一个
        return;
    }
    else if(m_fifo.size() >= mainHeaderSize)
    {
        //说明包头接收完
        QByteArray datas;
        getDataFromFifo(datas,mainHeaderSize);
        QDataStream io(datas);
        io >> m_mainHeader;
        const SALocalServeBaseHeader& header = m_mainHeader;
        if(header.getDataSize() > 0)
        {
            //说明文件头之后有数据
            m_isReadedMainHeader = true;
        }
        else
        {
            //说明文件头之后无数据
            switch(header.getType())
            {
            case SALocalServeBaseHeader::TypeShakeHand://握手协议
                emit receivedShakeHand(header);
                break;
            }
            m_isReadedMainHeader = false;
        }
    }
}

void SALocalServeReader::dealVectorDoubleDataProcData()
{
    if(m_fifo.size() < m_mainHeader.getDataSize())
    {
        //说明数据未接收完，继续等下一个
        return;
    }
    else if(m_fifo.size() >= m_mainHeader.getDataSize())
    {
        //说明包头接收完
        SALocalServeFigureItemProcessHeader header;
        const size_t headerSize = sizeof(SALocalServeFigureItemProcessHeader);
        QByteArray headerDatas;
        getDataFromFifo(headerDatas,headerSize);
        QDataStream ioHeader(headerDatas);
        ioHeader >> header;

        QVector<double> ys;
        QByteArray datas;
        getDataFromFifo(datas,m_mainHeader.getDataSize()-headerSize);
        QDataStream ioData(datas);
        ioData >> ys;

        //数据接收完，重置标记位
        resetFlags();
        emit receivedVectorDoubleData(header,ys);
    }
}

void SALocalServeReader::getDataFromFifo(QByteArray &data, int dataLen)
{
    for(int i=0;i<dataLen;++i)
    {
        data.append(m_fifo.dequeue());
    }
}
///
/// \brief 重置标志位
///
void SALocalServeReader::resetFlags()
{
    m_isReadedMainHeader = false;
    m_dataLength = 0;
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
