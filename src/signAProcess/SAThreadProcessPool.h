#ifndef SATHREADPROCESSPOOL_H
#define SATHREADPROCESSPOOL_H
#include "SAProcessGlobal.h"
#include "SAAbstractProcess.h"
#include <QObject>

//class SAThreadProcessPoolPrivate;

class SA_PROCESS_EXPORT SAThreadProcessPool : public QObject
{
    Q_OBJECT
    //SA_IMPL(SAThreadProcessPool)
public:
    SAThreadProcessPool(QObject* par = nullptr);
    void addTask(SAAbstractProcess* task);
signals:
    /**
     * @brief 返回处理的结果
     * @param res 返回处理的结果
     * @param id 标识，标识也可通过\sa getID 进行获取，每个任务会有一个唯一id
     */
    void result(const QVariant& res,uint id);
    /**
     * @brief 计算结束
     * @param id 标识，标识也可通过\sa getID 进行获取，每个任务会有一个唯一id
     */
    void finish(uint id);
    /**
     * @brief 用于反馈进度
     * @param present 进度百分比，0~100
     * @param iid 标识，标识也可通过\sa getID 进行获取，每个任务会有一个唯一id
     */
    void progress(short present,uint id);
};

#endif // SATHREADPROCESSPOOL_H
