#ifndef SATCPSECTION_H
#define SATCPSECTION_H
#include "SAServeGlobal.h"
#include "SATcpSocket.h"
#include "SAServeHandleFun.h"
#include <QObject>
#include <functional>
class SAXMLProtocolParser;
SA_IMPL_FORWARD_DECL(SATcpSection)
/**
 * @brief 对服务器每个socket的处理封装
 */
class SASERVE_EXPORT SATcpSection : public QObject
{
    SA_IMPL(SATcpSection)
    Q_OBJECT
public:
    enum SectionError
    {
        UnknowError = 0,///< 未知错误
        InvalidXmlProtocol = 1, ///< 收到的是xml协议请求，但无法解析到标准xml协议
    };
    //socket的生命周期由section管理
    SATcpSection(SATcpSocket* getSocket,QObject* par = nullptr);
    ~SATcpSection();
    //向socket写数据
    bool write(const SAProtocolHeader& header,const QByteArray& data);
    SATcpSocket* getSocket() const;
    //处理收到的请求
    virtual bool deal(const SAProtocolHeader &header, const QByteArray &data);
    //处理xml相关请求
    virtual bool dealXmlProtocol(const SAProtocolHeader &header, SAXMLProtocolParser* xml);
signals:
    void socketDisconnected();
    /**
     * @brief section在解析等其他操作过程发生错误
     * @param errid 错误id
     * @param errstring 错误描述
     */
    void sectionError(int errid,const QString& errstring);
private slots:
    void onReceivedSocketData(const SAProtocolHeader &header, const QByteArray &data);
    void onSocketDisconnected();
};

#endif // SATCPSECTION_H
