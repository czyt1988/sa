#ifndef SATCPCLIENT_H
#define SATCPCLIENT_H
#include "SAServeGlobal.h"
#include "SAAbstractProtocolParser.h"
#include "SAProtocolHeader.h"

#include <QObject>

#define __PRINT_FUNCTION_RUN_INFO 1
#if __PRINT_FUNCTION_RUN_INFO
#include <QDebug>
#define FUNCTION_RUN_PRINT() \
    do{\
      qDebug() << " >> fun:" << __FUNCTION__ << " line:" << __LINE__;\
    }while(0)
#else
#define FUNCTION_RUN_PRINT()
#endif

class SATcpClientPrivate;

/**
 * @brief 客户端的封装
 */
class SATcpClient : public QObject
{
    SA_IMPL(SATcpClient)
    Q_OBJECT
public:
    SATcpClient(QObject* par = nullptr);
    ~SATcpClient();
    bool write(const SAProtocolHeader& header,const QByteArray& data);
    virtual bool deal(const SAProtocolHeader &header, const QByteArray &data);
public slots:
    bool connectToServe(int timeout = 5000);
    //请求token
    void requestToken(int pid,const QString& appid);
    //连接和断开的槽，本例方法未做任何事情
    virtual void socketConnected();
    virtual void socketDisconnected();
private slots:
    void onReceivedData(const SAProtocolHeader &header, const QByteArray &data);
private:
    void init();
signals:
    /**
     * @brief 返回请求的token
     * @param token
     * @see SATcpClient::requestToken
     */
    void replyToken(const QString& token,int sequenceID);
};

#endif // SATCPCLIENT_H
