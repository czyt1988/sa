#include "sa_fun_core.h"

#include <QVector>
#include <iterator>
#include "SAValueManager.h"
#include "SAAbstractDatas.h"
#include "SAVectorDouble.h"
#include "SAVariantDatas.h"
#include "SADataConver.h"
#include <QCoreApplication>
///
/// \brief 激记录错误信息
///
static QString s_error_info = QString("");

#define TR(str)\
    QCoreApplication::translate("sa_fun_core", str, 0)

#if 0
#ifndef FUNCTION_DEFINE
#define FUNCTION_DEFINE(funName)\
    bool saFun::funName(QList<SAAbstractDatas *> &input,QList<SAAbstractDatas *> &output,QString* info)
#endif





#define INFO(str)\
    if(nullptr != info)\
    {\
        (*info) = TR(str);\
    }

#define INFO_QSTRING(str)\
    if(nullptr != info)\
    {\
        (*info) = str;\
    }

#define INFO_NOT_TR(str)\
    if(nullptr != info)\
    {\
        (*info) = str;\
    }

/// \def 如果输入的参数不等于count会返回错误
#define SA_CHECK_INPUT_COUNT(inputList,count)\
    if(inputList.size() != count)\
    {\
        INFO("argument incorrect");\
        return false;\
    }

/// \def 输入的参数需要大于count
#define SA_CHECK_INPUT_SHOULD_MORE_THAN(inputList,count)\
    if(inputList.size() <= count)\
    {\
        QString str = TR("argument incorrect,input count should more than %1").arg(count);\
        INFO_QSTRING(str);\
        return false;\
    }
/// \def 输入的参数需要大于count
#define SA_CHECK_INPUT_SHOULD_MORE_THAN_EQUAL(inputList,count)\
    if(inputList.size() < count)\
    {\
        QString str = TR("argument incorrect,input count should more than %1,now args is %2")\
                        .arg(count).arg(inputList.size());\
        INFO_QSTRING(str);\
        return false;\
    }

#define SA_CHECK_IS_DOUBLE_VECTOR(input,str)\
    if(input->getType() != SA::VectorDouble)\
    {\
        INFO_NOT_TR(str);\
        return false;\
    }

#define SA_CHECK_IS_VAR(input,str)\
    if(input->getType() != SA::Variant)\
    {\
        INFO_NOT_TR(str);\
        return false;\
    }

