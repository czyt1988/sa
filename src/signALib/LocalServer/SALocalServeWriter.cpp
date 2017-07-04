#include "SALocalServeWriter.h"
#include <QLocalSocket>
#include "SALocalServeBaseHeader.h"
#include "SALocalServeFigureItemProcessHeader.h"
#include <QTextCodec>
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

    SALocalServeBaseHeader data;

    data.setKey(1);
    data.setType((int)SALocalServeBaseHeader::TypeShakeHand);
    data.refreshCheck();
    QDataStream out(m_socket);
    out << data;
    m_socket->waitForBytesWritten();
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
    size_t dataTotalSize = SALocalServeFigureItemProcessHeader::sendSize();
    size_t dataLen = datas.size() * sizeof(double) * 2;
    dataTotalSize += dataLen;

    SALocalServeBaseHeader mainHeader;
    mainHeader.setKey(1);
    mainHeader.setType((int)SALocalServeBaseHeader::TypeVectorPointFData);
    mainHeader.setDataSize(dataTotalSize);
    mainHeader.refreshCheck();

    SALocalServeFigureItemProcessHeader subHeader;
    subHeader.setDataType(SALocalServeFigureItemProcessHeader::CurveData);
    subHeader.setWndPtr(wndPtr);
    subHeader.setFigPtr(figPtr);
    subHeader.setItem(itemPtr);
    subHeader.setDataLength(dataLen);
    subHeader.setDataVectorNum(datas.size());
    subHeader.refreshCheck();

    QDataStream out(m_socket);
    out << mainHeader << subHeader;
    //写数据
    const int size = datas.size();
    double x,y;
    size_t doubleSize = sizeof(double);
    for(int i=0;i<size;++i)
    {
        x = datas[i].x();
        y = datas[i].y();
        out.writeRawData((const char *)&x,doubleSize);
        out.writeRawData((const char *)&y,doubleSize);
    }
    m_socket->waitForBytesWritten();
}

void SALocalServeWriter::sendString(const QString str)
{
    if(!isEnableToWrite())
    {
        return;
    }
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray encodedString = codec->fromUnicode(str);
    SALocalServeBaseHeader mainHeader;
    mainHeader.setKey(1);
    mainHeader.setType((int)SALocalServeBaseHeader::TypeString);
    mainHeader.setDataSize(encodedString.size());
    mainHeader.refreshCheck();

    QDataStream out(m_socket);
    out << mainHeader;
    out.writeRawData(encodedString.data(),encodedString.size());
    m_socket->waitForBytesWritten();
}
