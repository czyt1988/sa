#include "SALocalConnection.h"

#include "SALocalServeReader.h"
#include "SALocalServeWriter.h"
#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#include <QDebug>
QElapsedTimer s_static_time_elaspade = QElapsedTimer();
#endif

SALocalConnection::SALocalConnection(QLocalSocket *socket, QObject *parent):QObject(parent)
  ,m_socket(socket)
{
    m_writer = new SALocalServeWriter(m_socket,this);
    m_reader = new SALocalServeReader(m_socket,this);
    connect(m_socket,static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error)
            ,this,&SALocalConnection::errorOccurred);
    connect(m_reader,&SALocalServeReader::receivedVectorPointFData
            ,this,&SALocalConnection::onReceivedVectorPointFData);
    connect(m_reader,&SALocalServeReader::receivedString
            ,this,&SALocalConnection::onReceivedString);
    connect(m_reader,&SALocalServeReader::receivedShakeHand
            ,this,&SALocalConnection::onRecShakeHand);
    connect(m_socket,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
    //connect(m_socket,&QLocalSocket::disconnected,this,&SALocalConnection::disconnect);
    connect(m_socket,&QLocalSocket::connected,this,&SALocalConnection::connected);
}

SALocalConnection::~SALocalConnection()
{
#ifdef _DEBUG_OUTPUT
    qDebug() << "~SALocalConnection destroy";
#endif
}
///
/// \brief 错误发生
/// \param err
///
void SALocalConnection::errorOccurred(QLocalSocket::LocalSocketError err)
{
    qDebug() << m_socket->errorString();
}
///
/// \brief 接收到发送过来的数据
/// \param header
/// \param ys
///
void SALocalConnection::onReceivedVectorPointFData(const SALocalServeFigureItemProcessHeader &header, QVector<QPointF> &datas)
{
#ifdef _DEBUG_OUTPUT
    qDebug() << "onReceivedVectorPointFData-> data size:"<<datas.size();
#endif
}
///
/// \brief 接收到发送的文字
/// \param str
///
void SALocalConnection::onReceivedString(const QString &str)
{
#ifdef _DEBUG_OUTPUT
    qDebug() << "onReceivedString:"<<str;
#endif
    if(str == "__test__1m")//这是一个特殊的测试请求，接收到这个字符串后，发送1000,000个点进行计时测试
    {
        QVector<QPointF> points;
        for(int i=0;i<1000000;++i)
        {
            points.append(QPointF(1,1));
        }
        if(m_writer)
        {
            m_writer->sendDoubleVectorData(0,0,points);
        }
    }
}

void SALocalConnection::onRecShakeHand()
{
#ifdef _DEBUG_OUTPUT
    qDebug() << "onRecShakeHand";
#endif
    if(m_writer)
    {
        m_writer->sendShakeHand();
    }
}
