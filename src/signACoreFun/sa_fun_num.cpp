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

#include "SAVectorInterval.h"
#define TR(str)\
    QApplication::translate("sa_fun_num", str, 0)


///
/// \brief 求均值 mean(vector) -> mean
/// \param data
/// \return SAVariantDatas
///
std::shared_ptr<SAVariantDatas> saFun::mean(SAAbstractDatas* data)
{
    QVector<double> vd;
    if(!SAAbstractDatas::converToDoubleVector(data,vd))
    {
        setErrorString(TR("data can not conver to double vector"));
        return false;
    }
    double meanData = czy::Math::mean(vd.begin(),vd.end());
    return SAValueManager::makeData<SAVariantDatas>(meanData);
}

///
/// \brief 求和运算
/// \param data 数据
/// \return SAVariantDatas
///
std::shared_ptr<SAVariantDatas> saFun::sum(SAAbstractDatas* data)
{
    QVector<double> vd;
    if(!SAAbstractDatas::converToDoubleVector(data,vd))
    {
        setErrorString( TR("data can not conver to double vector"));
        return nullptr;
    }
    double sumData = czy::Math::sum(vd.begin(),vd.end());
    return SAValueManager::makeData<SAVariantDatas>(sumData);
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
    if(!SAAbstractDatas::converToDoubleVector(data,waveData))
    {
        setErrorString(TR("can not conver data to double vector!"));
        return nullptr;
    }
    QVector<double> dy;
    std::back_insert_iterator< QVector<double> > bi(dy);
    czy::Math::difference(waveData.begin (),waveData.end (),bi,diffCount);
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
    czy::Math::get_statistics(vd.begin (),vd.end (),sum,mean,var,std,skewness,kurtosis);
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
    czy::Math::count_frequency(ys.begin (),ys.end ()
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
