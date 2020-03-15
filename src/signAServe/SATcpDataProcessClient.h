#ifndef SATCPDATAPROCESSCLIENT_H
#define SATCPDATAPROCESSCLIENT_H
#include "SAServeGlobal.h"
#include "SATcpClient.h"

SA_IMPL_FORWARD_DECL(SATcpDataProcessClient)

/**
 * @brief 用于数据处理的客户端
 */
class SASERVE_EXPORT SATcpDataProcessClient : public SATcpClient
{
    Q_OBJECT
    SA_IMPL(SATcpDataProcessClient)
public:
    SATcpDataProcessClient(QObject* par = nullptr);
    ~SATcpDataProcessClient();
    virtual bool deal(const SAProtocolHeader &header, const QByteArray &data) override;
    static QVariant vectorpointsToVariant(const QVector<QPointF> &arrs);
    static QVector<QPointF> variantToVectorpoints(const QVariant& var);
public slots:
    //请求2维数据的统计描述
    void request2DPointsDescribe(const QVector<QPointF>& arrs,uint key);
signals:
};

#endif // SADATAPROCESSCLIENT_H
