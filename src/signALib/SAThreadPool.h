#ifndef SATHREADPOOL_H
#define SATHREADPOOL_H
#include "SALibGlobal.h"
#include <QThread>
class SAThreadPoolPrivate;

/**
 * @brief 简单的管理QThread的线程池
 * @note 此类为单例
 * @note 此类专门管理QThread，有别于Qt的QThreadPool
 */
class SALIB_EXPORT SAThreadPool
{
    SA_IMPL(SAThreadPool)
    Q_DISABLE_COPY(SAThreadPool)
private:
    SAThreadPool();
public:
    ~SAThreadPool();
    //获取实例
    static SAThreadPool& getInstance();
    //获取最多线程数
    int getMaxThreadCount() const;
    void setMaxThreadCount(int n) const;
    //获取线程
    QThread* next();
    //获取线程
    static QThread* getThread();
    //获取当前池管理的最大线程数
    static int poolMaxThreadCount();
};

#endif // SATHREADPOOL_H
