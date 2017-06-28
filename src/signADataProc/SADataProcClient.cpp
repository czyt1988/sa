#include "SADataProcClient.h"
#include "SALocalServerDefine.h"
#include "SALocalServeBaseHeader.h"
#include <QCoreApplication>
#include <QMessageBox>

#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#include <QDebug>
QElapsedTimer s_static_time_elaspade = QElapsedTimer();
#endif

SADataProcClient::SADataProcClient(QObject *parent):QObject(parent)
  ,m_pid(0)
  ,m_writer(nullptr)
{
    m_socket.connectToServer(SA_LOCAL_SERVER_DATA_PROC_NAME);
    m_writer = new SALocalServeWriter(&m_socket,this);
    m_reader = new SALocalServeReader(&m_socket,this);
    connect(&m_socket,static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error)
            ,this,&SADataProcClient::errorOccurred);
    connect(m_reader,&SALocalServeReader::receivedVectorPointFData
            ,this,&SADataProcClient::onReceivedVectorPointFData);
    connect(m_reader,&SALocalServeReader::receivedString
            ,this,&SADataProcClient::onReceivedString);
}

void SADataProcClient::shakeHand()
{
    if(nullptr == m_writer)
    {
        return;
    }
    m_writer->sendShakeHand();
}

void SADataProcClient::errorOccurred(QLocalSocket::LocalSocketError err)
{
#ifdef _DEBUG_OUTPUT
    qDebug() << m_socket.errorString();
#endif
    QMessageBox::information(nullptr,"SADataProcClient",m_socket.errorString());
}



///
/// \brief 接收到发送过来的数据
/// \param header
/// \param ys
///
void SADataProcClient::onReceivedVectorPointFData(const SALocalServeFigureItemProcessHeader &header, QVector<QPointF> &datas)
{
    if(nullptr == m_writer)
    {
        return;
    }
    m_writer->sendString("ces");
}

void SADataProcClient::onReceivedString(const QString &str)
{
#ifdef _DEBUG_OUTPUT
    qDebug() << "recive string:" << str;
#endif
    if(str == "test")
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

uint SADataProcClient::getPid() const
{
    return m_pid;
}

void SADataProcClient::setPid(const uint &pid)
{
    m_pid = pid;
}
