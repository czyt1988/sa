#ifndef SADATASTATISTICRUNABLE_H
#define SADATASTATISTICRUNABLE_H
#include <QRunnable>
#include <QQueue>
#include <memory>
#include "SAProtocolHeader.h"
#include "SASocketHandle.h"
class SADataProcHandle;
/**
 * @brief 数据分析用的runable
 * 此runable设计成一种链式计算，一开始需要制定计算的内容，通过各种setup打头的函数，建立计算步骤和计算参数，
 */
class SADataStatisticRunable : public QRunnable
{
public:
    SADataStatisticRunable(std::weak_ptr<SADataProcHandle> session,
                           const SAProtocolHeader &header,
                           SASocketHandle::XMLDataPtr xml);
    //设置值
    void setDatas(std::weak_ptr<SADataProcHandle> session,const SAProtocolHeader &header,SASocketHandle::XMLDataPtr xml);
    //2维点的数据描述，计算和、均值、方程等，同时包含最大最小值
    void setup2DPointsDescribe();
public:
    void run() override;
protected:
     void run2DPointsDescribe();
     void write(SASocketHandle::XMLDataPtr xml, int funid);
private:
    enum RunFun{
        TwoDPointsDescribe ///< 对应setup2DPointsDescribe
    };
    QQueue<RunFun> m_step;
    std::weak_ptr<SADataProcHandle> m_session;
    SAProtocolHeader m_header;
    SASocketHandle::XMLDataPtr m_xml;
};

#endif // SADATASTATISTICRUNABLE_H
