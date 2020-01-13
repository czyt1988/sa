#ifndef SAABSTRACTRPCSERVE_H
#define SAABSTRACTRPCSERVE_H

#include <QObject>
#include "SARPCGlobal.h"

/**
 * @brief rpc服务的封装，此rpc服务不依赖任何现有协议如jsonrpc，或xmlrpc，而是根据qt variant的特性自己定义的
 * 数据传输协议，为了减少带宽以及提高速度
 *
 * 服务流程：
 * 先进行服务注册 @sa registerProcedure
 * 注册完毕后进行端口监听 @sa listen
 */
class SARPC_EXPORT SAAbstractRPCServe : public QObject
{
    Q_OBJECT
public:
    SAAbstractRPCServe(QObject* par = nullptr);
    virtual ~SAAbstractRPCServe();
    //注册调用
    virtual void registerProcedure(const QString& serName) = 0;
    //监听
    virtual bool listen(int port) = 0;
    //关闭
    virtual void close() = 0;
};

#endif // SARPCSERVE_H
