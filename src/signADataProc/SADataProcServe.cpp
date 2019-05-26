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
    SALocalServeSocketServeParse* sp = new SALocalServeSocketServeParse(socket,socket);
    sp->setToken(tokenID);
    m_socketOptDict[socket] = sp;
    m_tokenOptDict[tokenID] = sp;
    //发送登录成功，把token发生到接收端
    sp->sendLoginSucceed();
    //开始心跳检查
    sp->startAutoHeartbeat();


    connect(sp,&SALocalServeSocketServeParse::rec2DPointFs
            ,this,&SADataProcServe::onReceive2DPointFs);
    connect(sp,&SALocalServeSocketServeParse::recString
            ,this,&SADataProcServe::onReceivedString);
    connect(socket,static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error)
            ,this,&SADataProcServe::errorOccurred);
    connect(socket,&QLocalSocket::disconnected
            ,this,&SADataProcServe::onDisconnected);
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





void SADataProcServe::onReceive2DPointFs(const QVector<QPointF>& datas,uint key)
{
    SALocalServeSocketServeParse* sp = qobject_cast<SALocalServeSocketServeParse*>(sender());
    if(sp)
    {
        QHash<QString, QVariant> args;
        args[ARG_DES_KEY_ID]=QVariant::fromValue(key);
        args[ARG_DES_TOKEN_ID]=QVariant::fromValue(sp->getToken());
#ifdef _DEBUG_OUTPUT
    qDebug() << "onReceivedVectorPointFData-> data size:"<<datas.size()
             << " args:"<<args
             << " key:"<<key
                ;
#endif
        emit callVectorPointFProcess(datas,args,key);
    }
}

void SADataProcServe::onReceivedString(const QString& str,uint key)
{
    SALocalServeSocketServeParse* sp = qobject_cast<SALocalServeSocketServeParse*>(sender());
    if(nullptr == sp)
    {
        return;
    }
    if("__test__1m" == str)//这是一个特殊的测试请求，接收到这个字符串后，发送1000,000个点进行计时测试
    {
        qDebug() << "receive __test__1m : start 1m points test";
        QVector<QPointF> points;
        for(int i=0;i<1000000;++i)
        {
            points.append(QPointF(1,1));
        }
        qDebug() << "send 1m points test";
        sp->send2DPointFs(points);
    }
}


void SADataProcServe::onProcessVectorPointFResult(const QString& res
                                                  , uint key)
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
    SALocalServeSocketServeParse* sp = m_tokenOptDict.value(token,nullptr);
    if(nullptr == sp)
    {
        qDebug() << "rec unknow token:" << token;
        return;
    }

#ifdef _DEBUG_OUTPUT
    QElapsedTimer t;
    t.start();
#endif
    sp->sendString(res,key);
    qDebug() << res;

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
        if(m_socketOptDict.contains(socket))
        {
            int token = m_socketOptDict[socket]->getToken();
            m_tokenOptDict.remove(token);
            m_socketOptDict.remove(socket);
        }
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


