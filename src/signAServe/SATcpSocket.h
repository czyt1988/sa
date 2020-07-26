#ifndef SATCPSOCKET_H
#define SATCPSOCKET_H
#include <QTcpSocket>
#include "SAServeGlobal.h"
#include "SAProtocolHeader.h"
#include "SAXMLProtocolParser.h"
#include <memory>
class SATcpSocketPrivate;
class SAAbstractSocketHandle;

/**
 * @brief 针对sa tcp协议的socket封装
 */
class SASERVE_EXPORT SATcpSocket : public QTcpSocket
{
    SA_IMPL(SATcpSocket)
    Q_OBJECT
public:

    /**
     * @brief 错误描述符
     */
    enum ErrorSA {
        ErrorUnknow		= 0,    ///< 未知错误
        ErrorInvalidXmlProtocol = 1,    ///< 收到的是xml协议请求，但无法解析到标准xml协议
    };
    SATcpSocket(QObject *par = nullptr);
    ~SATcpSocket();
    //从socket读数据
    bool readFromSocket(void *p, int n);

signals:

    /**
     * @brief 接收到心跳
     * @param header 包头
     */
    void receivedHeartbreat(const SAProtocolHeader& header);

    /**
     * @brief 返回请求的token
     * @param token
     * @see SATcpClient::requestToken
     */
    void receiveToken(const QString& token, int sequenceID);

    /**
     * @brief 接收到对方回复的错误
     * @param token
     * @param sequenceID
     */
    void receiveError(const QString& msg, int errcode, int sequenceID);

    /**
     * @brief 错误
     * @param errid
     * @param errstring
     */
    void error_sa(int errid, const QString& errstring);

public slots:
    //带重试的写socket
    void ensureWrite(const QByteArray& data);

    //带重试的写socket
    void ensureWrite(const SAProtocolHeader& header, const QByteArray& data);

    //写xml数据
    void ensureWrite(const SAXMLProtocolParser& xml, int funid, int sequenceID, uint32_t extendValue);

    //请求心跳
    void requestHeartbreat();

    //发出token请求
    void requestToken(int pid, const QString& appid);

    //回复错误给对方
    void replyError(int sequenceID, int extendValue, const QString& msg, int errcode);

    //回复错误给对方
    void replyError(const SAProtocolHeader& requestHeader, const QString& msg, int errcode);

private slots:
    void onReadyRead();

protected:
    //读取完整的数据后调用，此类会发射receivedData信号
    virtual bool deal(const SAProtocolHeader& header, const QByteArray& data);

    //处理xml相关请求
    virtual bool dealXmlProtocol(const SAProtocolHeader& header, const SAXMLProtocolParser& xml);
};

#endif // SATCPSOCKET_H
