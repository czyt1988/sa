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

#define ARG_DES_KEY_ID "key"
#define ARG_DES_TOKEN_ID "token"
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
    static uint tokenID = 100;
    ++tokenID;
    SALocalServeWriter* writer = new SALocalServeWriter(socket,socket);
    SALocalServeProtocol* reader = new SALocalServeProtocol(socket,socket);
    writer->setToken(tokenID);
    reader->setToken(tokenID);
    m_token2readwrite[tokenID] = qMakePair(writer,reader);
    m_connectList.insert(socket);
    m_writerDict[socket] = writer;
    m_readerDict[socket] = reader;

    writer->sendLoginSucceed();
    //开始心跳检查
    writer->startHeartbeat();


    connect(reader,&SALocalServeProtocol::receive2DPointFs
            ,this,&SADataProcServe::onReceive2DPointFs);
    connect(reader,&SALocalServeProtocol::receiveString
            ,this,&SADataProcServe::onReceivedString);
    connect(reader,&SALocalServeProtocol::heartbeatTimeOut
            ,this,&SADataProcServe::onHeartbeatTimeOut);
    /*
    connect(reader,&SALocalServeReader::receivedString
            ,this,&SADataProcServe::onReceivedString);
    connect(reader,&SALocalServeReader::receivedShakeHand
            ,this,&SADataProcServe::onRecShakeHand);
    connect(socket,&QLocalSocket::disconnected,this,&SADataProcServe::onDisconnected);
    */


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

/**
 * @brief 心跳丢失
 * @param ms 丢失时间
 * @param tokenID 对应的token
 * @param key 对应的key
 */
void SADataProcServe::onHeartbeatTimeOut(int ms, int tokenID, uint key)
{
    Q_UNUSED(key);
    qDebug() << "unreach heart beat " << ms << " ms, token id:" << tokenID;
    //TODO:这里暂时不作为
}



void SADataProcServe::onReceive2DPointFs(const QVector<QPointF>& datas,uint key)
{
    SALocalServeReader* reader  = qobject_cast<SALocalServeReader*>(sender());
    if(reader)
    {
        QHash<QString, QVariant> args;
        args[ARG_DES_KEY_ID]=QVariant::fromValue(key);
        args[ARG_DES_TOKEN_ID]=QVariant::fromValue(reader->getToken());
#ifdef _DEBUG_OUTPUT
    qDebug() << "onReceivedVectorPointFData-> data size:"<<protocol.getPoints().size()
             << " wndId:"<<wndId
             << " figId:"<<figId
             << " itemId:"<<itemId
                ;
#endif
        emit callVectorPointFProcess(datas,args);
    }
}

void SADataProcServe::onReceivedString(const QString& str,uint key)
{
    SALocalServeReader* reader  = qobject_cast<SALocalServeReader*>(sender());
    if(nullptr == reader)
    {
        return;
    }
    if("__test__1m" == str)//这是一个特殊的测试请求，接收到这个字符串后，发送1000,000个点进行计时测试
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
        writer->send2DPointFs(points);
    }
}

/*
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
*/


void SADataProcServe::onProcessVectorPointFResult(const QString& res
                                                  , const QHash<QString, QVariant>& args)
{
    if(res.isEmpty())
    {
        qDebug() << "rec result is empty";
        return;
    }

    if(2 != args.size())
    {
        qDebug() << "argList invalid";
        return;
    }
    uint key = args[ARG_DES_KEY_ID].value<uint>();
    int token = args[ARG_DES_TOKEN_ID].value<int>();
    if(!m_token2readwrite.contains(token))
    {
        qDebug() << "rec unknow token:" << token;
        return;
    }
     SALocalServeWriter* writer = m_token2readwrite[token].first;
     if(nullptr == writer)
     {
         qDebug() << "unknow writer! token:" << token;
         return;
     }
#ifdef _DEBUG_OUTPUT
    QElapsedTimer t;
    t.start();
#endif
    QString xml;
    QTextStream st(&xml);
    st << "<" << SA_XML_TAG_SA << " " << SA_XML_ATT_TYPE << "=\"" <<  ATT_SA_TYPE_VPFR << "\">";
    st << "<" << SA_XML_TAG_UINTVAL << " " << SA_XML_ATT_NAME << " =\"key\" >" << key << "</" << SA_XML_TAG_UINTVAL << ">";
    st << res;
    st << "</" << SA_XML_TAG_SA << ">";
    st << endl;
    writer->sendString(xml,key);


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
        if(m_writerDict.contains(socket))
        {
            int token = m_writerDict[socket]->getToken();
            m_token2readwrite.remove(token);
        }
        m_writerDict.remove(socket);
        if(m_readerDict.contains(socket))
        {
            //虽无必要，但保证代码无异常
            int token = m_readerDict[socket]->getToken();
            m_token2readwrite.remove(token);
        }
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
