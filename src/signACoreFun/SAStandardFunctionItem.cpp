#include "SAStandardFunctionItem.h"


SAStandardFunctionItem::SAStandardFunctionItem():mFun(nullptr)
{

}

SAStandardFunctionItem::SAStandardFunctionItem(const QString &funDescribe, SAStandardFunctionItem::FUN_PTR funPtr)
{
    if(setFunctionDescribe(funDescribe))
    {
        mFun = funPtr;
    }
}

SAStandardFunctionItem::~SAStandardFunctionItem()
{

}
///
/// \brief 设置标准函数item的函数指针
/// \param fun
///
void SAStandardFunctionItem::setupFunPtr(SAStandardFunctionItem::FUN_PTR fun)
{
    mFun = fun;
}

///
/// \brief 执行函数
/// \param input 输入参数
/// \param output 输出参数
/// \param info 参数信息
/// \return 成功执行函数返回true
///
bool SAStandardFunctionItem::execute(QList<SAAbstractDatas *> &input
                                     , QList<SAAbstractDatas *> &output
                                     , QString *info)
{
    if(isValid())
    {
        return mFun(input,output,info);
    }
    return false;
}
///
/// \brief 判断此函数item是否有效
/// \return
///
bool SAStandardFunctionItem::isValid() const
{
    return (mFun != nullptr);
}
///
/// \brief 设置函数的简要说明
/// \param brief
///
void SAStandardFunctionItem::setFunctionBrief(const QString &brief)
{
    mFunBrief = brief;
}
///
/// \brief 获取函数的简要说明
/// \return
///
QString SAStandardFunctionItem::getFunctionBrief() const
{
    return mFunBrief;
}
///
/// \brief 设置函数名
/// 此函数名只是函数的名字，不是函数全名，设置函数全名使用setFunctionFullName
///
/// 函数全名是类似：spectrum(wave,fs,fftSize,ampType)->[fre,amp]
///
/// 函数名是：spectrum
/// \param str
///
void SAStandardFunctionItem::setFunctionName(const QString &str)
{
    mFunName = str;
}

QString SAStandardFunctionItem::getFunctionName() const
{
    return mFunName;
}

QString SAStandardFunctionItem::getFunctionDescribe() const
{
    return mFunDescribe;
}

bool SAStandardFunctionItem::setFunctionDescribe(const QString &str)
{
    if(!parserFunctionFullName(str))
    {
        return false;
    }
    mFunDescribe = str;
    return true;
}

///
/// \brief 获取此函数的输入参数数目
///
unsigned SAStandardFunctionItem::getInputCount() const
{
    return mFunInfo.inputSize;
}
///
/// \brief 获取此函数的输出参数数目
///
unsigned SAStandardFunctionItem::getOutputCount() const
{
    return mFunInfo.outputSize;
}
///
/// \brief 获取输入参数的说明
/// \param inputIndex 输入参数的索引
/// \param overloadIndex 重载的索引
/// \return 说明
///
QString SAStandardFunctionItem::getInputAnnotation(int inputIndex) const
{
    if(inputIndex < mFunInfo.inputAnnotation.size())
    {
        return  mFunInfo.inputAnnotation[inputIndex];
    }
    return QString();
}
///
/// \brief 获取输出参数的说明
/// \param inputIndex 输出参数的索引
/// \param overloadIndex 重载的索引
/// \return 说明
///
QString SAStandardFunctionItem::getOutputAnnotation(int inputIndex) const
{
    if(inputIndex < mFunInfo.outputAnnotation.size())
    {
        return mFunInfo.outputAnnotation[inputIndex];
    }
    return QString();
}


