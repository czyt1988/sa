#include "SAAbstractProcess.h"
#include <memory>


SAAbstractProcess::SAAbstractProcess(QObject* par):QObject(par),m_isStop(false)
{

}

SAAbstractProcess::~SAAbstractProcess()
{

}


/**
 * @brief 获取标识
 * 标识可以用于区分工作任务
 * @return
 */
uint SAAbstractProcess::getID() const
{
    return (uint)this;
}
/**
 * @brief 设置运行停止
 * @param stop
 */
void SAAbstractProcess::setStop(bool stop)
{
    m_isStop = stop;
}
/**
 * @brief 判断是否停止
 * @return
 */
bool SAAbstractProcess::isStop() const
{
    return m_isStop;
}