#define SA_GET_VAR_INPUT(dataType,input,val)\
    if(!SAVariantDatas::toData<dataType>(input,val))\
    {\
        INFO_NOT_TR(TR("argument %1 can not conver to "#dataType).arg(#val));\
        return false;\
    }

bool saFun::getDoubleVector(SAAbstractDatas* input,QString* info,QVector<double>& vd)
{
    if(!SAAbstractDatas::converToDoubleVector(input,vd))
    {
        INFO_NOT_TR(TR("can not conver %1 to vector").arg(input->getName()));
        return false;
    }
    return true;
}

///
/// \brief 求均值 mean(vector) -> mean
/// \param 数据
/// \param
/// \return
///
FUNCTION_DEFINE(mean)
{
    SA_CHECK_INPUT_COUNT(input,1);
    SAAbstractDatas* data = input[0];
    QVector<double> vd;
    if(!saFun::getDoubleVector(data,info,vd))
    {
        return false;
    }
    double meanData = czy::Math::mean(vd.begin(),vd.end());
    output.append( SAValueManager::makeData_s<SAVariantDatas>(meanData) );
    return true;
}
///
/// \brief 求和运算
/// \param 数据
/// \param output[频率，幅值]
/// \return [频率结果,幅值结果]
///
FUNCTION_DEFINE(sum)
{
    SA_CHECK_INPUT_COUNT(input,1);
    SAAbstractDatas* data = input[0];
    QVector<double> vd;
    if(!saFun::getDoubleVector(data,info,vd))
    {
        return nullptr;
    }
    double sum = czy::Math::sum(vd.begin(),vd.end());
    output.append( SAValueManager::makeData_s<SAVariantDatas>(sum) );
    return true;
}
///
/// \brief 加法运算
/// \param 加数，可以无限个
/// \param output[减法结果]
/// \return
///
FUNCTION_DEFINE(add)
{
    SA_CHECK_INPUT_SHOULD_MORE_THAN_EQUAL(input,2);

    int size = input.size();

    SAAbstractDatas* res = nullptr;
    SAAbstractDatas* tmp = nullptr;
    res = saFun::add(input[0],input[1]);
    if(nullptr == res)
    {
        INFO_NOT_TR(TR("sa cannot add %1 to %2").arg(input[0]->getName(),input[1]->getName()));
        return false;
    }
    for(int i=2;i<size;++i)
    {
        tmp = res;
        res = saFun::add(res,input[i]);
        if(nullptr == res)
        {
            INFO_NOT_TR(TR("sa cannot add %1 to %2").arg(tmp->getName(),input[i]->getName()));
            return false;
        }
        //删除临时变量
        SAValueManager::deleteData_s(tmp);

    }
    output.append(res);
    return true;
}
///
/// \brief 减法运算
/// \param 减数，可以无限个
/// \param output[减法结果]
/// \return
///
FUNCTION_DEFINE(subtract)
{
    SA_CHECK_INPUT_SHOULD_MORE_THAN_EQUAL(input,2);

    int size = input.size();

    SAAbstractDatas* res = nullptr;
    SAAbstractDatas* tmp = nullptr;
    res = saFun::subtract(input[0],input[1]);
    if(nullptr == res)
    {
        INFO_NOT_TR(TR("sa cannot subtract %1 to %2").arg(input[0]->getName(),input[1]->getName()));
        return false;
    }
    for(int i=2;i<size;++i)
    {
        tmp = res;
        res = saFun::subtract(res,input[i]);
        if(nullptr == res)
        {
            INFO_NOT_TR(TR("sa cannot subtract %1 to %2").arg(tmp->getName(),input[i]->getName()));
            return false;
        }
        //删除临时变量
        SAValueManager::deleteData_s(tmp);

    }
    output.append(res);
    return true;
}
///
/// \brief 乘法运算
/// \param 乘数，可以无限个
/// \param output[乘法结果]
/// \return
///
FUNCTION_DEFINE(multiplication)
{
    SA_CHECK_INPUT_SHOULD_MORE_THAN_EQUAL(input,2);

    int size = input.size();

    SAAbstractDatas* res = nullptr;
    SAAbstractDatas* tmp = nullptr;
    res = saFun::multiplication(input[0],input[1]);
    if(nullptr == res)
    {
        INFO_NOT_TR(TR("sa cannot multiplication %1 to %2").arg(input[0]->getName(),input[1]->getName()));
        return false;
    }
    for(int i=2;i<size;++i)
    {
        tmp = res;
        res = saFun::multiplication(res,input[i]);
        if(nullptr == res)
        {
            INFO_NOT_TR(TR("sa cannot multiplication %1 to %2").arg(tmp->getName(),input[i]->getName()));
            return false;
        }
        //删除临时变量
        SAValueManager::deleteData_s(tmp);

    }
    output.append(res);
    return true;
}
///
/// \brief 除法运算
/// \param 除数，可以无限个
/// \param output[除法结果]
/// \return
///
FUNCTION_DEFINE(division)
{
    SA_CHECK_INPUT_SHOULD_MORE_THAN(input,2);

    int size = input.size();

    SAAbstractDatas* res = nullptr;
    SAAbstractDatas* tmp = nullptr;
    res = saFun::division(input[0],input[1]);
    if(nullptr == res)
    {
        INFO_NOT_TR(TR("sa cannot division %1 to %2").arg(input[0]->getName(),input[1]->getName()));
        return false;
    }
    for(int i=2;i<size;++i)
    {
        tmp = res;
        res = saFun::multiplication(res,input[i]);
        if(nullptr == res)
        {
            INFO_NOT_TR(TR("sa cannot division %1 to %2").arg(tmp->getName(),input[i]->getName()));
            return false;
        }
        //删除临时变量
        SAValueManager::deleteData_s(tmp);

    }
    output.append(res);
    return true;
}
///
/// \brief 频谱分析
/// \param 波形
///  sampleRate 采样率
///  fftSize fft长度
///  type 频谱幅值标示方法，type==Magnitude使用幅度谱，type==Amplitude使用幅值谱
/// \param output[频率，幅值]
/// \return [频率结果,幅值结果]
///
FUNCTION_DEFINE(spectrum) //bool spectrum(QList<SAAbstractDatas *> &input,QList<SAAbstractDatas *> &output,QString* info)
{
    const int inputCount = 4;
    SA_CHECK_INPUT_COUNT(input,inputCount);
    SA_CHECK_IS_DOUBLE_VECTOR(input[0],TR("argument 1[wave] type must be double vector!"));
    for(int i=1;i<inputCount;++i)
    {
        SA_CHECK_IS_VAR(input[i],TR("argument %1 must be variant!").arg(i+1));
    }
    double fs;
    int fftSize,ampType;

    SA_GET_VAR_INPUT(double,input[1],fs);
    SA_GET_VAR_INPUT(int,input[2],fftSize);
    SA_GET_VAR_INPUT(int,input[3],ampType);

    SAVectorDouble* fre = nullptr;
    SAVectorDouble* mag = nullptr;

    fre = SAValueManager::makeData_s<SAVectorDouble>();
    mag = SAValueManager::makeData_s<SAVectorDouble>();
    fre->setName(QString("%1-fre").arg(input[0]->getName()));
    mag->setName(QString("%1-mag").arg(input[0]->getName()));
    SAVectorDouble* waveDouble = static_cast<SAVectorDouble*>(input[0]);
    std::back_insert_iterator<SAVectorDouble> freIte(*fre);
    std::back_insert_iterator<SAVectorDouble> magIte(*mag);
    int len = czy::Math::DSP::spectrum(waveDouble->begin(),waveDouble->end()
                                   ,freIte,magIte
                                       ,fs,fftSize
                                       ,static_cast<czy::Math::DSP::SpectrumType>(ampType));

    if(len <= 0)
    {
        SAValueManager::deleteData_s(fre);
        SAValueManager::deleteData_s(mag);
        return false;
    }
    output.append(fre);
    output.append(mag);
    return true;
}
///
/// \brief 功率谱分析
/// \param 波形
///  sampleRate 采样率
///  fftSize fft长度
///  type PowerDensityWay功率谱估计的方法，功率谱估计提供了3种估计方法
/// \param output[频率，幅值]
/// \return [频率结果,幅值结果]
///
FUNCTION_DEFINE(powerSpectrum)//bool powerSpectrum(QList<SAAbstractDatas *> &input,QList<SAAbstractDatas *> &output,QString* info)
{
    const int minInputCount = 4;
    SA_CHECK_INPUT_SHOULD_MORE_THAN_EQUAL(input,minInputCount);
    SA_CHECK_IS_DOUBLE_VECTOR(input[0],TR("argument 1[wave] type must be double vector!"));
    for(int i=1;i<input.size();++i)
    {
        SA_CHECK_IS_VAR(input[i],TR("argument %1 must be variant!").arg(i+1));
    }

    double fs = 100;
    int fftSize = 1024;
    int tmp = 0;
    czy::Math::DSP::PowerDensityWay pdw;
    double samplingInterval = 0;

    SA_GET_VAR_INPUT(double,input[1],fs);
    SA_GET_VAR_INPUT(int,input[2],fftSize);
    SA_GET_VAR_INPUT(int,input[3],tmp);
    pdw = static_cast<czy::Math::DSP::PowerDensityWay>(tmp);

    if(czy::Math::DSP::TISA == pdw)
    {
        if(input.size() < 5)
        {
            INFO_NOT_TR(TR("TISA mode must have 5 input,now have %1 args;miss sampling Interval").arg(input.size()));
            return false;
        }
        SA_GET_VAR_INPUT(double,input[4],samplingInterval);
    }

    SAAbstractDatas* fre = nullptr;
    SAAbstractDatas* mag = nullptr;
    std::tie(fre,mag) = powerSpectrum(input[0],fs,fftSize,tmp,samplingInterval);
    if(nullptr == fre|| nullptr == mag)
    {
        INFO("unknow error when run powerSpectrum");
        return false;
    }
    output<<fre<<mag;
    return true;
}
#endif
///
/// \brief 获取数组，如果没能获取，会写入错误信息
/// \param input 输入数据
/// \param vd 获取结果
/// \return 成功返回true，否则可以通过getLastErrorString获取错误信息
///
bool saFun::getDoubleVector(const SAAbstractDatas* input,QVector<double>& vd)
{
    if(!SADataConver::converToDoubleVector(input,vd))
    {
        setErrorString(TR("can not conver %1 to vector").arg(input->getName()));
        return false;
    }
    return true;
}
///
/// \brief 获取点数组，如果没能获取，会写入错误信息
/// \param input 输入数据
/// \param vd 获取结果
/// \return 成功返回true，否则可以通过getLastErrorString获取错误信息
///
bool saFun::getPointFVector(const SAAbstractDatas *input, QVector<QPointF> &vd)
{
    if(!SADataConver::converToPointFVector(input,vd))
    {
        setErrorString(TR("can not conver %1 to points vector").arg(input->getName()));
        return false;
    }
    return true;
}
//获取错误信息
QString saFun::getLastErrorString()
{
    return s_error_info;
}




void saFun::setErrorString(const QString &str)
{
    s_error_info = str;
}


///
/// \brief 获取点集的y值
/// \param input 输入的参数
/// \param vd 向量
/// \return 成功获取返回true
///
bool saFun::getPointFVectorYData(const SAAbstractDatas *input, QVector<double> &vd)
{
    if(input->getType() == SA::VectorPoint)
    {
        vd.resize(input->getSize());
        SAVectorPointF::getYs(static_cast<const SAVectorPointF*>(input),vd.begin());
        return true;
    }
    QVector<QPointF> ps;
    if(saFun::getPointFVector(input,ps))
    {
        vd.reserve(ps.size());
        std::for_each(ps.begin(),ps.end(),[&vd](const QPointF& p){
            vd.append(p.y());
        });
        return true;
    }
    return false;
}


///
/// \brief 获取点集的x值
/// \param input 输入的参数
/// \param vd 向量
/// \return 成功获取返回true
///
bool saFun::getPointFVectorXData(const SAAbstractDatas *input, QVector<double> &vd)
{
    if(input->getType() == SA::VectorPoint)
    {
        vd.resize(input->getSize());
        SAVectorPointF::getXs(static_cast<const SAVectorPointF*>(input),vd.begin());
        return true;
    }
    QVector<QPointF> ps;
    if(saFun::getPointFVector(input,ps))
    {
        vd.reserve(ps.size());
        std::for_each(ps.begin(),ps.end(),[&vd](const QPointF& p){
            vd.append(p.x());
        });
        return true;
    }
    return false;
}
///
/// \brief 获取点集的x,y值
/// \param input 输入的参数
/// \param xs x值
/// \param ys y值
/// \return 成功获取返回true
///
bool saFun::getPointFVectorXYData(const SAAbstractDatas *input, QVector<double> &xs, QVector<double> &ys)
{
    if(input->getType() == SA::VectorPoint)
    {
        SAVectorPointF::getXs(static_cast<const SAVectorPointF*>(input),xs.begin());
        SAVectorPointF::getYs(static_cast<const SAVectorPointF*>(input),ys.begin());
        return true;
    }
    QVector<QPointF> ps;
    if(saFun::getPointFVector(input,ps))
    {
        xs.reserve(ps.size());
        ys.reserve(ps.size());
        std::for_each(ps.begin(),ps.end(),[&xs,&ys](const QPointF& p){
            xs.append(p.x());
            ys.append(p.y());
        });
        return true;
    }
    return false;
}
///
/// \brief 把x，y数组设置为等长，长度为最小一组的长度
/// \param xs x数组
/// \param ys y数组
///
void saFun::fixSizeXYVector(QVector<double> &xs, QVector<double> &ys)
{
    if(xs.size() != ys.size())
    {
        int minSize = std::min(xs.size(),ys.size());
        if(xs.size() != minSize)
        {
            xs.resize(minSize);
        }
        if(ys.size() != minSize)
        {
            ys.resize(minSize);
        }
    }
}

///
/// \brief saFun::makeVectorPointF
/// \param x
/// \param y
/// \param xys
///
void saFun::makeVectorPointF(const QVector<double> &x, const QVector<double> &y, QVector<QPointF> &xys)
{
    const int minSize = qMin(x.size(),y.size());
    xys.resize(minSize);
    for(int i=0;i<minSize;++i)
    {
        xys[i] = QPointF(x[i],y[i]);
    }
}
