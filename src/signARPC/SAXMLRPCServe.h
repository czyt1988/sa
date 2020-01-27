#ifndef SAXMLRPCSERVE_H
#define SAXMLRPCSERVE_H
#include "SAAbstractRPCServe.h"
#include "SARPCGlobal.h"
class SAXMLRPCServePrivate;

class SARPC_EXPORT SAXMLRPCServe : public SAAbstractRPCServe
{
    Q_OBJECT
    SA_IMPL(SAXMLRPCServe)
public:
    SAXMLRPCServe(QObject* par = nullptr);
    ~SAXMLRPCServe();
    //注册调用
    virtual void registerProcedure(const QString& serName);
    //监听
    virtual bool listen(int port);
    //关闭
    virtual void close();
};

#endif // SAXMLRPCSERVE_H
