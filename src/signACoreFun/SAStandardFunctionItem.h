#ifndef SASTANDARDFUNCTIONITEM_H
#define SASTANDARDFUNCTIONITEM_H
#include <QString>
#include <QList>
#include <QVector>
#include <functional>
#include "SAAbstractFunctionItem.h"
#include "SACoreFunGlobal.h"
class SAAbstractDatas;

class SA_CORE_FUN__EXPORT SAStandardFunctionItem : public SAAbstractFunctionItem
{
public:
    typedef std::function<bool(QList<SAAbstractDatas*>&,QList<SAAbstractDatas*>&,QString*)> FUN_PTR;
public:
    SAStandardFunctionItem();
    SAStandardFunctionItem(const QString& funDescribe,FUN_PTR funPtr);

    virtual ~SAStandardFunctionItem();
    ///
    /// \brief FUN_PTR
    /// bool fun(QList<SAAbstractDatas*>& input
    ///             ,QList<SAAbstractDatas*>& output
    ///             ,QString& info)
    ///
    //设置函数指针
    void setupFunPtr(FUN_PTR fun);
    //执行函数
    bool execute(QList<SAAbstractDatas*>& input,QList<SAAbstractDatas*>& output,QString* info);
    //是否有效
    bool isValid() const;
    //获取函数名
    void setFunctionName(const QString& str);
    QString getFunctionName() const;
    //
    QString getFunctionDescribe() const;
    bool setFunctionDescribe(const QString& str);
    //获取函数的简要说明
    QString getFunctionBrief() const;
    void setFunctionBrief(const QString& brief);
    //获取此函数的输入参数数目
    unsigned getInputCount() const;
    //获取此函数的输出参数数目
    unsigned getOutputCount() const;
    //获取输入参数的说明
    QString getInputAnnotation(int inputIndex) const;
    //获取输出参数的说明
    QString getOutputAnnotation(int inputIndex) const;
    //检查输入的合法性
    bool checkInput(SAAbstractDatas* data,int inputIndex) const;
    //添加函数的信息
    void setFunctionInfo(QStringList inputAnnotation
                            , QStringList outputAnnotation
                            , int inputSize
                            , int outputSize);
private:
    bool parserFunctionFullName(const QString& funFullName);
    static bool parserFunctionArgs(const QString& args,QStringList &argsList);

private:
    class FunctionInputOutputInfo
    {
    public:
        QVector<QString> inputAnnotation;
        QVector<QString> outputAnnotation;
        int inputSize;
        int outputSize;
    };
    FunctionInputOutputInfo mFunInfo;///< 保存函数重载的各种信息
    QString mFunName;///<函数名
    FUN_PTR mFun;///<函数指针
    QString mFunBrief;///<函数简要说明
    QString mFunDescribe;///<函数全称描述
};

#endif // SAFUNINFO

