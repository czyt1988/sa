#include "SAAbstractProcess.h"
#include <atomic>
class SAAbstractProcessPrivate
{
    SA_IMPL_PUBLIC(SAAbstractProcess)
public:
    SAAbstractProcessPrivate(SAAbstractProcess* p):q_ptr(p)
        ,m_isstop(true)
    {

    }
    ~SAAbstractProcessPrivate(){

    }

public:
    uint m_Key;
    std::atomic_bool m_isstop;
};


SAAbstractProcess::SAAbstractProcess(QObject* par):QObject(par)
  ,d_ptr(new SAAbstractProcessPrivate(this))
{

}

SAAbstractProcess::~SAAbstractProcess()
{

}

/**
 * @brief 设置一个key
 * @param key 标识key
 */
void SAAbstractProcess::setKey(uint key)
{
    d_ptr->m_Key = key;
}

/**
 * @brief SAAbstractProcess::getKey
 * @return
 */
uint SAAbstractProcess::getKey() const
{
    return d_ptr->m_Key;
}

void SAAbstractProcess::setStop(bool stop)
{
    d_ptr->m_isstop = stop;
}

bool SAAbstractProcess::isStop() const
{
    return d_ptr->m_isstop;
}
