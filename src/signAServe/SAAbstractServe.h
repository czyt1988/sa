#ifndef SAABSTRACTSERVE_H
#define SAABSTRACTSERVE_H
#include <QObject>
#include <QAbstractSocket>
#include <QHostAddress>
#include "SAServeGlobal.h"
/**
 * @brief sa的服务基类
 * 为什么不直接用QTCPServe之类的继承？因为sa协议是想兼容xml和json协议的，protocol就做成了
 * 兼容性，不过目前暂时只会支持xml
 */
class SASERVE_EXPORT SAAbstractServe
{
public:
    SAAbstractServe();
    virtual ~SAAbstractServe();
    //监听
    virtual bool listen(int port,const QHostAddress &address = QHostAddress::Any) = 0;
    //关闭
    virtual void close() = 0;

};

#endif // SARPCSERVE_H
