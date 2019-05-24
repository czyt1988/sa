#include "SALocalServe.h"
#include <QLocalSocket>
#include "SALocalServeBaseHeader.h"
#include <QTextCodec>
#include <QBuffer>

class SALocalServePrivate{
    SA_IMPL_PUBLIC(SALocalServe)
public:
    SALocalServePrivate(SALocalServe* p)
        :q_ptr(p)
        ,m_socket(nullptr)
        ,m_token(0)
        ,m_autoHeartBreakTimer(nullptr)
        ,m_isReadedMainHeader(false)
        ,m_index(0)
    {
    }
    QLocalSocket* m_socket;
    int m_token;
    QTimer* m_autoHeartBreakTimer;

    SALocalServeBaseHeader m_mainHeader;///< 当前的主包头
    bool m_isReadedMainHeader;///< 标记是否读取了包头
    QByteArray m_buffer;
    int m_index;
    int m_token;
};

SALocalServe::SALocalServe(QLocalSocket *localSocket, QObject *parent):QObject(parent)
    ,d_ptr(new SALocalServePrivate(this))
{
    setSocket(localSocket);
}

SALocalServe::SALocalServe(QObject *parent):QObject(parent)
    ,d_ptr(new SALocalServePrivate(this))
{

}

QLocalSocket *SALocalServe::getSocket() const
{
    return d_ptr->m_socket;
}

void SALocalServe::setSocket(QLocalSocket *socket)
{
    if(d_ptr->m_socket)
    {
        disconnect(d_ptr->m_socket,&QLocalSocket::readyRead,this,&SALocalServe::onReadyRead);
    }
    d_ptr->m_socket = socket;
    if(socket)
    {
        connect(socket,&QLocalSocket::readyRead,this,&SALocalServe::onReadyRead);
        connect(socket,&QLocalSocket::disconnected,this,&SALocalServe::onDisconnected);
        connect(socket,static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error)
                ,this,&SALocalServe::onError);
    }
}

int SALocalServe::getToken() const
{
    return d_ptr->m_token;
}

/**
 * @brief 是否可写
 * @return
 */
bool SALocalServe::isEnableToWrite() const
{
    if(nullptr == d_ptr->m_socket)
    {
        return false;
    }
    if(!(d_ptr->m_socket->isValid()))
    {
        return false;
    }
    if(!(d_ptr->m_socket->isWritable()))
    {
        return false;
    }
    return true;
}

void SALocalServe::setToken(uint token)
{
    d_ptr->m_token = token;
}

/**
 * @brief 从socket中安全的读取文件
 * @param p 指针
 * @param n 读取的长度
 * @return
 */
bool SALocalServe::readFromSocket(void *p, int n)
{
    int readLen = 0,index = 0;
    bool ret = false;
    do
    {
        readLen = d_ptr->m_socket->read((char*)p+index,n);
        if(readLen <= 0)
        {
            break;
        }
        index += readLen;
        n -= readLen;
    }while( readLen && n );
    if(0 == n)
    {
        ret = true;
    }
    return ret;
}

/**
 * @brief 根据类型分发协议
 * @param head 头描述
 * @param datas 数据
 */
void SALocalServe::deal(const SALocalServeBaseHeader &head, const QByteArray &datas)
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
 * @brief 开始定时握手
 * @param freTime 握手频率
 */
void SALocalServe::startAutoHeartbeat(int freTime)
{
    if(nullptr == d_ptr->m_autoHeartBreakTimer)
    {
        d_ptr->m_autoHeartBreakTimer = new QTimer(this);
        connect(&(d_ptr->m_autoHeartBreakTimer),&QTimer::timeout,this,&SALocalServe::onAutoHeartbeatTimeout);
    }
    d_ptr->m_autoHeartBreakTimer.start(freTime);
}

/**
 * @brief 发送内容
 * @param header 头的文件大小必须设置
 * @param data 数据区
 */
void SALocalServe::send(const SALocalServeBaseHeader &header, const QByteArray &data)
{
    if(!isEnableToWrite())
    {
        return;
    }
    QDataStream out(d_ptr->m_socket);
    out << header;
    if(data.size()>0)
    {
        out << data;
    }
    d_ptr->m_socket->waitForBytesWritten();
}
/**
 * @brief 登录(1-1)，成功会返回tokenID
 * @param key
 */
void SALocalServe::sendLoginSucceed(uint key)
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
/**
 * @brief 发送握手协议(2-1)
 * @param key 标识号，会随结果返回，用于标识
 */
void SALocalServe::sendHeartbeat(uint key)
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
/**
 * @brief 发送线性数组数据，应用于绘图程序绘制完图片后把绘图的点下发给处理进程进行进一步的数据分析
 * @param datas 数据
 * @param key 标识号，会随结果返回，用于标识
 */
void SALocalServe::send2DPointFs(const QVector<QPointF> &datas, uint key)
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
}
/**
 * @brief 发送字符串
 * @param str 字符串
 * @param key 标识号，会随结果返回，用于标识
 */
void SALocalServe::sendString(const QString &str, uint key)
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
}
/**
 * @brief 发送错误
 * @param errCode 错误代码
 * @param key 标识号，会随结果返回，用于标识
 */
void SALocalServe::sendError(const int errCode, uint key)
{
    SALocalServeBaseHeader h;
    h.init();
    h.type = SA_LOCAL_SER_ERROR_TYPE;
    h.classID = SA_LOCAL_SER_ERROR_CLASS;
    h.functionID = SA_LOCAL_SER_ERROR_NORMAL_FUN;
    h.tokenID = getToken();
    h.key = key;
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    QDataStream io(&buffer);
    io << errCode;
    h.dataSize = byteArray.size();
    send(h,byteArray);
}
/**
 * @brief 处理登录成功
 * @param datas
 */
