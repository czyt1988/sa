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

#define ARG_DES_WND_ID "widgetId"
#define ARG_DES_FIG_ID "figureId"
#define ARG_DES_ITEM_ID "itemId"
#define ARG_DES_SOCKET_DES "socketDes"

//#define _DEBUG_OUTPUT
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

void SADataProcServe::errorOccurred(QLocalSocket::LocalSocketError err)
{
    Q_UNUSED(err);
    QLocalSocket* socket  = qobject_cast<QLocalSocket*>(sender());
    if(socket)
    {
        qDebug() << socket->errorString();
    }
}

void SADataProcServe::onReceivedVectorPointFData(const SALocalServeVectorPointProtocol& protocol)
{
    SALocalServeReader* reader  = qobject_cast<SALocalServeReader*>(sender());
    if(reader)
    {
        if(!protocol.isValid())
        {
            qDebug() << "rec VectorPointProtocol but invalid!";
            return;
        }
        QHash<QString, QVariant> args;
        qintptr wndId,figId,itemId;
        protocol.getIDs(wndId,figId,itemId);
        args[ARG_DES_WND_ID]=QVariant::fromValue(wndId);
        args[ARG_DES_FIG_ID]=QVariant::fromValue(figId);
        args[ARG_DES_ITEM_ID]=QVariant::fromValue(itemId);
        args[ARG_DES_SOCKET_DES]=QVariant::fromValue((quintptr)reader->getSocket());
#ifdef _DEBUG_OUTPUT
    qDebug() << "onReceivedVectorPointFData-> data size:"<<protocol.getPoints().size()
             << " wndId:"<<wndId
             << " figId:"<<figId
             << " itemId:"<<itemId
                ;
#endif
        emit callVectorPointFProcess(protocol.getPoints(),args);
    }
}

void SADataProcServe::onReceivedString(const SALocalServeStringProtocol& protocol)
{
    SALocalServeReader* reader  = qobject_cast<SALocalServeReader*>(sender());
    if(nullptr == reader)
    {
        return;
    }
    if("__test__1m" == protocol.string())//这是一个特殊的测试请求，接收到这个字符串后，发送1000,000个点进行计时测试
    {
        qDebug() << "receive __test__1m : start 1m points test";
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

void SADataProcServe::onRecShakeHand(const SALocalServeShakeHandProtocol& protocol)
{
    if(!protocol.isValid())
    {
        qDebug() << "rec ShakeHand but invalid!";
        return;
    }
    SALocalServeReader* reader  = qobject_cast<SALocalServeReader*>(sender());
    SALocalServeWriter* writer =  getWriter(reader);
    if(nullptr == writer)
    {
        return;
    }
    writer->sendShakeHand();
}

void SADataProcServe::onProcessVectorPointFResult(const QString& res
                                                  , const QHash<QString, QVariant>& args)
{
    if(res.isEmpty())
    {
        return;
    }

    if(4 != args.size())
    {
        qDebug() << "argList invalid";
        return;
    }
    quintptr widget,fig,item,client;
    widget = args[ARG_DES_WND_ID].value<quintptr>();
    fig = args[ARG_DES_FIG_ID].value<quintptr>();
    item = args[ARG_DES_ITEM_ID].value<quintptr>();
    client = args[ARG_DES_SOCKET_DES].value<quintptr>();
#ifdef _DEBUG_OUTPUT
    QElapsedTimer t;
    t.start();
#endif
    QString xml;
    QTextStream st(&xml);
    st << "<" << SA_XML_TAG_SA << " " << SA_XML_ATT_TYPE << "=\"" <<  ATT_SA_TYPE_VPFR << "\">";
    st << "<" << SA_XML_TAG_QUINTPTR << ">" << widget << "</" << SA_XML_TAG_QUINTPTR << ">";
    st << "<" << SA_XML_TAG_QUINTPTR << ">" << fig << "</" << SA_XML_TAG_QUINTPTR << ">";
    st << "<" << SA_XML_TAG_QUINTPTR << ">" << item << "</" << SA_XML_TAG_QUINTPTR << ">";
    st << res;
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



SALocalServeWriter *SADataProcServe::getWriter(SALocalServeReader *reader)
{
    if(nullptr == reader)
    {
        return nullptr;
    }
    return m_writerDict.value(reader->getSocket(),nullptr);
}
