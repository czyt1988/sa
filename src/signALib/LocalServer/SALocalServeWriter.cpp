#include "SALocalServeWriter.h"
#include <QLocalSocket>
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include <QTextCodec>
#include "SALocalServeBaseProtocol.h"
#define _DEBUG_PRINT
#ifdef _DEBUG_PRINT
#include <QDebug>
#endif
SALocalServeWriter::SALocalServeWriter(QLocalSocket* localSocket,QObject *parent):QObject(parent)
  ,m_socket(nullptr)
{
    setSocket(localSocket);
}

SALocalServeWriter::SALocalServeWriter(QObject *parent):QObject(parent)
  ,m_socket(nullptr)
{

}

///
/// \brief 获取套接字
/// \return
///
QLocalSocket *SALocalServeWriter::getSocket() const
{
    return m_socket;
}
///
/// \brief 设置套接字
/// \param socket
///
void SALocalServeWriter::setSocket(QLocalSocket *socket)
{
    m_socket = socket;
}

bool SALocalServeWriter::isEnableToWrite() const
{
    if(nullptr == m_socket)
    {
        return false;
    }
    if(!m_socket->isValid())
    {
        return false;
    }
    if(!m_socket->isWritable())
    {
        return false;
    }
    return true;
}
///
/// \brief 发送握手协议
///
void SALocalServeWriter::sendShakeHand()
{

    if(!isEnableToWrite())
    {
        return;
    }
    SALocalServeShakeHandProtocol sh;
    QDataStream out(m_socket);
    out << sh;
    m_socket->waitForBytesWritten();
#ifdef _DEBUG_PRINT
    qDebug() << "SALocalServeWriter::sendShakeHand:";
#endif
}
///
/// \brief 发送线性数组数据，应用于绘图程序绘制完图片后把绘图的点下发给处理进程进行进一步的数据分析
/// \param wndPtr 对应子窗口的指针，这个指针只作为标记用，无实际作用
/// \param itemPtr 对应绘图的曲线指针，这个指针只作为标记用，无实际作用
/// \param datas
///
void SALocalServeWriter::sendDoubleVectorData(qintptr wndPtr,qintptr figPtr,qintptr itemPtr, const QVector<QPointF> &datas)
{
    if(!isEnableToWrite())
    {
        return;
    }
    SALocalServeVectorPointProtocol sh;
    sh.setPoints(datas);
    sh.setIDs(wndPtr,figPtr,itemPtr);
    QDataStream out(m_socket);
    out << sh;
    m_socket->waitForBytesWritten();
#ifdef _DEBUG_PRINT
    qDebug() << "SALocalServeWriter::sendDoubleVectorData: dataSize(" << datas.size()<<")";
#endif
}

void SALocalServeWriter::sendString(const QString str)
{
    if(!isEnableToWrite())
    {
        return;
    }
    SALocalServeStringProtocol sh;
    sh.setString(str);
    QDataStream out(m_socket);
    out << sh;
    m_socket->waitForBytesWritten();
#ifdef _DEBUG_PRINT
    qDebug() << "SALocalServeWriter::sendString:" << str;
#endif
}
