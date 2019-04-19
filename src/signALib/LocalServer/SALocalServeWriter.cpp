#include "SALocalServeWriter.h"
#include <QLocalSocket>
#include "SALocalServeBaseHeader.h"
#include <QTextCodec>
#include <QBuffer>
#include "SALocalServeProtocol.h"
#define _DEBUG_PRINT
#ifdef _DEBUG_PRINT
#include <QDebug>
#endif
SALocalServeWriter::SALocalServeWriter(QLocalSocket* localSocket,QObject *parent):QObject(parent)
  ,m_socket(nullptr)
  ,m_token(0)
{
    setSocket(localSocket);
    connect(&m_timer,&QTimer::timeout,this,&SALocalServeWriter::onShakeHandeTimeout);
}

SALocalServeWriter::SALocalServeWriter(QObject *parent):QObject(parent)
  ,m_socket(nullptr)
  ,m_token(0)
{
    connect(&m_timer,&QTimer::timeout,this,&SALocalServeWriter::onShakeHandeTimeout);
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
/// \brief 发送内容
/// \param header 头的文件大小必须设置
/// \param data 数据区
///
void SALocalServeWriter::send(const SALocalServeBaseHeader &header, const QByteArray &data)
{
    if(!isEnableToWrite())
    {
        return;
    }
    QDataStream out(m_socket);
    out << header;
    if(data.size()>0)
    {
        out << data;
    }
    m_socket->waitForBytesWritten();
}

///
/// \brief 登录(1-1)，成功会返回tokenID
///
void SALocalServeWriter::sendLoginSucceed(uint key)
{
    SALocalServeBaseHeader h;
    h.init();
    h.classID = SA_LOCAL_SER_LOGIN_CLASS;
    h.functionID = SA_LOCAL_SER_LOGIN_FUN;
    h.type = SA_LOCAL_SER_LOGIN_TYPE;
    h.tokenID = getToken();
    h.key = key;
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream st(&buffer);
    st << h.tokenID;
    send(h,data);
}
///
/// \brief 发送握手协议(2-1)
/// \param key 标识号，会随结果返回，用于标识
///
void SALocalServeWriter::sendShakeHand(uint key)
{
    SALocalServeBaseHeader h;
    h.init();
    h.classID = SA_LOCAL_SER_SHAKEHAND_CLASS;
    h.functionID = SA_LOCAL_SER_SHAKEHAND_FUN;
    h.type = SA_LOCAL_SER_SHAKEHAND_TYPE;
    h.tokenID = getToken();
    h.key = key;
    send(h,QByteArray());
}
///
/// \brief 发送线性数组数据，应用于绘图程序绘制完图片后把绘图的点下发给处理进程进行进一步的数据分析
/// \param datas 数据
/// \param key 标识号，会随结果返回，用于标识
///
void SALocalServeWriter::send2DPointFs(const QVector<QPointF>& datas,uint key)
{
    SALocalServeBaseHeader h;
    h.init();
    h.type = SA_LOCAL_SER_PARAM_TYPE;
    h.classID = SA_LOCAL_SER_PARAM_CLASS;
    h.functionID = SA_LOCAL_SER_2DPOINTF_FUN;
    h.tokenID = getToken();
    h.key = key;
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    QDataStream io(&buffer);
    io << datas;
    h.dataSize = byteArray.size();
    send(h,byteArray);

    /*
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
*/
}

void SALocalServeWriter::sendString(const QString& str, uint key)
{
    SALocalServeBaseHeader h;
    h.init();
    h.type = SA_LOCAL_SER_PARAM_TYPE;
    h.classID = SA_LOCAL_SER_PARAM_CLASS;
    h.functionID = SA_LOCAL_SER_STRING_FUN;
    h.tokenID = getToken();
    h.key = key;
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    QDataStream io(&buffer);
    io << str;
    h.dataSize = byteArray.size();
    send(h,byteArray);
    /*
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
*/
}

/**
 * @brief SALocalServeWriter::onShakeHandeTimeout
 */
void SALocalServeWriter::onShakeHandeTimeout()
{
    sendShakeHand();
}

int SALocalServeWriter::getToken() const
{
    return m_token;
}

void SALocalServeWriter::setToken(uint token)
{
    m_token = token;
}

/**
 * @brief 开始定时握手
 * @param freTime 握手频率
 */
void SALocalServeWriter::startHeartbeat(int freTime)
{
    m_timer.start(freTime);
}
