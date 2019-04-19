#include "SALocalServeProtocol.h"
#include <QTextCodec>
#include <QBuffer>
#include <QDebug>
#include "SALocalServerDefine.h"

/*
SALocalServeBaseProtocol::SALocalServeBaseProtocol()
{

}

SALocalServeBaseProtocol::~SALocalServeBaseProtocol()
{

}

bool SALocalServeBaseProtocol::isValid() const
{
    return m_header.isValid();
}

SALocalServeBaseHeader &SALocalServeBaseProtocol::header()
{
    return m_header;
}

const SALocalServeBaseHeader &SALocalServeBaseProtocol::header() const
{
    return m_header;
}

//
//========================================================
//

SALocalServeShakeHandProtocol::SALocalServeShakeHandProtocol():SALocalServeBaseProtocol()
{
    memset(m_raw,0,sizeof(unsigned char)*SA_PROTOCOL_SHAKE_HAND_DATA_LEN);
}

void SALocalServeShakeHandProtocol::serializeIn(QDataStream &st)
{
    //TODO 考虑加校验位
    st >> m_header;
    st.readRawData((char*)m_raw,SA_PROTOCOL_SHAKE_HAND_DATA_LEN);
}

void SALocalServeShakeHandProtocol::serializeOut(QDataStream &st) const
{
    SALocalServeBaseHeader header;
    header.init();
    header.type = (int)TypeShakeHand;
    header.dataSize = SA_PROTOCOL_SHAKE_HAND_DATA_LEN;
    st << header;
    st.writeRawData((char*)m_raw,header.dataSize);
}


SALocalServeBaseProtocol::Type SALocalServeShakeHandProtocol::type() const
{
    return TypeShakeHand;
}

//===================
SALocalServeStringProtocol::SALocalServeStringProtocol():SALocalServeBaseProtocol()
{

}

void SALocalServeStringProtocol::serializeIn(QDataStream &st)
{
    st >> m_header;
    QByteArray encodedString;
    encodedString.resize(m_header.dataSize);
    st.readRawData(encodedString.data(),m_header.dataSize);
    m_str = QString::fromUtf8(encodedString);

//    qDebug() << "SALocalServeStringProtocol::serializeInData:encodedString byte size"<<encodedString.size()
//             << " hex:" << QString(encodedString.toHex())
//             <<" fromUtf8:"<<m_str

//               ;
}

void SALocalServeStringProtocol::serializeOut(QDataStream &st) const
{
    QByteArray encodedString = m_str.toUtf8();
    SALocalServeBaseHeader header;
    header.init();
    header.type = (int)TypeString;
    header.dataSize = encodedString.size();
    st << header;
    st.writeRawData(encodedString.data(),header.dataSize);
}


SALocalServeBaseProtocol::Type SALocalServeStringProtocol::type() const
{
    return TypeString;
}

const QString &SALocalServeStringProtocol::string() const
{
    return m_str;
}

QString &SALocalServeStringProtocol::string()
{
    return m_str;
}

void SALocalServeStringProtocol::setString(const QString &str)
{
    m_str = str;
}

//===================

SALocalServeVectorPointProtocol::SALocalServeVectorPointProtocol():SALocalServeBaseProtocol()
  ,m_winId(0)
  ,m_figId(0)
  ,m_itemId(0)
{

}

void SALocalServeVectorPointProtocol::serializeIn(QDataStream &st)
{
    st >> m_header;
    QByteArray byteArray;
    byteArray.resize(m_header.dataSize);
    st.readRawData(byteArray.data(),m_header.dataSize);
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::ReadOnly);
    QDataStream io(&buffer);
    io >> m_points >> m_winId >> m_figId >> m_itemId;
}

void SALocalServeVectorPointProtocol::serializeOut(QDataStream &st) const
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    QDataStream io(&buffer);
    io << m_points << m_winId << m_figId << m_itemId;
    SALocalServeBaseHeader header;
    header.init();
    header.type = (int)TypeVectorPointFData;
    header.dataSize = byteArray.size();
    st << header;
    st.writeRawData(byteArray.data(),header.dataSize);
}


SALocalServeBaseProtocol::Type SALocalServeVectorPointProtocol::type() const
{
    return TypeVectorPointFData;
}

void SALocalServeVectorPointProtocol::setPoints(const QVector<QPointF> &points)
{
    m_points = points;
}

const QVector<QPointF> &SALocalServeVectorPointProtocol::getPoints() const
{
    return m_points;
}

QVector<QPointF> &SALocalServeVectorPointProtocol::getPoints()
{
    return m_points;
}

void SALocalServeVectorPointProtocol::setIDs(qintptr wndId, qintptr figId, qintptr itemId)
{
    m_winId = wndId;
    m_figId = figId;
    m_itemId = itemId;
}

void SALocalServeVectorPointProtocol::getIDs(qintptr &wndId, qintptr &figId, qintptr &itemId) const
{
    wndId = m_winId;
    figId = m_figId;
    itemId = m_itemId;
}

//===================
QDataStream &operator <<(QDataStream &st, const SALocalServeShakeHandProtocol &d)
{
    d.serializeOut(st);
    return st;
}

QDataStream &operator >>(QDataStream &st, SALocalServeShakeHandProtocol &d)
{
    d.serializeIn(st);
    return st;
}


QDataStream &operator <<(QDataStream &st, const SALocalServeStringProtocol &d)
{
    d.serializeOut(st);
    return st;
}

QDataStream &operator >>(QDataStream &st, SALocalServeStringProtocol &d)
{
    d.serializeIn(st);
    return st;
}


QDataStream &operator <<(QDataStream &st, const SALocalServeVectorPointProtocol &d)
{
    d.serializeOut(st);
    return st;
}

QDataStream &operator >>(QDataStream &st, SALocalServeVectorPointProtocol &d)
{
    d.serializeIn(st);
    return st;
}


*/

