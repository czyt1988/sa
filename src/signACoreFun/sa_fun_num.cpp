#include "sa_fun_num.h"
#include <algorithm>
#include "sa_fun_core_std.h"
#include <QVector>
#include <iterator>
#include "SAValueManager.h"
#include "SAAbstractDatas.h"
#include "SAVectorDouble.h"
#include "SAVariantDatas.h"
#include "SATableVariant.h"
#include <QCoreApplication>
#include "SAVectorInterval.h"
#include "SADataConver.h"
#include "SAMath.h"

#define TR(str)\
    QCoreApplication::translate("sa_fun_num", str, 0)


///
/// \brief 求均值 mean(vector) -> mean
/// \param data
/// \return SAVariantDatas
///
std::shared_ptr<SAVariantDatas> saFun::mean(SAAbstractDatas* data)
{
    QVector<double> vd;
    if(!SADataConver::converToDoubleVector(data,vd))
    {
        setErrorString(TR("data can not conver to double vector"));
        return nullptr;
    }
    double meanData = SA::mean(vd.begin(),vd.end());
    return SAValueManager::makeData<SAVariantDatas>(meanData);
}
///
/// \brief 求均值
/// \param points
/// \return
///
double saFun::meamPointY(const QVector<QPointF> &points)
{
    if(points.size() > 0)
        return sumPointY(points)/ double(points.size());
    return 0;
}
///
/// \brief 求和运算
/// \param data 数据
/// \return SAVariantDatas
///
std::shared_ptr<SAVariantDatas> saFun::sum(SAAbstractDatas* data)
{
    QVector<double> vd;
    if(!SADataConver::converToDoubleVector(data,vd))
    {
        setErrorString( TR("data can not conver to double vector"));
        return nullptr;
    }
    double sumData = SA::sum(vd.begin(),vd.end());
    return SAValueManager::makeData<SAVariantDatas>(sumData);
}
///
/// \brief 求点集的y值和
/// \param points
/// \return
///
double saFun::sumPointY(const QVector<QPointF> &points)
{
    double res = 0.0;
    std::for_each(points.begin(),points.end(),[&res](const QPointF& p){
        res += p.y();
    });
    return res;
}
///
/// \brief 加法
/// \param a
/// \param b
/// \return a+b
///
std::shared_ptr<SAAbstractDatas> saFun::add(SAAbstractDatas* a,SAAbstractDatas* b)
{
    return double_transform(a,b,
                     [](const double& a,const double &b)->double{
        return a+b;});
}

///
/// \brief 减法
/// \param a
/// \param b
/// \return a-b
///
std::shared_ptr<SAAbstractDatas> saFun::subtract(SAAbstractDatas* a,SAAbstractDatas* b)
{
    return double_transform(a,b,
                     [](const double& a,const double &b)->double{
        return a-b;});
}

///
/// \brief 乘法
/// \param a
/// \param b
/// \return a*b
///
std::shared_ptr<SAAbstractDatas> saFun::multiplication(SAAbstractDatas* a,SAAbstractDatas* b)
{
    return double_transform(a,b,
                     [](const double& a,const double &b)->double{
        return a*b;});
}

///
/// \brief 除法
/// \param a
/// \param b
/// \return a/b
///
std::shared_ptr<SAAbstractDatas> saFun::division(SAAbstractDatas* a,SAAbstractDatas* b)
{
    return double_transform(a,b,
                     [](const double& a,const double &b)->double{
        return a/b;});
}

///
/// \brief 计算差分
/// \param data 数据指针
/// \param diffCount 差分次数
/// \return 计算的结果
///
std::shared_ptr<SAVectorDouble> saFun::diff(SAAbstractDatas *data, unsigned diffCount)
{
    QVector<double> waveData;
    if(!SADataConver::converToDoubleVector(data,waveData))
    {
        setErrorString(TR("can not conver data to double vector!"));
        return nullptr;
    }
    QVector<double> dy;
    std::back_insert_iterator< QVector<double> > bi(dy);
    SA::difference(waveData.begin (),waveData.end (),bi,diffCount);
    QString name = QString("%1_diff%2").arg(data->getName()).arg(diffCount);
    return SAValueManager::makeData<SAVectorDouble>(name,waveData);
}

