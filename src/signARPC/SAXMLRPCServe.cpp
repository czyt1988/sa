#include "SAXMLRPCServe.h"
#include <QTcpServer>

class SAXMLRPCServePrivate
{
public:
    SA_IMPL_PUBLIC(SAXMLRPCServe)
    SAXMLRPCServePrivate(SAXMLRPCServe* p);
};

SAXMLRPCServePrivate::SAXMLRPCServePrivate(SAXMLRPCServe* p):q_ptr(p)
{

}

SAXMLRPCServe::SAXMLRPCServe(QObject *par):SAAbstractRPCServe(par)
  ,d_ptr(new SAXMLRPCServePrivate(this))
{

}

SAXMLRPCServe::~SAXMLRPCServe()
{

}

void SAXMLRPCServe::registerProcedure(const QString &serName)
{

}

bool SAXMLRPCServe::listen(int port)
{

}

void SAXMLRPCServe::close()
{

}


