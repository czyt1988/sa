#ifndef SATCPCLIENT_H
#define SATCPCLIENT_H
#include "SAServeGlobal.h"
#include "SAAbstractProtocolParser.h"
#include "SAProtocolHeader.h"
#include <QAbstractSocket>
#include <QObject>
#include <memory>
#include "SAXMLProtocolParser.h"
#define __PRINT_FUNCTION_RUN_INFO    1
#if __PRINT_FUNCTION_RUN_INFO
#include <QDebug>
#ifndef FUNCTION_RUN_PRINT
#define FUNCTION_RUN_PRINT()							\
    do{									\
        qDebug() << " >> fun:" << __FUNCTION__ << " line:" << __LINE__;	\
    }while(0)
#endif
#else
#define FUNCTION_RUN_PRINT()
#endif

class SATcpClientPrivate;
class SAXMLProtocolParser;
class SATcpSocket;

/**
 * @brief 客户端的封装
 *
 * 此客户封装了最基本的token请求和心跳检测
 *
 * 具体的客户端需要继承此类，如SATcpDataProcessClient是专门进行数据处理用的客户端
 */
class SASERVE_EXPORT SATcpClient : public QObject
{
    SA_IMPL(SATcpClient)
    Q_OBJECT
public:
    //socket工厂的函数指针
    typedef std::function<SATcpSocket *()> FunPtrSocketFactory;
    enum ClientError {
        UnknowError			= 0     ///< 未知错误
        , SharedMemoryNotReadyError	= 1     ///< 共享内存还未准备好
        , SharedMemoryGetPortError	= 2     ///< 从共享内存获取的port不正确
        , ConnectTimeout		= 3     ///< 连接服务器超时
        , InvalidXmlProtocol		= 4     ///< 收到的是xml协议请求，但无法解析到标准xml协议
        , MaxSATcpClientError		= 20    ///< SATcpClient对应的最大错误值

        , UserDefineError		= 9000  ///< 用户自定义错误
    };

    SATcpClient(QObject *par = nullptr);
    ~SATcpClient();
    //获取socket
    SATcpSocket *getSocket() const;

    //注册socket工厂
    void registSocketFactory(FunPtrSocketFactory fp);

public slots:
    //连接服务器
    void connectToServe(int timeout = 5000);

    //断开和服务器的连接
    void disconnectFromServe();

    //心跳检测设置 - 默认为false，30秒
    void setHeartbreakCheck(bool enable);

    //请求token
    void requestToken(int pid, const QString& appid);

    //主动请求心跳,服务器收到会立马回复一个心跳协议
    void requestHeartbreat();

    //连接和断开的槽，本例方法未做任何事情
    void onSocketConnected();

    //关闭客户端
    void close();

private slots:
    void onHeartbreatCheckTimeout();

    //获取到心跳应答
    void onReceivedHeartbreat();

private:
    void createSocket();
    void destroySocket();
    void connectSocket();
    void disconnectSocket();

signals:

    /**
     * @brief 客户端的错误
     * @note 默认的错误见 @see SATcpSocket::ClientError
     */
    void clientError(int errcode);

    /**
     * @brief 返回请求的token
     * @param token
     * @see SATcpClient::requestToken
     */
    void receiveToken(const QString& token, int sequenceID);

    /**
     * @brief socket错误
     * @param socketError
     */
    void socketError(QAbstractSocket::SocketError socketError);

    /**
     * @brief socket连接成功的信号
     */
    void connectedServe(QAbstractSocket *socket);

    /**
     * @brief socket连接丢失的信号
     */
    void disconnectedServe();

    /**
     * @brief 心跳超时
     * @param lastDatetime 上次心跳的时间
     */
    void heartbreatTimeout(const QDateTime& lastDatetime);

    /**
     * @brief 客户端结束
     */
    void aboutToClose();
};

#endif // SATCPCLIENT_H
