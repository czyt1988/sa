#include "SADataProcServe.h"
#include "SALocalServerDefine.h"
#include "SALocalServeBaseHeader.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QLocalServer>
#include <QThread>
#include <QApplication>
#include "SADataProcessVectorPointF.h"
#include "SAXMLTagDefined.h"
#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#endif
SADataProcServe::SADataProcServe(QObject *parent):QObject(parent)
  ,m_pid(0)
  ,m_localServer(new QLocalServer(this))
  ,m_willBeQuit(false)
{
    qRegisterMetaType<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaType<quintptr>("quintptr");
    initCalcThread();
    connect(m_localServer,&QLocalServer::newConnection
            ,this,&SADataProcServe::onLocalServeNewConnection);
    if(!m_localServer->listen(SA_LOCAL_SERVER_DATA_PROC_NAME))
    {
       qDebug() << tr("listern loacl server error");
    }
    qDebug() << "start DataProc Serve";
    connect(&m_liveCheck,&QTimer::timeout
            ,this,&SADataProcServe::onLifeCheckTimeOut);
    m_liveCheck.start(5000);
}


void SADataProcServe::onLocalServeNewConnection()
{
    qDebug() << "New Connection";
    QLocalSocket* socket = m_localServer->nextPendingConnection();
    if(nullptr == socket)
    {
        qDebug() << "can not exec m_localServer->nextPendingConnection();";
        return;
    }
    SALocalServeWriter* writer = new SALocalServeWriter(socket,socket);
    SALocalServeReader* reader = new SALocalServeReader(socket,socket);
    m_connectList.insert(socket);
    m_writerDict[socket] = writer;
    m_readerDict[socket] = reader;
    connect(socket,static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error)
            ,this,&SADataProcServe::errorOccurred);
    connect(reader,&SALocalServeReader::receivedVectorPointFData
            ,this,&SADataProcServe::onReceivedVectorPointFData);
    connect(reader,&SALocalServeReader::receivedString
            ,this,&SADataProcServe::onReceivedString);
    connect(reader,&SALocalServeReader::receivedShakeHand
            ,this,&SADataProcServe::onRecShakeHand);
    connect(socket,&QLocalSocket::disconnected,this,&SADataProcServe::onDisconnected);



}

void SADataProcServe::errorOccurred(QLocalSocket::LocalSocketError err)
{
    Q_UNUSED(err);
    QLocalSocket* socket  = qobject_cast<QLocalSocket*>(sender());
    if(socket)
    {
        qDebug() << socket->errorString();
    }
}

void SADataProcServe::onReceivedVectorPointFData(const SALocalServeFigureItemProcessHeader &header, QVector<QPointF> &datas)
{
    SALocalServeReader* reader  = qobject_cast<SALocalServeReader*>(sender());
    if(reader)
    {
#ifdef _DEBUG_OUTPUT
    qDebug() << "onReceivedVectorPointFData-> data size:"<<datas.size()
             << " WndPtr:"<<header.getWndPtr()
             << " FigPtr:"<<header.getFigPtr()
             << " ItemPtr:"<<header.getItemPtr()
                ;
#endif
        QList<QVariant> args;
        args.append(QVariant::fromValue(header.getWndPtr()));
        args.append(QVariant::fromValue(header.getFigPtr()));
        args.append(QVariant::fromValue(header.getItemPtr()));
        args.append(QVariant::fromValue((qintptr)reader->getSocket()));
        emit callVectorPointFProcess(datas,QVariant(args));
    }
}

void SADataProcServe::onReceivedString(const QString &str)
{
    SALocalServeReader* reader  = qobject_cast<SALocalServeReader*>(sender());
    if(nullptr == reader)
    {
        return;
    }
    if(str == "__test__1m")//这是一个特殊的测试请求，接收到这个字符串后，发送1000,000个点进行计时测试
    {
        SALocalServeWriter* writer =  getWriter(reader);
        if(nullptr == writer)
        {
            return;
        }
        QVector<QPointF> points;
        for(int i=0;i<1000000;++i)
        {
            points.append(QPointF(1,1));
        }
        qDebug() << "send 1m points test";
        writer->sendDoubleVectorData(0,0,0,points);
    }
}

void SADataProcServe::onRecShakeHand()
{
    SALocalServeReader* reader  = qobject_cast<SALocalServeReader*>(sender());
    SALocalServeWriter* writer =  getWriter(reader);
    if(nullptr == writer)
    {
        return;
    }
    writer->sendShakeHand();
}

void SADataProcServe::onProcessVectorPointFResult(SADataFeatureItem *result, QVariant args)
{
    if(nullptr == result)
    {
        return;
    }
    QList<QVariant> argList = args.toList();
    if(4 != argList.size())
    {
        qDebug() << "argList invalid";
        return;
    }
    qDebug() << "start process vector pointF result";
    quintptr widget,fig,item,client;
    widget = argList[0].value<quintptr>();
    fig = argList[1].value<quintptr>();
    item = argList[2].value<quintptr>();
    client = argList[3].value<quintptr>();
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
    QLocalSocket * clientPtr = (QLocalSocket *)client;
    if(clientPtr)
    {
        SALocalServeWriter* writer = m_writerDict.value(clientPtr,nullptr);
        if(writer)
        {
            writer->sendString(xml);
        }
    }
#ifdef _DEBUG_OUTPUT
    qDebug() << "onProcessVectorPointFResult cost:" << t.elapsed()
             << " WndPtr:"<<widget
             << " FigPtr:"<<fig
             << " ItemPtr:"<<item
                ;
#endif
    delete result;
}

void SADataProcServe::onDisconnected()
{
    QLocalSocket* socket  = qobject_cast<QLocalSocket*>(sender());
    qDebug() << "LocalSocket disconnect";
    if(socket)
    {
        qDebug() <<"remove socket";
        m_connectList.remove(socket);
        m_writerDict.remove(socket);
        m_readerDict.remove(socket);
    }
}

void SADataProcServe::onLifeCheckTimeOut()
{
    if(m_connectList.size() <= 0)
    {
        if(m_willBeQuit)
        {
            qDebug() << "signADataProc Auto Quit!";
            qApp->quit();
        }
        else
        {
            m_willBeQuit = true;
        }
    }
    else
    {
        m_willBeQuit = false;
    }
}



uint SADataProcServe::getPid() const
{
    return m_pid;
}

void SADataProcServe::setPid(const uint &pid)
{
    m_pid = pid;
}

void SADataProcServe::initCalcThread()
{
    qDebug() << "init thread";
    m_calcThread = new QThread;
    m_pointFCalctor = new SADataProcessVectorPointF;
    m_pointFCalctor->moveToThread(m_calcThread);

    connect(m_calcThread,&QThread::finished,m_calcThread,&QThread::deleteLater);//内存释放
    connect(m_calcThread,&QThread::finished,m_pointFCalctor,&SADataProcessVectorPointF::deleteLater);//内存释放
    connect(m_pointFCalctor,&SADataProcessVectorPointF::result
            ,this,&SADataProcServe::onProcessVectorPointFResult);
    connect(this,&SADataProcServe::callVectorPointFProcess
            ,m_pointFCalctor,&SADataProcessVectorPointF::setPoints);
    m_calcThread->start();

}

SALocalServeWriter *SADataProcServe::getWriter(SALocalServeReader *reader)
{
    if(nullptr == reader)
    {
        return nullptr;
    }
    return m_writerDict.value(reader->getSocket(),nullptr);
}