///
/// \brief 计算频率统计参数
/// \param data 数据
/// \return 一个variant表格，表格第一列为计算参数的名称，第二列为计算结果
///
std::shared_ptr<SATableVariant> saFun::statistics(SAAbstractDatas *data)
{
    QVector<double> vd;
    if(!getDoubleVector(data,vd))
    {
        return nullptr;
    }
    double sum,mean,var,std,skewness,kurtosis;
    SA::get_statistics(vd.begin (),vd.end (),sum,mean,var,std,skewness,kurtosis);
    double ppv;//峰峰值
    auto minmax = std::minmax_element(vd.begin (),vd.end ());
    ppv = *minmax.second - *minmax.first;
    std::shared_ptr<SATableVariant> varTable = SAValueManager::makeData<SATableVariant>();
    int r = 0;
    varTable->setTableData (r,0,TR("sum"));varTable->setTableData (r++,1,sum);
    varTable->setTableData (r,0,TR("mean"));varTable->setTableData (r++,1,mean);
    varTable->setTableData (r,0,TR("var"));varTable->setTableData (r++,1,var);
    varTable->setTableData (r,0,TR("std"));varTable->setTableData (r++,1,std);
    varTable->setTableData (r,0,TR("skewness"));varTable->setTableData (r++,1,skewness);
    varTable->setTableData (r,0,TR("kurtosis"));varTable->setTableData (r++,1,kurtosis);
    varTable->setTableData (r,0,TR("max"));varTable->setTableData (r++,1,*minmax.second);
    varTable->setTableData (r,0,TR("min"));varTable->setTableData (r++,1,*minmax.first);
    varTable->setTableData (r,0,TR("peak to peak value"));varTable->setTableData (r++,1,ppv);
    return varTable;
}


std::shared_ptr<SAVectorInterval> saFun::hist(const SAAbstractDatas *wave, unsigned section)
{
    QVector<double> ys;
    if(!saFun::getDoubleVector(wave,ys))
    {
        return nullptr;
    }
    if(ys.size () == 0)
    {
        saFun::setErrorString(TR("%1 size is to short").arg(wave->getName()));
        return nullptr;
    }
    std::vector<double> sectionRang;
    std::vector<int> freCount;
    sectionRang.resize( section+1,0);
    freCount.resize (section,0);
    SA::count_frequency(ys.begin (),ys.end ()
                               ,section
                               ,sectionRang.begin()
                               ,freCount.begin());
    QVector< QwtIntervalSample > sample;
    sample.reserve (section);
    for(unsigned int j=0;j<section;++j)
    {
        sample.push_back(QwtIntervalSample(freCount[j],sectionRang[j],sectionRang[j+1]));
    }
    auto res = SAValueManager::makeData<SAVectorInterval>();
    res->setValueDatas (sample);
    return res;
}

QMap<QString, double> saFun::statistics(const QVector<double> &data)
{
    QMap<QString, double> res;
    double sum,mean,var,std,skewness,kurtosis;
    SA::get_statistics(data.begin (),data.end (),sum,mean,var,std,skewness,kurtosis);
    double ppv;//峰峰值
    auto minmax = std::minmax_element(data.begin (),data.end ());
    ppv = *minmax.second - *minmax.first;
    res[IDS_SUM] = sum;
    res[IDS_MEAN] = mean;
    res[IDS_VAR] = var;
    res[IDS_STD] = std;
    res[IDS_SKEWNESS] = skewness;
    res[IDS_KURTOSIS] = kurtosis;
    res[IDS_PEAK2PEAK] = ppv;
    return res;
}


/**
 * @brief 获取最大最小值
 * @param data
 * @return
 */
std::pair<double, double> saFun::minmax_value(const SAAbstractDatas *data)
{
    QVector<double> ys;
    if(!saFun::getDoubleVector(data,ys))
    {
        return std::make_pair(NAN,NAN);
    }
    return std::make_pair(*(std::min_element(ys.begin(),ys.end()))
                          ,*(std::max_element(ys.begin(),ys.end())));
}
