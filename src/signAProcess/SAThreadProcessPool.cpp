#include "SAThreadProcessPool.h"
#include <QThreadPool>

//class SAThreadProcessPoolPrivate{
//    SA_IMPL_PUBLIC(SAThreadProcessPool)
//public:
//    SAThreadProcessPoolPrivate(SAThreadProcessPool* p):q_ptr(p)
//    {

//    }
//};

SAThreadProcessPool::SAThreadProcessPool(QObject *par):QObject(par)
{

}

void SAThreadProcessPool::addTask(SAAbstractProcess *task)
{
    connect(task,&SAAbstractProcess::result,this,&SAThreadProcessPool::result);
    connect(task,&SAAbstractProcess::finish,this,&SAThreadProcessPool::finish);
    connect(task,&SAAbstractProcess::progress,this,&SAThreadProcessPool::progress);
    QThreadPool::globalInstance()->start(task);
}
