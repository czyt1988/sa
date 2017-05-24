#ifndef SAABSTRACTFUNCTIONITEM
#define SAABSTRACTFUNCTIONITEM
#include <QString>
#include "SACoreFunGlobal.h"
class SAAbstractDatas;

class SA_CORE_FUN__EXPORT SAAbstractFunctionItem
{
public:
    SAAbstractFunctionItem(){}
    virtual ~SAAbstractFunctionItem(){}
    ///
    /// \brief 执行函数
    /// \param input 输入参数列表
    /// \param output 输出参数列表
    /// \param info 返回的信息，如果函数执行成功无需理会，如果函数执行失败，错误信息通过此传递
    /// \return 函数执行成功返回true，否则为false
    ///
    virtual bool execute(QList<SAAbstractDatas*>& input,QList<SAAbstractDatas*>& output,QString* info) = 0;
    ///
    /// \brief 获取获取函数名
    /// 函数function(wave,fs,type)->[ret1,ret2],函数名应该为function
    /// \return 函数名
    ///
    virtual QString getFunctionName() const = 0;
    ///
    /// \brief 函数的描述，既全称 function(wave,fs,type)->[ret1,ret2]
    /// \return
    ///
    virtual QString getFunctionDescribe() const = 0;
    ///
    /// \brief 函数的简要说明
    /// \return
    ///
    virtual QString getFunctionBrief() const = 0;
    ///
    /// \brief 获取此函数的输入参数数目
    ///
    virtual unsigned getInputCount() const = 0;
    ///
    /// \brief 获取此函数的输出参数数目
    ///
    virtual unsigned getOutputCount() const = 0;
    ///
    /// \brief 获取输入参数的说明
    /// \param inputIndex 输入参数的索引
    /// \param overloadIndex 重载的索引
    /// \return 说明
    ///
    virtual QString getInputAnnotation(int inputIndex) const = 0;
    ///
    /// \brief 获取输出参数的说明
    /// \param inputIndex 输出参数的索引
    /// \param overloadIndex 重载的索引
    /// \return 说明
    ///
    virtual QString getOutputAnnotation(int inputIndex) const = 0;
};

#endif // SAABSTRACTFUNCTIONITEM

