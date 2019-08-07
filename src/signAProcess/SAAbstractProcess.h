#ifndef SAABSTRACTPROCESS_H
#define SAABSTRACTPROCESS_H
#include "SAProcessGlobal.h"
#include <QObject>
#include <QVariant>
class SAAbstractProcessPrivate;
/**
 * @brief sa处理基类
 * 此类可以理解为轻量级的QThread，run函数不会自动进入新线程，此类主要通过movetothread进行多线程工作
 * @author czy
 * @date 2019-08-06
 */
class SA_PROCESS_EXPORT SAAbstractProcess : public QObject
{
    Q_OBJECT
    SA_IMPL(SAAbstractProcess)
public:
    SAAbstractProcess(QObject* par = nullptr);
    virtual ~SAAbstractProcess();
    //设置一个key标识
    void setKey(uint key);
    uint getKey() const;
    //调用的虚函数 - 类似于QThread::run
    virtual void run() = 0;
    //设置停止运算
    void setStop(bool stop = true);
    bool isStop() const;
signals:
    /**
     * @brief 返回处理的结果
     * @param res 返回处理的结果
     * @param key 标识，标识通过\sa setKey 进行设置
     */
    void result(QVariant& res,uint key);
    /**
     * @brief 计算结束
     */
    void finish();
    /**
     * @brief 用于反馈进度
     * @param present 进度百分比，0~100
     */
    void progress(short present);
};

#endif // SAABSTRACTPROCESS_H