SALocalServeProtocol::SALocalServeProtocol(QLocalSocket *localSocket, QObject *parent)
    :SALocalServeReader(localSocket,parent)
    ,m_heartbeatCheckTime(5000)
    ,m_maxHeartbeatSpanTime(10000)
    ,m_maxHeartbeatMissTimes(8)
    ,m_missHeartbeatCount(0)
    ,m_haveRecHeartbeat(false)
    ,m_heartBreakKey(0)
    ,m_tokenID(0)
{
    connect(this,&SALocalServeReader::receive,this,&SALocalServeProtocol::onReceive);
    connect(&m_timer,&QTimer::timeout,this,&SALocalServeProtocol::onHeartbeatCheckTimeOut);
}

SALocalServeProtocol::SALocalServeProtocol(QObject *parent)
    :SALocalServeReader(parent)
    ,m_heartbeatCheckTime(5000)
    ,m_maxHeartbeatSpanTime(10000)
    ,m_maxHeartbeatMissTimes(8)
    ,m_missHeartbeatCount(0)
    ,m_haveRecHeartbeat(false)
    ,m_heartBreakKey(0)
    ,m_tokenID(0)
{
    connect(this,&SALocalServeReader::receive,this,&SALocalServeProtocol::onReceive);
    connect(&m_timer,&QTimer::timeout,this,&SALocalServeProtocol::onHeartbeatCheckTimeOut);
}

/**
 * @brief 处理接收的数据
 * @param head 文件头
 * @param datas 数据
 */
