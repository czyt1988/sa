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
#define _DEBUG_OUTPUT
#ifdef _DEBUG_OUTPUT
#include <QElapsedTimer>
#endif
SADataProcServe::SADataProcServe(QObject *parent):QObject(parent)
  ,m_pid(0)
  ,m_localServer(new QLocalServer(this))
  ,m_willBeQuit(false)
  ,m_checkLiveTime(10000)
{
    connect(&m_liveChecker,&QTimer::timeout,this,&SADataProcServe::onCheckLive);

    qRegisterMetaType<QVector<QPointF> >("QVector<QPointF>");
    initCalcThread();
    connect(m_localServer,&QLocalServer::newConnection
            ,this,&SADataProcServe::onLocalServeNewConnection);
    if(!m_localServer->listen(SA_LOCAL_SERVER_DATA_PROC_NAME))
    {
       qDebug() << tr("listern loacl server error");
    }
    qInfo() << tr("start DataProc Serve");
    m_liveChecker.start(m_checkLiveTime);
}


void SADataProcServe::onLocalServeNewConnection()
{
    qDebug() << tr("New Connection");
    QLocalSocket* socket = m_localServer->nextPendingConnection();
    if(nullptr == socket)
    {
        qDebug() << tr("can not exec m_localServer->nextPendingConnection();");
        return;
    }
    m_connectList.insert(socket);
    static uint tokenID = 100;
    ++tokenID;
    socket->flush();
    SALocalServeSocketServeParse* sp = new SALocalServeSocketServeParse(socket,socket);
    sp->setToken(tokenID);
    m_socketOptDict[socket] = sp;
    m_tokenOptDict[tokenID] = sp;
    qDebug() << "New Connection token ID:" << tokenID;
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
    qDebug() << str;
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
        sp->send2DPointFs(points,key);
    }
}


void SADataProcServe::onProcessVectorPointFResult(const QString& res
                                                  ,const QHash<QString, QVariant>& args
                                                  , uint key)
{
    if(res.isEmpty())
    {
        qDebug() << "rec result is empty";
        return;
    }
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
             << " token:"<<token
             << " res:"<<res
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

/**
 * @brief 检查是否需要结束
 */
void SADataProcServe::onCheckLive()
{
    if(m_connectList.size() <= 0)
    {
        qDebug() << tr("no connection at %1 ms,quit signadataproc!").arg(m_checkLiveTime);
        qApp->quit();
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


