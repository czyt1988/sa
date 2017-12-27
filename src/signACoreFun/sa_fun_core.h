#ifndef SACOREFUNCTION_H
#define SACOREFUNCTION_H
///
/// \file 这是SA的核心内置函数
///
#include "SACoreFunGlobal.h"
#include "czyMath.h"
#include "czyMath_DSP.h"
#include <QList>
#include <QString>
#include <memory>
#include <QPointF>
class SAAbstractDatas;
class SAVariantDatas;
class SAVectorDouble;
//========================Math====================================
//加法运算
namespace saFun {
#if 0
    //求和 sum(vector) -> sum
    SA_FUNCTION_DEFINE(sum);
    // 加法 add(n1,n2,n3,...) -> add
    SA_FUNCTION_DEFINE(add);
    //减法 subtract(n1,n2,n3,...) -> subtract
    SA_FUNCTION_DEFINE(subtract);
    //乘法 multiplication(n1,n2,n3,...) -> multiplication
    SA_FUNCTION_DEFINE(multiplication);
    //除法 division(n1,n2,n3,...) -> division
    SA_FUNCTION_DEFINE(division);
    //求均值 mean(vector) -> mean
    SA_FUNCTION_DEFINE(mean);
    //========================DSP=====================================
    //频谱分析 spectrum(wave,fs,fftSize,ampType)->[fre,amp]
    SA_FUNCTION_DEFINE(spectrum);
    //功率谱分析 powerSpectrum(wave,fs,fftSize,pdw,samplingInterval)->[fre,amp]
    SA_FUNCTION_DEFINE(powerSpectrum);
#endif

    SA_CORE_FUN__EXPORT
    void setErrorString(const QString& str);
    //获取错误信息
    SA_CORE_FUN__EXPORT
    QString getLastErrorString();
    //获取数组，如果没能获取，会写入错误信息
    SA_CORE_FUN__EXPORT
    bool getDoubleVector(const SAAbstractDatas* input,QVector<double>& vd);
    //获取点数组，如果没能获取，会写入错误信息
    SA_CORE_FUN__EXPORT
    bool getPointFVector(const SAAbstractDatas* input,QVector<QPointF>& vd);
    //获取点集的y值，如果没能获取，会写入错误信息
    SA_CORE_FUN__EXPORT
    bool getPointFVectorYData(const SAAbstractDatas* input,QVector<double>& vd);
    //获取点集的x值，如果没能获取，会写入错误信息
    SA_CORE_FUN__EXPORT
    bool getPointFVectorXData(const SAAbstractDatas* input,QVector<double>& vd);
    //获取点集的x,y值，如果没能获取，会写入错误信息
    SA_CORE_FUN__EXPORT
    bool getPointFVectorXYData(const SAAbstractDatas* input,QVector<double>& xs,QVector<double>& ys);
    //把x，y数组设置为等长，长度为最小一组的长度
    SA_CORE_FUN__EXPORT
    void fixSizeXYVector(QVector<double>& xs,QVector<double>& ys);
    //把x序列和y序列组合成pointF序列
    SA_CORE_FUN__EXPORT
    void makeVectorPointF(const QVector<double>& x,const QVector<double>& y,QVector<QPointF>& xys);
}


#endif // SACOREFUNCTION_H
