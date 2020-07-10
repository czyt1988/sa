#include "SADataStatisticRunable.h"
#include "SADataProcHandle.h"
#include "SATcpDataProcessClient.h"
#include "SAMath.h"
#include "SAServerDefine.h"
#include "SADataProcFunctions.h"


SADataStatisticRunable::SADataStatisticRunable(std::weak_ptr<SADataProcHandle> session, const SAProtocolHeader &header, SASocketHandle::XMLDataPtr xml)
{
    setDatas(session,header,xml);
}

void SADataStatisticRunable::setDatas(std::weak_ptr<SADataProcHandle> session, const SAProtocolHeader &header, SASocketHandle::XMLDataPtr xml)
{
    m_session = session;
    m_header = header;
    m_xml = xml;
}

void SADataStatisticRunable::setup2DPointsDescribe()
{
    m_step.enqueue(TwoDPointsDescribe);
}

void SADataStatisticRunable::run()
{
    do
    {
        RunFun fun = m_step.dequeue();
        switch(fun)
        {
        case TwoDPointsDescribe:
            run2DPointsDescribe();
            break;
        default:
            break;
        }
    }while(m_step.size()>0);
}

void SADataStatisticRunable::run2DPointsDescribe()
{
    //返回的结果
    SASocketHandle::XMLDataPtr res = createXMLDataPtr(SA::ProtocolFunReply2DPointsDescribe,m_xml);
    if(SA::ProtocolFunReq2DPointsDescribe != m_xml->getFunctionID())
    {
        res->setValue("result",0);//标记结果异常
        write(res,SA::ProtocolFunReply2DPointsDescribe);
        return;
    }
    //获取points
    QVector<double> ys;
    QVector<QPointF> points;
    {
        QVariant pv = m_xml->getDefaultGroupValue("points");
        points = SATcpDataProcessClient::variantToVectorpoints(pv);
    }
    ys.reserve(points.size());
    for(const QPointF& p : points)
    {
        ys.append(p.y());
    }
    //获取排序数
    int sortcount = m_xml->getDefaultGroupValue("sort-count").toInt();
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
    write(res,SA::ProtocolFunReply2DPointsDescribe);
}

/**
 * @brief 发送答复结果
 * @param xml
 * @param funid
 */
void SADataStatisticRunable::write(SASocketHandle::XMLDataPtr xml, int funid)
{
    QByteArray data = xml->toByteArray();
    SAProtocolHeader header = createXMLReplyHeader(m_header,data,funid);
    std::shared_ptr<SADataProcHandle> ptr = m_session.lock();
    if(ptr)
    {
        ptr->safe_write(header,data);
    }
}