///
/// \brief 添加函数的信息
/// \param checkPtr 检测指针，如果没有对应的参数输入检测指针，会返回true
/// \param inputAnnotation 输入参数的注释
/// \param outputAnnotation 输出参数的注释
/// \param inputSize 输入参数的数量
/// \param outputSize 输出参数的数量
/// \note 如果输入参数的数量和checkPtr 检测指针及inputAnnotation 输入参数的注释不一致
/// 那么对应checkPtr 检测指针列表里没有的会默认返回true，对应inputAnnotation 输入参数的注释列表没有的会返回一个空字符串，outputSize同理
///
void SAStandardFunctionItem::setFunctionInfo(QStringList inputAnnotation
                                                , QStringList outputAnnotation
                                                , int inputSize
                                                , int outputSize)
{
    if(mFunInfo.inputAnnotation.size() < inputSize)
    {
        mFunInfo.inputAnnotation.resize(inputSize);
    }
    for(int i=0;i<inputAnnotation.size() && i < inputSize;++i)
    {
        mFunInfo.inputAnnotation[i] = inputAnnotation[i];
    }

    if(mFunInfo.outputAnnotation.size() < outputSize)
    {
        mFunInfo.outputAnnotation.resize(outputSize);
    }
    for(int i=0;i<outputAnnotation.size() && i < outputSize;++i)
    {
        mFunInfo.outputAnnotation[i] = outputAnnotation[i];
    }

    mFunInfo.inputSize = inputSize;
    mFunInfo.outputSize = outputSize;
}

///
/// \brief parser类似于"fun(arg1,arg2,arg3)->[ret1,ret2,ret3]"
///
/// 解析函数名，自动解析出函数名字，函数输入参数，函数输出参数
/// \param funFullName
/// \return
///
bool SAStandardFunctionItem::parserFunctionFullName(const QString &funFullName)
{
    int leftBracketIndex = funFullName.indexOf("(");
    int rightBracketIndex = funFullName.indexOf(")");
    int leftMidBracketIndex = funFullName.indexOf("[");
    int rightMidBracketIndex = funFullName.indexOf("]");

    if(leftBracketIndex <= 0)
    {
        return false;
    }
    if(rightBracketIndex < leftBracketIndex)
    {
        return false;
    }
    if(leftMidBracketIndex <= 0 || leftMidBracketIndex < rightBracketIndex)
    {
        return false;
    }
    if(rightMidBracketIndex < leftMidBracketIndex)
    {
        return false;
    }
    //提取参数列表 - arg1,arg2,arg3
    QString args;
    QStringList argLists;
    if((rightBracketIndex - leftBracketIndex) > 1)
    {
        args = funFullName.mid(leftBracketIndex+1,rightBracketIndex-leftBracketIndex-1);
    }
    if(!parserFunctionArgs(args,argLists))
    {
        return false;
    }
    //提取返回列表 - [ret1,ret2,ret3]
    QString rets;
    QStringList retLists;
    if((rightMidBracketIndex - leftMidBracketIndex) > 1)
    {
        rets = funFullName.mid(leftMidBracketIndex+1,rightMidBracketIndex-leftMidBracketIndex-1);
    }
    if(!parserFunctionArgs(rets,retLists))
    {
        return false;
    }

    //提取函数名 - fun
    mFunName = funFullName.mid(0,leftBracketIndex);
    mFunInfo.inputSize = argLists.size();
    mFunInfo.outputSize = retLists.size();
    //默认说明就是函数描述的参数名称
    if(mFunInfo.inputAnnotation.size() < mFunInfo.inputSize)
    {
        mFunInfo.inputAnnotation.resize(mFunInfo.inputSize);
    }
    for(int i=0;i<mFunInfo.inputAnnotation.size();++i)
    {
        if(mFunInfo.inputAnnotation[i].isEmpty())
        {
            mFunInfo.inputAnnotation[i] = argLists[i];
        }
    }

    if(mFunInfo.outputAnnotation.size() < mFunInfo.outputSize)
    {
        mFunInfo.outputAnnotation.resize(mFunInfo.outputSize);
    }
    for(int i=0;i<mFunInfo.outputAnnotation.size();++i)
    {
        if(mFunInfo.outputAnnotation[i].isEmpty())
        {
            mFunInfo.outputAnnotation[i] = retLists[i];
        }
    }
    return true;
}

///
/// \brief 解析参数列表
/// 如果输入空字符串也返回true
/// \param args 参数列表如:"arg1,arg2,arg3,arg4"
/// \return 如果输入空字符串也返回true，如果参数列表非法返回false
///
bool SAStandardFunctionItem::parserFunctionArgs(const QString &args, QStringList &argsList)
{
    if(args.isEmpty())
    {
        return true;
    }
    argsList = args.split(',');

    //TODO 验证命名正确性
    return true;
}









