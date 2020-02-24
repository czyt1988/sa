#ifndef SA_FUN_FIT_H
#define SA_FUN_FIT_H

#include "SACoreFunGlobal.h"
#include "czyMath.h"
#include "czyMath_Fit.h"
#include "sa_fun_core.h"
class SAVectorPointF;
class SAAbstractDatas;
class SAVariantDatas;
class SAVectorDouble;
class SATableVariant;

namespace saFun {

///
/// \brief 进行多项式拟合
/// \param wave 拟合的波形
/// \param n 多项式阶次
/// \return std::tuple::get<0>():tuple第一个参数为factor 拟合结果的各个系数，索引0代表x^0的系数
///
/// std::tuple::get<1>():tuple第二个参数为拟合的误差参数
///
/// 若tuple参数为nullptr，说明计算错误
///
SA_CORE_FUN__EXPORT
std::tuple<
std::shared_ptr<SAVectorDouble>//拟合的系数
,std::shared_ptr<SATableVariant>//拟合的误差参数
>
polyfit(const SAAbstractDatas* wave,int n);


///
/// \brief 进行多项式拟合
/// \param x 拟合的x值
/// \param y 拟合的y值
/// \param n 多项式阶次
/// \return std::tuple::get<0>():tuple第一个参数为factor 拟合结果的各个系数，索引0代表x^0的系数
///
/// std::tuple::get<1>():tuple第二个参数为拟合的误差参数
///
/// 若tuple参数为nullptr，说明计算错误
///
SA_CORE_FUN__EXPORT
std::tuple<
std::shared_ptr<SAVectorDouble>//拟合的系数
,std::shared_ptr<SATableVariant>//拟合的误差参数
>
polyfit(const SAAbstractDatas* x,const SAAbstractDatas* y,int n);

///
/// \brief 进行多项式拟合
/// \param xs 拟合的x值
/// \param ys 拟合的y值
/// \param n 多项式阶次
/// \return std::tuple::get<0>():tuple第一个参数为factor 拟合结果的各个系数，索引0代表x^0的系数
///
/// std::tuple::get<1>():tuple第二个参数为拟合的误差参数
///
/// 若tuple参数为nullptr，说明计算错误
///
SA_CORE_FUN__EXPORT
std::tuple<
std::shared_ptr<SAVectorDouble>//拟合的系数
,std::shared_ptr<SATableVariant>//拟合的误差参数
>
polyfit(const QVector<double> &xs,const QVector<double> &ys,int n);
///
/// \brief 根据多项式拟合的系数，以及输入的x值，计算拟合的值
/// \param x 输入的值，如果是单一数组向量，此值作为x值，如果是点集，此值提取点集的x值
/// \param factor
/// \return std::tuple::get<0>()拟合的值,若传入数组，返回数组y值，若传入点数组，返回拟合后的点集
/// ，若出错，返回nullptr
///
SA_CORE_FUN__EXPORT
std::tuple<
std::shared_ptr<SAAbstractDatas>//拟合的值,若传入数组，返回数组y值，若传入点数组，返回拟合后的点集
>
polyval(const SAAbstractDatas* wave,const SAVectorDouble* factor);

SA_CORE_FUN__EXPORT
void polyval(const QVector<double> &x,const SAVectorDouble* factor,SAVectorDouble* res);
SA_CORE_FUN__EXPORT
void polyval(const QVector<double> &x,const SAVectorDouble* factor,SAVectorPointF* res);
void setFitInfo(SATableVariant* info,const SA::Math::PolyFit* fit);
}

#endif // SA_FUN_FIT_H
