#include "SAThreadProcessPool.h"
#include <QThreadPool>

#define IS_DEBUG_PRINT 1
#if IS_DEBUG_PRINT
    #include <QDebug>
#endif
SAThreadProcessPool::SAThreadProcessPool(QObject *par):QObject(par)
{

}

void SAThreadProcessPool::addTask(SAAbstractProcess *task)
{
#if IS_DEBUG_PRINT
    qDebug() << "add task:" << task->getID();
#endif
    connect(task,&SAAbstractProcess::result,this,&SAThreadProcessPool::result);
    connect(task,&SAAbstractProcess::finish,this,&SAThreadProcessPool::finish);
    connect(task,&SAAbstractProcess::progress,this,&SAThreadProcessPool::progress);
    QThreadPool::globalInstance()->start(task);
}
