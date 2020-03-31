#include "SADataProcFunctions.h"
#include "SAMath.h"
#include "SAServerDefine.h"
#include "SATcpDataProcessClient.h"
// 简化的XMLDataPtr的创建
SASession::XMLDataPtr createXMLDataPtr(int funid);
SASession::XMLDataPtr createXMLDataPtr(int funid,SASession::XMLDataPtr reqxml);
SASession::XMLDataPtr createXMLDataPtr(int funid)
{
    SASession::XMLDataPtr res = SASession::makeXMLDataPtr();
    res->setClassID(SA::ProtocolTypeXml);
    res->setFunctionID(funid);
    return res;
}
SASession::XMLDataPtr createXMLDataPtr(int funid,SASession::XMLDataPtr reqxml)
{
    SASession::XMLDataPtr res = SASession::makeXMLDataPtr();
    res->setClassID(SA::ProtocolTypeXml);
    res->setFunctionID(funid);
    QVariant key = reqxml->getValue("key");
    res->setValue("key",key);
    return res;
}

SASession::XMLDataPtr reply2DPointsDescribe(const SAProtocolHeader &header,SASession::XMLDataPtr xml)
{
    Q_UNUSED(header);
    SASession::XMLDataPtr res = createXMLDataPtr(SA::ProtocolFunReply2DPointsDescribe,xml);
    if(SA::ProtocolFunReq2DPointsDescribe != xml->getFunctionID())
    {
        res->setValue("result",0);
        return res;
    }
    //获取points
    QVector<double> ys;
    QVector<QPointF> points;
    {
        QVariant pv = xml->getValue("points");
        points = SATcpDataProcessClient::variantToVectorpoints(pv);
    }
    ys.reserve(points.size());
    for(const QPointF& p : points)
    {
        ys.append(p.y());
    }
    //获取排序数
    int sortcount = xml->getValue("sort-count").toInt();
    if (sortcount<0 || sortcount>1000)
    {
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
    SA::get_statistics(ys.begin(),ys.end()
                              ,sum
                              ,mean
                              ,var
                              ,stdVar
                              ,skewness
                              ,kurtosis);
    std::sort(points.begin(),points.end()
              ,[](const QPointF& a,const QPointF& b)->bool{
        return (a.y() < b.y());
    });
    double min = points.cbegin()->y();//最小
    double max = (points.cend()-1)->y();//最大
    double mid = n>1 ? (points.cbegin() + int(n/2))->y() : min;//中位数
    double peak2peak = max - min;
    QPointF minPoint = *points.cbegin();
    QPointF maxPoint = *(points.cend()-1);
    QPointF midPoint = n>1 ? *(points.cbegin() + int(n/2)) : minPoint;//中位数
    //把结果写入结果xml
    res->setValue("sum",sum);
    res->setValue("mean",mean);
    res->setValue("var",var);
    res->setValue("stdVar",stdVar);
    res->setValue("skewness",skewness);
    res->setValue("kurtosis",kurtosis);
    res->setValue("min",min);
    res->setValue("max",max);
    res->setValue("mid",mid);
    res->setValue("peak2peak",peak2peak);
    res->setValue("min-point",minPoint);
    res->setValue("max-point",maxPoint);
    res->setValue("mid-point",midPoint);
    int sortedcount = sortcount < n ? sortcount : n;
    QVector<QPointF> tops(sortedcount);
    QVector<QPointF> lows(sortedcount);
    //拷贝tops
    std::copy(points.rbegin(),points.rbegin()+sortedcount,tops.begin());
    //拷贝lows
    std::copy(points.begin(),points.begin()+sortedcount,lows.begin());
    res->setValue("sorted-lows",QVariant::fromValue(tops));
    res->setValue("sorted-lows",QVariant::fromValue(lows));
    return res;
}
