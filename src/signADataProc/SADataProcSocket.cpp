#include "SADataProcSocket.h"
#include "SAServerDefine.h"
#include "SAServeHandleFun.h"
#include "SADataProcFunctions.h"
#include <QThreadPool>
#include "SAMath.h"
#include "runnable/SADataStatisticRunable.h"

SADataProcSocket::SADataProcSocket(QObject *p) : SATcpSocket(p)
{
}


SADataProcSocket::~SADataProcSocket()
{
}


bool SADataProcSocket::dealXmlProtocol(const SAProtocolHeader& header, const SAXMLProtocolParser& xml)
{
    qDebug() << "serve rec,header fun:" << header.protocolFunID << " type:" << header.protocolTypeID << xml.toString();
    if (SATcpSocket::dealXmlProtocol(header, xml)) {
        qDebug() << "SATcpSocket::dealXmlProtocol return true";
        return (true);
    }
    //对应SADataProcSection的处理
    switch (header.protocolFunID)
    {
    case SA::ProtocolFunReq2DPointsDescribe:
        return (deal2DPointsDescribe(header, xml));

    default:
        break;
    }
    return (false);
}


/**
 * @brief 处理二维点的请求
 * @param header 协议头
 * @param xml xml信息
 * @return
 */
bool SADataProcSocket::deal2DPointsDescribe(const SAProtocolHeader& header, const SAXMLProtocolParser& xml)
{
    qDebug() << QStringLiteral("开始执行deal2DPointsDescribe");
    //SADataStatisticRunable* runnable = new SADataStatisticRunable(shared_from_this(),header,xml);
    //QThreadPool::globalInstance()->start(runnable);
    return (_deal2DPointsDescribe(header, xml));
}


/**
 * @brief 处理点序列的具体函数
 * @param header
 * @param xml
 * @return
 */
bool SADataProcSocket::_deal2DPointsDescribe(const SAProtocolHeader& header, const SAXMLProtocolParser& xml)
{
    if (SA::ProtocolFunReq2DPointsDescribe != xml.getFunctionID()) {
        replyError(header, tr("unknow fun id"), SA::ProtocolErrorUnknowFun);
        return (true);
    }
    //获取points
    QVector<double> ys;
    QVector<QPointF> points;
    int sortcount = 20;

    if (!SA::receive_request_2d_points_describe_xml(&xml, points, sortcount)) {
        replyError(header, tr("xml content error"), SA::ProtocolErrorContent);
        return (true);
    }

    ys.reserve(points.size());
    for (const QPointF& p : points)
    {
        ys.append(p.y());
    }
    if ((sortcount < 0) || (sortcount > 1000)) {
        sortcount = 20;
    }
    //进行数据处理
    double sum;
    double mean;
    double var;
    double stdVar;
    double skewness;
    double kurtosis;
    int n = ys.size();

    SA::get_statistics(ys.begin(), ys.end()
        , sum
        , mean
        , var
        , stdVar
        , skewness
        , kurtosis);
    std::sort(points.begin(), points.end()
        , [](const QPointF& a, const QPointF& b)->bool {
        return (a.y() < b.y());
    });
    double min = points.cbegin()->y();                              //最小
    double max = (points.cend()-1)->y();                            //最大
    double mid = n > 1 ? (points.cbegin() + int(n/2))->y() : min;   //中位数
    double peak2peak = max - min;
    QPointF minPoint = *points.cbegin();
    QPointF maxPoint = *(points.cend()-1);
    QPointF midPoint = n > 1 ? *(points.cbegin() + int(n/2)) : minPoint;//中位数
    int sortedcount = sortcount < n ? sortcount : n;
    QVector<QPointF> tops(sortedcount);
    QVector<QPointF> lows(sortedcount);

    //拷贝tops
    std::copy(points.rbegin(), points.rbegin()+sortedcount, tops.begin());
    //拷贝lows
    std::copy(points.begin(), points.begin()+sortedcount, lows.begin());


    SA::reply_2d_points_describe_xml(this, header
        , sum, mean, var, stdVar, skewness, kurtosis
        , min, max, mid, peak2peak, minPoint, maxPoint, midPoint
        , tops, lows);
#if 0
    qDebug()	<< "reply_2d_points_describe_xml,sum:" << sum << " mean:"  << mean
            << " var:" << var << " stdVar:"<<stdVar << " skewness:" << skewness
            << " kurtosis:" << kurtosis << " min:"<< min << " max:" << max << " peak2peak:"<<peak2peak
            <<" minPoint:" << minPoint << " maxPoint:"<< maxPoint << " midPoint:"<<midPoint;
#endif
    return (true);
}
