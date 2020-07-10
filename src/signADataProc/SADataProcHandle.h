#ifndef SADATAPROCSESSION_H
#define SADATAPROCSESSION_H
#include "SASocketHandle.h"
#include <memory>
#include <QFutureWatcher>
#include <QMutex>

/**
 * @brief 处理数据的session
 */
class SADataProcHandle : public SASocketHandle,
              public std::enable_shared_from_this<SASocketHandle> //为了把this通过weak_ptr传递到线程中
{
public:
    SADataProcHandle(QObject *p = nullptr);
    ~SADataProcHandle();

    //处理xml相关请求
    virtual bool dealXmlProtocol(const SAProtocolHeader& header, SASocketHandle::XMLDataPtr xml);

protected:
    //处理2维点描述
    virtual bool deal2DPointsDescribe(const SAProtocolHeader& header, SASocketHandle::XMLDataPtr xml);

private:
};

#endif // SADATAPROCSECTION_H
