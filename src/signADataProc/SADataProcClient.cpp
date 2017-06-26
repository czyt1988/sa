#include "SADataProcClient.h"
#include "SALocalServerDefine.h"
#include "SALocalServeBaseHeader.h"
#include <QCoreApplication>
#include <QMessageBox>
SADataProcClient::SADataProcClient(QObject *parent):QObject(parent)
  ,m_pid(0)
  ,m_writer(nullptr)
{
    m_socket.connectToServer(SA_LOCAL_SERVER_DATA_PROC_NAME);
    m_writer = new SALocalServeWriter(&m_socket,this);
    m_reader = new SALocalServeReader(&m_socket,this);
    connect(m_reader,&SALocalServeReader::receivedVectorDoubleData
            ,this,&SADataProcClient::onReceivedVectorDoubleData);
}

void SADataProcClient::shakeHand()
{
    m_writer->sendShakeHand();
}

void SADataProcClient::errorOccurred(QLocalSocket::LocalSocketError err)
{
    QMessageBox::information(nullptr,"SADataProcClient",m_socket.errorString());
}



///
/// \brief 接收到发送过来的数据
/// \param header
/// \param ys
///
void SADataProcClient::onReceivedVectorDoubleData(const SALocalServeFigureItemProcessHeader &header, QVector<double> &ys)
{

}

uint SADataProcClient::getPid() const
{
    return m_pid;
}

void SADataProcClient::setPid(const uint &pid)
{
    m_pid = pid;
}
