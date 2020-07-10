#ifndef SASOCKETHANDLE_H
#define SASOCKETHANDLE_H
#include "SAServeGlobal.h"
#include "SATcpSocket.h"
#include "SAServeHandleFun.h"
#include <memory>
#include <QObject>
#include <functional>
class SAXMLProtocolParser;
SA_IMPL_FORWARD_DECL(SASocketHandle)



/**
 * @brief 基本socket处理封装
 */
class SASERVE_EXPORT SAAbstractSocketHandle : public QObject
{
    Q_OBJECT
public:
    SAAbstractSocketHandle(QObject *par = nullptr);
    virtual ~SAAbstractSocketHandle();
    //设置socket
    virtual void setSocket(SATcpSocket *s) = 0;

public slots:
    //基本处理函数
    virtual void onRecSocketData(const SAProtocolHeader& header, const QByteArray& data) = 0;

    //socket断开连接触发的槽
    virtual void onSocketDisconnected() = 0;
};


/**
 * @brief 对服务器每个socket的处理封装
 */
class SASERVE_EXPORT SASocketHandle : public SAAbstractSocketHandle
{
    Q_OBJECT
    SA_IMPL(SASocketHandle)
public:

    /**
     * @brief 错误描述符
     */
    enum Error {
        ErrorUnknow		= 0,    ///< 未知错误
        ErrorInvalidXmlProtocol = 1,    ///< 收到的是xml协议请求，但无法解析到标准xml协议
    };

    /**
     * @brief XMLDataPtr 封装的SAXMLProtocolParser智能指针
     */
    typedef std::shared_ptr<SAXMLProtocolParser> XMLDataPtr;
    //创建智能指针
    static XMLDataPtr makeXMLDataPtr();

    //socket的生命周期由section管理
    SASocketHandle(QObject *par = nullptr);
    ~SASocketHandle();
    //设置socket
    virtual void setSocket(SATcpSocket *s) override;

    //向socket写数据
    void ensureWrite(const SAProtocolHeader& header, const QByteArray& data);
    void ensureWrite(const SAXMLProtocolParser& xml, int funid, int sequenceID, uint32_t extendValue);

    //获取socket
    SATcpSocket *getSocket() const;

    //处理xml相关请求
    virtual bool dealXmlProtocol(const SAProtocolHeader& header, XMLDataPtr xml);

signals:
    void error(int errid, const QString& errstring);

private slots:
    //处理收到的请求
    virtual void onRecSocketData(const SAProtocolHeader& header, const QByteArray& data) override;

    //
    virtual void onSocketDisconnected() override;
};

#endif // SATCPSECTION_H