void SALocalServeProtocol::onReceive(const SALocalServeBaseHeader &head, const QByteArray &datas)
{
    if(!head.isValid())
        return;
    int classID = head.classID;
    int funID = head.functionID;
    uint key = head.key;
    switch (classID) {
    case SA_LOCAL_SER_LOGIN_CLASS:
        {
        switch (funID) {
        case SA_LOCAL_SER_LOGIN_FUN:
            //1-1
            dealLoginSucceed(datas,key);
            break;
        default:
            break;
        }
        }
    case SA_LOCAL_SER_SHAKEHAND_CLASS:
        switch (funID) {
        case SA_LOCAL_SER_SHAKEHAND_FUN:
            //2-1
            dealHeartbeat(key);
            break;
        default:
            break;
        }
        break;
    case SA_LOCAL_SER_PARAM_CLASS:
        switch (funID) {
        case SA_LOCAL_SER_STRING_FUN:
            dealString(datas,key);
            break;
        case SA_LOCAL_SER_2DPOINTF_FUN:
            //3-2
            deal2DPointFs(datas,key);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/**
 * @brief 定时检查握手的情况
 */
void SALocalServeProtocol::onHeartbeatCheckTimeOut()
{
    if(m_heartbeatTick.elapsed() > m_maxHeartbeatSpanTime)
    {
        ++m_missHeartbeatCount;
        if(m_missHeartbeatCount > getMaxHeartbeatMissTimes())
        {
            emit heartbeatTimeOut(m_missHeartbeatCount - getMaxHeartbeatMissTimes(),m_tokenID,m_heartBreakKey);
        }
    }
    else
    {
        m_missHeartbeatCount = 0;
    }
}

/**
 * @brief 处理登录成功
 * @param datas
 */
void SALocalServeProtocol::dealLoginSucceed(const QByteArray &datas, uint key)
{
    m_tokenID = 0;
    QDataStream st(datas);
    st >> m_tokenID;
    emit loginSucceed(m_tokenID,key);
}

/**
 * @brief 处理握手
 */
void SALocalServeProtocol::dealHeartbeat(uint key)
{
    m_heartBreakKey = key;
    if(!m_haveRecHeartbeat)
    {
        m_haveRecHeartbeat = !m_haveRecHeartbeat;
        m_timer.start();
    }
    m_heartbeatTick.restart();
}

void SALocalServeProtocol::stopHeartbeatStatusAndTimer()
{
    m_heartbeatTick.restart();
    m_timer.stop();
    m_haveRecHeartbeat = false;
}

/**
 * @brief 对二维序列进行统计
 * @param datas 数据集
 */
void SALocalServeProtocol::deal2DPointFs(const QByteArray &datas,uint key)
{
    QVector<QPointF> arrs;
    QDataStream io(datas);
    io >> arrs;
    emit receive2DPointFs(arrs,key);
}

/**
 * @brief 处理9-1
 * @param datas 数据
 * @param key 标识
 */
void SALocalServeProtocol::dealString(const QByteArray &datas, uint key)
{
    QString str;
    QDataStream io(datas);
    io >> str;
    emit receiveString(str,key);
}

int SALocalServeProtocol::getHeartbeatCheckTime() const
{
    return m_heartbeatCheckTime;
}

void SALocalServeProtocol::setHeartbeatCheckTime(int shakeHandCheckTime)
{
    m_heartbeatCheckTime = shakeHandCheckTime;
}

/**
 * @brief 停止握手检查，在接收到shakeHandTimeOut后应该调用此函数停止握手检查，否则会一直触发此信号
 */
void SALocalServeProtocol::stopHeartbeatCheck()
{
    stopHeartbeatStatusAndTimer();
}

int SALocalServeProtocol::getMaxHeartbeatMissTimes() const
{
    return m_maxHeartbeatMissTimes;
}

void SALocalServeProtocol::setMaxHeartbeatMissTimes(int maxShakeHandMissTimes)
{
    m_maxHeartbeatMissTimes = maxShakeHandMissTimes;
}

int SALocalServeProtocol::getMaxHeartbeatSpanTime() const
{
    return m_maxHeartbeatSpanTime;
}

void SALocalServeProtocol::setMaxHeartbeatSpanTime(int maxShakeHandSpanTime)
{
    m_maxHeartbeatSpanTime = maxShakeHandSpanTime;
}
