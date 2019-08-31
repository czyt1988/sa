#include "SADataClient.h"
#include <QLocalSocket>
#include "SALocalServeSocketClineParse.h"
#include "SALocalServerDefine.h"
#include <memory>
#include <QDebug>
#include <QDateTime>
#include <QMap>
/// \def 连接不上的计数
#define CONNECT_RETRY_COUNT (50)
/// \def 心跳丢失次数记为连接丢失
#define HEART_BREAK_COUNT_AS_DISCONNECT (10)
const uint c_test_key = 0x2312;
class SADataClientPrivate
{
public:
    SA_IMPL_PUBLIC(SADataClient)
    SADataClientPrivate(SADataClient* p);
    ~SADataClientPrivate();
    short mConnectRetryCount;///< 重连服务器次数
    std::unique_ptr<QLocalSocket> mDataProcessSocket;///< 数据处理对应的socket
    std::unique_ptr<SALocalServeSocketClineParse> mSocketOpt;///< 处理m_dataProcessSocket的具体封装
    QDateTime mLastHeartbeatTime;///< 记录心跳间隔时长
    std::unique_ptr<QDateTime> mStartSpeedTestDatetime;///< 记录开始测试的时间
    QTimer mHeartbeatChecker;///< 用于定时检测心跳
    short mLossHeartbeatCount;///<丢失心跳的次数
    //把错误码转为文字
    QString errorCodeToString(int code);
private:
    QMap<int,QString> mErrcodeToString;///< 错误码对应文本
};

SADataClientPrivate::SADataClientPrivate(SADataClient *p)
    :q_ptr(p)
    ,mConnectRetryCount(CONNECT_RETRY_COUNT)
    ,mDataProcessSocket(nullptr)
    ,mSocketOpt(nullptr)
    ,mLastHeartbeatTime(QDateTime::currentDateTime())
    ,mLossHeartbeatCount(0)
{
    mErrcodeToString[SALocalServe::Unknow] = tr("unknow");
    mErrcodeToString[SALocalServe::ReceiveDataError] = tr("Receive Data Error");
    mErrcodeToString[SALocalServe::ReceiveUnknowHeader] = tr("Receive Unknow Header");
}

SADataClientPrivate::~SADataClientPrivate()
{

}

QString SADataClientPrivate::errorCodeToString(int code)
{
    return mErrcodeToString.value(code);
}

SADataClient::SADataClient(QObject *p):QObject(p)
  ,d_ptr(new SADataClientPrivate(this))
{
    connect(&(d_ptr->mHeartbeatChecker),&QTimer::timeout
            ,this,&SADataClient::onHeartbeatCheckerTimerout);
}

/**
 * @brief 连接服务器
 */
void SADataClient::connectToServer()
{
    if(d_ptr->mDataProcessSocket)
    {
        d_ptr->mDataProcessSocket.reset();
    }
    d_ptr->mDataProcessSocket = std::make_unique<QLocalSocket>(this);
    connect(d_ptr->mDataProcessSocket,&QLocalSocket::disconnected
            ,this,&SADataClient::onLocalSocketDisconnect);
     d_ptr->mConnectRetryCount = CONNECT_RETRY_COUNT;
     tryToConnectServer();
}

void SADataClient::onLoginSucceed(int tokenID, uint key)
{
    if(nullptr == d_ptr->mSocketOpt)
        return;
    d_ptr->mSocketOpt->setToken(tokenID);
    emit messageInfo(tr("connect calc serve success"));
    d_ptr->mStartSpeedTestDatetime.reset(new QDateTime(QDateTime::currentDateTime()));
    //m_socketOpt->sendString("__test__1m",c_test_key);

}

/**
 * @brief 心跳
 * @param key
 */
void SADataClient::onRecHeartbeat(uint key)
{
    Q_UNUSED(key);
    d_ptr->mLastHeartbeatTime = QDateTime::currentDateTime();
}
/**
 * @brief 接收到字符串
 * @param str
 * @param key
 */
void SADataClient::onReceivedString(const QString &str, uint key)
{

}
/**
 * @brief 接收到点数组
 * @param arrs
 * @param key
 */
void SADataClient::onReceive2DPointFs(const QVector<QPointF> &arrs, uint key)
{
    if(c_test_key == key)
    {
        if(d_ptr->mStartSpeedTestDatetime!=nullptr)
        {
            qint64 ms = QDateTime::currentDateTime().msecsTo(*(d_ptr->mStartSpeedTestDatetime));
            ms = qAbs(ms);
            int byteSize = 16000004;
            QString msg = tr("1mil points test connect time cost:%1 ms ys.size:%2 , send speed:%3 byte/ms(%4 MB/s)")
                    .arg(ms)
                    .arg(arrs.size())
                    .arg((byteSize)/ms)
                    .arg((byteSize/(1024.0*1024)) / (ms/1000.0));
            emit messageInfo(msg);
        }
    }
}

void SADataClient::onLocalSocketDisconnect()
{
    qInfo() << tr("disconnect from serve,try to reconnect");
    QTimer::singleShot(100,this,&SADataFeatureWidget::tryToStartDataProc);
}

void SADataClient::onHeartbeatCheckerTimerout()
{
    if (0 == d_ptr->mLossHeartbeatCount)
    {
        emit messageInfo(tr("connect lost !"),SA::NormalMessage);
    }
    ++(d_ptr->mLossHeartbeatCount);
    if (d_ptr->mLossHeartbeatCount > HEART_BREAK_COUNT_AS_DISCONNECT)
    {
        //心跳多次无法到达，重新连接
        connectToServer();
        d_ptr->mLossHeartbeatCount = 0;
    }
}

void SADataClient::onErrorOccure(int errcode)
{
    QString str = QString("Error Occure,code:%1,msg:%2").arg(errcode).arg(d_ptr->errorCodeToString(errcode));
    emit messageInfo(str,SA::ErrorMessage);
}

void SADataClient::tryToConnectServer()
{
    d_ptr->mDataProcessSocket->connectToServer(SA_LOCAL_SERVER_DATA_PROC_NAME);
    if(d_ptr->mConnectRetryCount <= 0)
    {
        //一直尝试连接不成功才报此异常
        QString str = tr("can not connect dataProc Serve!%1").arg(d_ptr->mDataProcessSocket->errorString());
        emit messageInfo(str,SA::ErrorMessage);
        qInfo() << str;
        return;
    }
    if(! (d_ptr->mDataProcessSocket->waitForConnected(1000)) )
    {
        QTimer::singleShot(100,this,&SADataClient::tryToConnectServer);
        --(d_ptr->mConnectRetryCount);
        return;
    }

    qInfo() << tr("connect to dataProc serve success!");
    //建立socket解析操作类
    d_ptr->mSocketOpt = std::make_unique<SALocalServeSocketClineParse>(d_ptr->mDataProcessSocket,this);
    connect(d_ptr->mSocketOpt.get(),&SALocalServeSocketOpt::recHeartbeat
            ,this,&SADataClient::onRecHeartbeat);
    connect(d_ptr->mSocketOpt.get(),&SALocalServeSocketOpt::loginSucceed
            ,this,&SADataClient::onLoginSucceed);
    connect(d_ptr->mSocketOpt.get(),&SALocalServeSocketOpt::recString
            ,this,&SADataClient::onReceivedString);
    connect(d_ptr->mSocketOpt.get(),&SALocalServeSocketOpt::rec2DPointFs
            ,this,&SADataClient::onReceive2DPointFs);
    connect(d_ptr->mSocketOpt.get(),&SALocalServeSocketOpt::errorOccure
            ,this,&SADataClient::onErrorOccure);

}


