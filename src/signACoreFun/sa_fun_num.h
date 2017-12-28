#ifndef SAFUNNUM_H
#define SAFUNNUM_H
#include "SACoreFunGlobal.h"
#include "czyMath.h"
#include "sa_fun_core.h"
#include <QString>
#include <QMap>
class SAAbstractDatas;
class SAVariantDatas;
class SAVectorDouble;
class SATableVariant;
class SAVectorInterval;

#define IDS_SUM "sum"
#define IDS_MEAN "mean"
#define IDS_VAR "var"
#define IDS_STD "std"
#define IDS_SKEWNESS "skewness"
#define IDS_KURTOSIS "kurtosis"
#define IDS_PEAK2PEAK "peak2peak"

namespace saFun {
//求和 sum(vector) -> sum
SA_CORE_FUN__EXPORT
std::shared_ptr<SAVariantDatas> sum(SAAbstractDatas* data);
//加法
SA_CORE_FUN__EXPORT
std::shared_ptr<SAAbstractDatas> add(SAAbstractDatas* a,SAAbstractDatas* b);
//减法
SA_CORE_FUN__EXPORT
std::shared_ptr<SAAbstractDatas> subtract(SAAbstractDatas* a,SAAbstractDatas* b);
//乘法
SA_CORE_FUN__EXPORT
std::shared_ptr<SAAbstractDatas> multiplication(SAAbstractDatas* a,SAAbstractDatas* b);
//除法
SA_CORE_FUN__EXPORT
std::shared_ptr<SAAbstractDatas> division(SAAbstractDatas* a,SAAbstractDatas* b);
//求均值 mean(vector) -> mean
SA_CORE_FUN__EXPORT
std::shared_ptr<SAVariantDatas> mean(SAAbstractDatas* data);
//差分
SA_CORE_FUN__EXPORT
std::shared_ptr<SAVectorDouble> diff(SAAbstractDatas* data,unsigned diffCount);
//计算频率统计参数
SA_CORE_FUN__EXPORT
std::shared_ptr<SATableVariant> statistics(SAAbstractDatas* data);

SA_CORE_FUN__EXPORT
QMap<QString,double> statistics(const QVector<double>& data);
///
/// \brief 频率统计计算
/// \param wave 波形
/// \param section 统计段数
/// \return 如果错误发生返回nullptr
///
SA_CORE_FUN__EXPORT
std::shared_ptr<SAVectorInterval> hist(const SAAbstractDatas* wave,unsigned section);

}

#endif // SAFUNSTATISTICS_H
