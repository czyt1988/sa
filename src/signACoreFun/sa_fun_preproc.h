#ifndef SA_FUN_PREPROC_H
#define SA_FUN_PREPROC_H
#include "SACoreFunGlobal.h"
#include <memory>
#include <algorithm>
#include <vector>
#include <QPointF>
class SAAbstractDatas;
class SAVariantDatas;
class SAVectorDouble;
class SAVectorInt;
class SAVectorPointF;

namespace saFun {
//m点n次滤波,m值和n值不能随便设置[m,n]={[3,1],[5,1],[7,1],[5,2],[7,2],[5,3],[7,3]}
///
/// \brief m点n次滤波
///
/// points:[int]m点n次平滑的n值
/// power:[int]m点n次平滑的m值
/// \param wave 输入数据
/// \param points 点数，分别为3,5,7
/// \param power 次数，分别为1,2,3
/// \return 成功返回计算结果指针,如果输入可以转换为vector point返回的指针为SAVectorPointF，否则为SAVectorDouble
/// \note m值和n值不能随便设置[m,n]={[3,1],[5,1],[7,1],[5,2],[7,2],[5,3],[7,3]}
///
SA_CORE_FUN__EXPORT
std::shared_ptr<SAAbstractDatas> pointSmooth(SAAbstractDatas* wave,int points,int power);
///
/// \brief m点n次滤波
/// \param wave 输入数据
/// \param points 点数，分别为3,5,7
/// \param power 次数，分别为1,2,3
/// \param output 计算结果指针
/// \return 计算成功返回true
/// \note m值和n值不能随便设置[m,n]={[3,1],[5,1],[7,1],[5,2],[7,2],[5,3],[7,3]}
/// \note 如果不清楚传入的数据是点数组还是数据数组，调用std::shared_ptr<SAAbstractDatas> pointSmooth(SAAbstractDatas* wave,int points,int power);
///
SA_CORE_FUN__EXPORT
bool pointSmooth(const SAAbstractDatas* wave,int points,int power,SAVectorDouble* output);
///
/// \brief m点n次滤波
/// \param wave 输入数据
/// \param points 点数，分别为3,5,7
/// \param power 次数，分别为1,2,3
/// \param output 计算结果指针
/// \return 计算成功返回true
/// \note m值和n值不能随便设置[m,n]={[3,1],[5,1],[7,1],[5,2],[7,2],[5,3],[7,3]}
/// \note 如果不清楚传入的数据是点数组还是数据数组，调用std::shared_ptr<SAAbstractDatas> pointSmooth(SAAbstractDatas* wave,int points,int power);
///
SA_CORE_FUN__EXPORT
bool pointSmooth(const SAVectorPointF* wave, int points, int power, SAVectorPointF *output);

SA_CORE_FUN__EXPORT
bool pointSmooth(const QVector<double>& orData, int points, int power, QVector<double> &smoothY);
///
/// \brief sigma异常值检测
/// \param wave 传入数据波形，波形可为vectordouble或vectorpoint
/// \param sigma sigma值
/// \return std::tuple<waveDenoising,waveRemove,outRangIndex,inRangIndex>
/// waveDenoising 存放去噪后的数据,如果为nullptr说明计算错误
/// waveRemove 存放超出sigma范围的数据,如果为nullptr说明计算错误
/// outRangIndex 存放超出范围数据的索引
/// inRangIndex 存放在范围内数据的索引
///
SA_CORE_FUN__EXPORT
std::tuple<
std::shared_ptr<SAAbstractDatas>//waveDenoising
,std::shared_ptr<SAAbstractDatas>//waveRemove
,std::shared_ptr<SAVectorInt>//outRangIndex
,std::shared_ptr<SAVectorInt> //inRangIndex
>
sigmaDenoising(const SAAbstractDatas *wave, double sigma);
//sigma去噪
///
/// \brief sigma异常值检测
/// \param wave 波形，保证可以转换为vectordouble
/// \param sigma sigma值
/// \param waveDenoising 存放去噪后的数据，传入nullptr将不把此计算结果保存
/// \param waveRemove 存放超出sigma范围的数据，传入nullptr将不把此计算结果保存
/// \param outRangIndex 存放超出范围数据的索引，传入nullptr将不把此计算结果保存
/// \param inRangIndex 存放在范围内数据的索引，传入nullptr将不把此计算结果保存
/// \return 成功执行返回true
///
SA_CORE_FUN__EXPORT
bool sigmaDenoising(const SAAbstractDatas* wave,double sigma
        ,SAVectorDouble* waveDenoising
        ,SAVectorDouble* waveRemove
        ,SAVectorInt* outRangIndex
        ,SAVectorInt* inRangIndex);
///
/// \brief sigma异常值检测
/// \param wave 波形，保证为SAVectorPointF
/// \param sigma sigma值
/// \param waveDenoising 存放去噪后的数据，传入nullptr将不把此计算结果保存
/// \param waveRemove 存放超出sigma范围的数据，传入nullptr将不把此计算结果保存
/// \param outRangIndex 存放超出范围数据的索引，传入nullptr将不把此计算结果保存
/// \param inRangIndex 存放在范围内数据的索引，传入nullptr将不把此计算结果保存
/// \return 成功执行返回true
///
SA_CORE_FUN__EXPORT
bool sigmaDenoising(const SAVectorPointF* wave,double sigma
        ,SAVectorPointF* waveDenoising
        ,SAVectorPointF* waveRemove
        ,SAVectorInt* outRangIndex
        ,SAVectorInt* inRangIndex);



SA_CORE_FUN__EXPORT
void sigmaDenoising(const QVector<double>& xs
                    , const QVector<double>& ys
                    , double sigma
                    , QVector<int> &index);

}


#endif // SA_FUN_PREPROC_H
