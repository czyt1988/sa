#include "SATcpDataProcessClient.h"
class SATcpDataProcessClientPrivate
{
    SA_IMPL_PUBLIC(SATcpDataProcessClient)
public:
    SATcpDataProcessClientPrivate(SATcpDataProcessClient* p);
};

SATcpDataProcessClientPrivate::SATcpDataProcessClientPrivate(SATcpDataProcessClient *p)
{

}

SATcpDataProcessClient::SATcpDataProcessClient(QObject *par):SATcpClient(par)
{

}

SATcpDataProcessClient::~SATcpDataProcessClient()
{

}

bool SATcpDataProcessClient::deal(const SAProtocolHeader &header, const QByteArray &data)
{
    if(SATcpClient::deal(header,data))
    {
        return true;
    }
}

void SATcpDataProcessClient::request2DPointsDescribe(const QVector<QPointF> &arrs, uint key)
{

}