void SALocalServe::dealLoginSucceed(const QByteArray &datas, uint key)
{
    d_ptr->m_tokenID = 0;
    QDataStream st(datas);
    st >> (d_ptr->m_tokenID);
    emit loginSucceed(d_ptr->m_tokenID,key);
}



/**
 * @brief 处理心跳
 */
void SALocalServe::dealHeartbeat(uint key)
{
    emit recHeartbeat(key);
}
/**
 * @brief 对二维序列进行统计
 * @param datas 数据集
 */
void SALocalServe::deal2DPointFs(const QByteArray &datas, uint key)
{
    QVector<QPointF> arrs;
    QDataStream io(datas);
    io >> arrs;
    emit rec2DPointFs(arrs,key);
}
/**
 * @brief 处理9-1
 * @param datas 数据
 * @param key 标识
 */
void SALocalServe::dealString(const QByteArray &datas, uint key)
{
    QString str;
    QDataStream io(datas);
    io >> str;
    emit recString(str,key);
}



void SALocalServe::onAutoHeartbeatTimeout()
{
    sendHeartbeat();
}

void SALocalServe::onReadyRead()
{
    const static unsigned int s_headerSize = sizeof(SALocalServeBaseHeader);
    if(d_ptr->m_isReadedMainHeader)
    {
        if(d_ptr->m_socket->bytesAvailable() >= m_mainHeader.dataSize)
        {
            //说明数据接收完
#ifdef _DEBUG_PRINT
            qDebug() << "rec Data:"<<m_socket->bytesAvailable()
                     << " bytes \r\n header.getDataSize:"<<m_mainHeader.dataSize
                     << "\r\n header.type:"<<m_mainHeader.type
                     << "current index:" << m_index
                        ;
#endif
            if(d_ptr->m_buffer.size() < s_headerSize+d_ptr->m_mainHeader.dataSize)
            {
                d_ptr->m_buffer.resize(s_headerSize+d_ptr->m_mainHeader.dataSize);
            }
            if(!readFromSocket(d_ptr->m_buffer.data()+d_ptr->m_index,d_ptr->m_mainHeader.dataSize))
            {
                emit errorOccure(SALocalServe::ReceiveDataError);
                d_ptr->m_isReadedMainHeader = false;
                d_ptr->m_socket->reset();
                qDebug() << __FILE__ <<"[" << __FUNCTION__ << "][" << __LINE__ << "]can not read from socket io!";
                return;
            }
            d_ptr->m_index += d_ptr->m_mainHeader.dataSize;
            deal(d_ptr->m_mainHeader,d_ptr->m_buffer);
            d_ptr->m_isReadedMainHeader = false;
            if(d_ptr->m_socket->bytesAvailable() >= s_headerSize)
            {
                onReadyRead();
            }
        }
    }
    else if(d_ptr->m_socket->bytesAvailable() >= s_headerSize)
    {
        //说明包头数据接收完
#ifdef _DEBUG_PRINT
    qDebug() <<"main header may receive:"
            << "\r\n byte available:"<<m_socket->bytesAvailable()
                ;
#endif
        d_ptr->m_index = 0;
        if(d_ptr->m_buffer.size() < s_headerSize)
        {
            d_ptr->m_buffer.resize(s_headerSize);
        }
        if(!readFromSocket(d_ptr->m_buffer.data(),s_headerSize))
        {
            emit errorOccure(SALocalServe::ReceiveUnknowHeader);
            d_ptr->m_isReadedMainHeader = false;
            d_ptr->m_socket->reset();
            return;
        }
        d_ptr->m_index += s_headerSize;
        QDataStream io(d_ptr->m_buffer);
        io >> d_ptr->m_mainHeader;
        if(!(d_ptr->m_mainHeader.isValid()))
        {
            d_ptr->m_isReadedMainHeader = false;
            qDebug() << "receive unknow header";
            emit errorOccure(SALocalServe::ReceiveUnknowHeader);
            d_ptr->m_socket->reset();
            return;
        }
#ifdef _DEBUG_PRINT
    qDebug() << "header.getDataSize:"<<m_mainHeader.dataSize
             << "\r\n header.key:"<<m_mainHeader.key
             << "\r\n header.type:"<<m_mainHeader.type
             << "\r\n byte available:"<<m_socket->bytesAvailable()
                ;
#endif
        if(d_ptr->m_mainHeader.dataSize > 0)
        {
            //说明文件头之后有数据
            d_ptr->m_isReadedMainHeader = true;
        }
        else
        {
            //说明文件头之后无数据
#ifdef _DEBUG_PRINT
            qDebug() << "!!!!mainHeader dataSize == 0: type:"<<m_mainHeader.type
                            ;
#endif
            deal(d_ptr->m_mainHeader,d_ptr->m_buffer);
            d_ptr->m_isReadedMainHeader = false;
        }

        if(d_ptr->m_socket->bytesAvailable() > 0)
        {
            onReadyRead();
        }
    }
}

void SALocalServe::onDisconnected()
{
    emit disconnectFromServe();
}

void SALocalServe::onError(QLocalSocket::LocalSocketError socketError)
{
    qDebug() << __FILE__<<":"<<__FUNCTION__ << "err code:"<<(int)socketError;
    qDebug() << __FILE__<<":"<<__FUNCTION__ << m_socket->errorString();
}
