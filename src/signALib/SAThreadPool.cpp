#include "SAThreadPool.h"
#include <QList>
class SAThreadPoolPrivate
{
    SA_IMPL_PUBLIC(SAThreadPool)
public:
    SAThreadPoolPrivate(SAThreadPool* p);
    int maxThreadCnt;///< 最大允许线程数
    QList<QThread*> threadList;///< 存放线程的列表
    int currentIndex;///< 记录当前的索引
};

SAThreadPoolPrivate::SAThreadPoolPrivate(SAThreadPool *p):q_ptr(p)
  ,maxThreadCnt(8)
  ,currentIndex(-1)
{

}

SAThreadPool::SAThreadPool()
{

}

SAThreadPool::~SAThreadPool()
{

}

SAThreadPool &SAThreadPool::getInstance()
{
    static SAThreadPool s_p;
    return s_p;
}

/**
 * @brief 获取允许的最大线程数
 * @return
 */
int SAThreadPool::getMaxThreadCount() const
{
    return d_ptr->maxThreadCnt;
}

/**
 * @brief 设置最大线程数
 * @param n
 * @note 如果已经分配的线程超过此设定，不会裁剪数量，就算有空闲也不会裁剪
 */
void SAThreadPool::setMaxThreadCount(int n) const
{
    d_ptr->maxThreadCnt = n;
}

/**
 * @brief 轮询下一个线程
 * @return
 */
QThread *SAThreadPool::next()
{
    int index = d_ptr->currentIndex;
    ++index;
    if(index >= d_ptr->maxThreadCnt)
    {
        index = 0;
    }
    if(index < d_ptr->threadList.size())
    {
        d_ptr->currentIndex = index;
        return d_ptr->threadList[index];
    }
    QThread* p = new QThread();
    d_ptr->threadList.append(p);
    d_ptr->currentIndex = index;
    return p;
}

/**
 * @brief 获取线程
 * @return
 */
QThread *SAThreadPool::getThread()
{
    return SAThreadPool::getInstance().next();
}

/**
 * @brief 获取当前池管理的最大线程数
 * @return
 */
int SAThreadPool::poolMaxThreadCount()
{
    return SAThreadPool::getInstance().getMaxThreadCount();
}


