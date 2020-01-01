#ifndef SAABSTRACTPROCESS_H
#define SAABSTRACTPROCESS_H
#include "SAProcessGlobal.h"
#include <QObject>
#include <QVariant>
#include <QRunnable>
#include <QVariantList>
class SAAbstractProcessPrivate;
/**
 * @brief sa处理基类
 * 此类封装了QRunnable，用于QThreadPool,处理密集的处理请求，并通过信号槽传递数据
 *
 * 处理类都是一些耗时较长的操作进行计算，如果简单计算，不应该使用处理类来执行
 * @author czy
 * @date 2019-08-06
 */
class SA_PROCESS_EXPORT SAAbstractProcess : public QObject, public QRunnable
{
    Q_OBJECT
    SA_IMPL(SAAbstractProcess)
public:
    enum ResultType{
             XmlString   ///< xml协议
             ,JsonString ///< json协议
             ,UserDef    ///< 用户自定义协议
    };

    SAAbstractProcess(QObject* par = nullptr);
    virtual ~SAAbstractProcess();
    //获取ID
    uint getID() const;
    //设置停止运算
    void setStop(bool stop = true);
    bool isStop() const;
   /**
    * @brief 返回名字
    * @return 返回名字
    */
    virtual QString getName() const = 0;
    /**
     * @brief 获取命名空间
     * @return 命名空间，如果没有返回empty字符串
     */
    virtual QString getNameSpace() const = 0;
    /**
     * @brief 设置参数
     * @param args 参数
     * @return 设置成功返回true
     */
    virtual bool setArgs(const QVariantList& args) = 0;
signals:
    /**
     * @brief 返回处理的结果
     * @param res 返回处理的结果
     * @param id 标识，标识也可通过 @sa getID 进行获取，每个任务会有一个唯一id
     * @param type 标记结果的数据类型，具体类型见 @sa SAAbstractProcess::ResultType
     */
    void result(const QVariant& res,uint id,int type);
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
