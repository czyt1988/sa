#include "SALocalConnection.h"

#include "SALocalServeReader.h"
#include "SALocalServeWriter.h"
#include <QThread>
#include "SADataProcessVectorPointF.h"
#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#include <QDebug>
#include "SAXMLTagDefined.h"
QElapsedTimer s_static_time_elaspade = QElapsedTimer();
#endif

SALocalConnection::SALocalConnection(QLocalSocket *socket, QObject *parent):QObject(parent)
  ,m_socket(socket)
  ,m_calcThread(nullptr)
  ,m_pointFCalctor(nullptr)
{
    qRegisterMetaType<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaType<quintptr>("quintptr");
    initThread();

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
    if(m_calcThread)
    {
        m_calcThread->quit();
        m_calcThread->wait();
    }
#ifdef _DEBUG_OUTPUT
    qDebug() << "~SALocalConnection destroy";
#endif
}

void SALocalConnection::initThread()
{
    m_calcThread = new QThread;
    connect(m_calcThread,&QThread::finished,m_calcThread,&QThread::deleteLater);//内存释放
    //数组处理线程
    m_pointFCalctor = new SADataProcessVectorPointF;
    m_pointFCalctor->moveToThread(m_calcThread);
    connect(m_calcThread,&QThread::finished,m_pointFCalctor,&SADataProcessVectorPointF::deleteLater);//内存释放
    connect(m_pointFCalctor,&SADataProcessVectorPointF::result
            ,this,&SALocalConnection::onProcessVectorPointFResult);
    connect(this,&SALocalConnection::callVectorPointFProcess
            ,m_pointFCalctor,&SADataProcessVectorPointF::setPoints);
    //
    m_calcThread->start();
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
    qDebug() << "onReceivedVectorPointFData-> data size:"<<datas.size()
             << " WndPtr:"<<header.getWndPtr()
             << " FigPtr:"<<header.getFigPtr()
             << " ItemPtr:"<<header.getItemPtr()
                ;
#endif
    emit callVectorPointFProcess(datas,header.getWndPtr(),header.getFigPtr(),header.getItemPtr());
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
            m_writer->sendDoubleVectorData(0,0,0,points);
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
///
/// \brief 接收到点数组的计算结果
/// \param result 计算结果指针，需要手动销毁
/// \param widget 标记1
/// \param item 标记2
///
void SALocalConnection::onProcessVectorPointFResult(SADataFeatureItem *result, quintptr widget,quintptr fig, quintptr item)
{
    if(nullptr == result)
    {
        return;
    }
#ifdef _DEBUG_OUTPUT
    QElapsedTimer t;
    t.start();
#endif
    QString xml;
    QTextStream st(&xml);
    st << "<" << SA_XML_TAG_SA << " " << SA_XML_ATT_NAME_SA_TYPE << "=\"" <<  SA_XML_ATT_SA_TYPE_VPFR << "\">";
    st << "<" << SA_XML_TAG_QUINTPTR << ">" << widget << "</" << SA_XML_TAG_QUINTPTR << ">";
    st << "<" << SA_XML_TAG_QUINTPTR << ">" << fig << "</" << SA_XML_TAG_QUINTPTR << ">";
    st << "<" << SA_XML_TAG_QUINTPTR << ">" << item << "</" << SA_XML_TAG_QUINTPTR << ">";
    st << result->toXml();
    st << "</" << SA_XML_TAG_SA << ">";
    st << endl;
    m_writer->sendString(xml);
#ifdef _DEBUG_OUTPUT
    qDebug() << "onProcessVectorPointFResult cost:" << t.elapsed()
             << " WndPtr:"<<widget
             << " FigPtr:"<<fig
             << " ItemPtr:"<<item
                ;
#endif
    delete result;
}
