#ifndef SAABSTRACTPROCESS_H
#define SAABSTRACTPROCESS_H
#include "SAProcessGlobal.h"
#include "SAVariantHashData.h"
#include <QObject>
#include <QVariant>
#include <QRunnable>
class SAAbstractProcessPrivate;
/**
 * @brief sa处理基类
 * 此类封装了QRunnable，用于QThreadPool,处理密集的处理请求，并通过信号槽传递数据
 * @author czy
 * @date 2019-08-06
 */
class SA_PROCESS_EXPORT SAAbstractProcess : public QObject, public QRunnable
{
    Q_OBJECT
    SA_IMPL(SAAbstractProcess)
public:
    SAAbstractProcess(QObject* par = nullptr);
    virtual ~SAAbstractProcess();
    //获取ID
    uint getID() const;
    //设置停止运算
    void setStop(bool stop = true);
    bool isStop() const;
signals:
    /**
     * @brief 返回处理的结果
     * @param res 返回处理的结果
     * @param id 标识，标识也可通过\sa getID 进行获取，每个任务会有一个唯一id
     */
    void result(SAVariantHashData& res,uint id);
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

#endif // SAABSTRACTPROCESS_H
