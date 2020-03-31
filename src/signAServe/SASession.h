#ifndef SASESSION_H
#define SASESSION_H
#include "SAServeGlobal.h"
#include "SATcpSocket.h"
#include "SAServeHandleFun.h"
#include <memory>
#include <QObject>
#include <functional>
class SAXMLProtocolParser;
SA_IMPL_FORWARD_DECL(SASession)
/**
 * @brief 对服务器每个socket的处理封装
 */
class SASERVE_EXPORT SASession : public QObject
{
    SA_IMPL(SASession)
    Q_OBJECT
public:
    /**
    * @brief 错误描述符
    */
    enum SectionError
    {
        UnknowError = 0,///< 未知错误
        InvalidXmlProtocol = 1, ///< 收到的是xml协议请求，但无法解析到标准xml协议
    };
     /**
     * @brief XMLDataPtr 封装的SAXMLProtocolParser智能指针
     */
    typedef std::shared_ptr<SAXMLProtocolParser> XMLDataPtr;
    //创建智能指针
    static XMLDataPtr makeXMLDataPtr();
    //socket的生命周期由section管理
    SASession(SATcpSocket* socket,QObject* par = nullptr);
    ~SASession();
    //向socket写数据
    bool write(const SAProtocolHeader& header,const QByteArray& data);
    SATcpSocket* getSocket() const;
    //处理收到的请求
    virtual bool deal(const SAProtocolHeader &header, const QByteArray &data);
    //处理xml相关请求
    virtual bool dealXmlProtocol(const SAProtocolHeader &header, XMLDataPtr xml);
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
