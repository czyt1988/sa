#include "sa_fun_preproc.h"
#include "sa_fun_core.h"
#include <QVector>
#include "SASmooth.h"
#include "SAMath.h"
#include "SAValueManager.h"
#include "SAAlgorithm.h"
#include <QCoreApplication>

#define TR(str)\
    QCoreApplication::translate("sa_fun_preproc", str, 0)




std::shared_ptr<SAAbstractDatas> saFun::pointSmooth(SAAbstractDatas *wave, int points, int power)
{
    if(wave->getType() == SA::VectorPoint)
    {
        auto ptr = SAValueManager::makeData<SAVectorPointF>(QString("%1_%2P%3P")
                                                            .arg(wave->getName())
                                                            .arg(points)
                                                            .arg(power));
        if(pointSmooth(static_cast<const SAVectorPointF*>(wave),points,power,ptr.get()))
        {
            return SAValueManager::castPointToBase(ptr);
        }
    }
    else
    {
        auto ptr = SAValueManager::makeData<SAVectorDouble>(QString("%1_%2P%3P")
                                                            .arg(wave->getName())
                                                            .arg(points)
                                                            .arg(power));
        if(pointSmooth(wave,points,power,ptr.get()))
        {
            return SAValueManager::castPointToBase(ptr);
        }
    }
    return nullptr;
}
bool saFun::pointSmooth(const SAAbstractDatas *wave, int points, int power, SAVectorDouble *output)
{
    QVector<double> orData;
    QVector<double> smoothY;
    if(!saFun::getDoubleVector(wave,orData))
    {
        return false;
    }
    smoothY.resize (orData.size ());
    if(3 == points && 1 == power)
        SA::SASmooth::linearSmooth3 (orData.begin (),orData.end (),smoothY.begin ());
    else if(5 == points && 1 == power)
        SA::SASmooth::linearSmooth5 (orData.begin (),orData.end (),smoothY.begin ());
    else if(7 == points && 1 == power)
        SA::SASmooth::linearSmooth7 (orData.begin (),orData.end (),smoothY.begin ());
    else if(5 == points && 2 == power)
        SA::SASmooth::quadraticSmooth5 (orData.begin (),orData.end (),smoothY.begin ());
    else if(7 == points && 2 == power)
        SA::SASmooth::quadraticSmooth7 (orData.begin (),orData.end (),smoothY.begin ());
    else if(5 == points && 3 == power)
        SA::SASmooth::cubicSmooth5 (orData.begin (),orData.end (),smoothY.begin ());
    else if(7 == points && 3 == power)
        SA::SASmooth::cubicSmooth7 (orData.begin (),orData.end (),smoothY.begin ());
    else
    {
        saFun::setErrorString( (TR("can not deal [%1 points %2 power],"
                                   "m,n should be set to"
                                   " [m,n]={[3,1],[5,1],[7,1],[5,2],[7,2],[5,3],[7,3]").arg(points).arg(power)));
        return false;
    }
    if(output)
    {
        output->setValueDatas(smoothY);
    }
    return true;
}
bool saFun::pointSmooth(const SAVectorPointF *wave, int points, int power, SAVectorPointF *output)
{
    QVector<double> orData;
    QVector<double> smoothY;
    orData.reserve(wave->getSize());
    SAVectorPointF::getYs(wave,std::back_inserter(orData));
    smoothY.resize (orData.size ());
    if(3 == points && 1 == power)
        SA::SASmooth::linearSmooth3 (orData.begin (),orData.end (),smoothY.begin ());
    else if(5 == points && 1 == power)
        SA::SASmooth::linearSmooth5 (orData.begin (),orData.end (),smoothY.begin ());
    else if(7 == points && 1 == power)
        SA::SASmooth::linearSmooth7 (orData.begin (),orData.end (),smoothY.begin ());
    else if(5 == points && 2 == power)
        SA::SASmooth::quadraticSmooth5 (orData.begin (),orData.end (),smoothY.begin ());
    else if(7 == points && 2 == power)
        SA::SASmooth::quadraticSmooth7 (orData.begin (),orData.end (),smoothY.begin ());
    else if(5 == points && 3 == power)
        SA::SASmooth::cubicSmooth5 (orData.begin (),orData.end (),smoothY.begin ());
    else if(7 == points && 3 == power)
        SA::SASmooth::cubicSmooth7 (orData.begin (),orData.end (),smoothY.begin ());
    else
    {
        saFun::setErrorString( (TR("can not deal [%1 points %2 power],"
                                   "m,n should be set to"
                                   " [m,n]={[3,1],[5,1],[7,1],[5,2],[7,2],[5,3],[7,3]").arg(points).arg(power)));
        return false;
    }
    if(output)
    {
        SAVectorPointF::replaceYs(output,smoothY.begin(),smoothY.end());
    }
    return true;
}

std::tuple<
    std::shared_ptr<SAAbstractDatas>
    ,std::shared_ptr<SAAbstractDatas>
    ,std::shared_ptr<SAVectorInt>
    ,std::shared_ptr<SAVectorInt>
>
saFun::sigmaDenoising(const SAAbstractDatas *wave, double sigma)
{
    std::shared_ptr<SAVectorInt> outRangIndex = SAValueManager::makeData<SAVectorInt>(QString("%1_sigmaOutRangIndex").arg(wave->getName()));
    std::shared_ptr<SAVectorInt> inRangIndex = SAValueManager::makeData<SAVectorInt>(QString("%1_sigmaInRangIndex").arg(wave->getName()));
    if(wave->getType() == SA::VectorPoint)
    {
        auto waveDenoising = SAValueManager::makeData<SAVectorPointF>(QString("%1_sigmaDenoising").arg(wave->getName()));
        auto waveRemove = SAValueManager::makeData<SAVectorPointF>(QString("%1_sigmaRemove").arg(wave->getName()));
        if(sigmaDenoising(static_cast<const SAVectorPointF*>(wave),sigma
                          ,waveDenoising.get(),waveRemove.get(),outRangIndex.get(),inRangIndex.get()))
        {
            return std::make_tuple(SAValueManager::castPointToBase(waveDenoising)
                                   ,SAValueManager::castPointToBase(waveRemove)
                                   ,outRangIndex,inRangIndex);
        }
        return std::make_tuple(nullptr,nullptr,nullptr,nullptr);
    }
    else
    {
        auto waveDenoising = SAValueManager::makeData<SAVectorDouble>(QString("%1_sigmaDenoising").arg(wave->getName()));
        auto waveRemove = SAValueManager::makeData<SAVectorDouble>(QString("%1_sigmaRemove").arg(wave->getName()));
        if(sigmaDenoising(wave,sigma,waveDenoising.get(),waveRemove.get(),outRangIndex.get(),inRangIndex.get()))
        {
            return std::make_tuple(SAValueManager::castPointToBase(waveDenoising)
                                   ,SAValueManager::castPointToBase(waveRemove)
                                   ,outRangIndex,inRangIndex);
        }
        return std::make_tuple(nullptr,nullptr,nullptr,nullptr);
    }
    return std::make_tuple(nullptr,nullptr,nullptr,nullptr);
}

bool saFun::sigmaDenoising(const SAAbstractDatas *wave, double sigma, SAVectorDouble *waveDenoising, SAVectorDouble *waveRemove, SAVectorInt *outRangIndex, SAVectorInt *inRangIndex)
{
    QVector<double> orData,denoisingData,beRemoveData;
    if(!saFun::getDoubleVector(wave,orData))
    {
        return false;
    }
    QVector<int> indexOutRang;//记录超出范围的索引
    QVector<int> indexInRang;//记录在范围的索引
    SA::get_n_sigma_rang(orData.cbegin (),orData.cend (),sigma
                                    ,std::back_inserter(indexOutRang)
                                    ,std::back_inserter(indexInRang));
    beRemoveData.reserve(indexOutRang.size());
    denoisingData.reserve(orData.size() - indexOutRang.size());
    SA::split_with_indexs(orData.cbegin(),orData.cend()
                       ,indexOutRang.begin(),indexOutRang.end()
                       ,std::back_inserter(beRemoveData)
                       ,std::back_inserter(denoisingData));
    if(waveDenoising)
    {
        waveDenoising->setValueDatas(denoisingData);
    }
    if(waveRemove)
    {
        waveRemove->setValueDatas(beRemoveData);
    }
    if(outRangIndex)
    {
        outRangIndex->setValueDatas(indexOutRang);
    }
    if(inRangIndex)
    {
        inRangIndex->setValueDatas(indexInRang);
    }
    return true;
}
///
/// \brief sigma去噪
/// \param wave 波形
/// \param igma sigma值，默认为3
/// \param waveDenoising 滤波后的波形
/// \param waveRemove 被虑去的内容
/// \param outRangIndex 被虑去的索引
/// \return 成功返回true
///
bool saFun::sigmaDenoising(const SAVectorPointF *wave, double sigma, SAVectorPointF *waveDenoising, SAVectorPointF *waveRemove, SAVectorInt *outRangIndex, SAVectorInt *inRangIndex)
{
    QVector<double> ys;
    ys.reserve(wave->getSize());
    SAVectorPointF::getYs(wave,std::back_inserter(ys));
    QVector<QPointF> denoisingData,beRemoveData;
    QVector<int> indexOutRang;//记录超出范围的索引
    QVector<int> indexInRang;//记录在范围的索引
    SA::get_n_sigma_rang(ys.begin (),ys.end (),sigma
                                 ,std::back_inserter(indexOutRang)
                                 ,std::back_inserter(indexInRang));
    beRemoveData.reserve(indexOutRang.size());
    denoisingData.reserve(ys.size() - indexOutRang.size());
    SA::split_with_indexs(wave->cbegin(),wave->cend()
                       ,indexOutRang.begin(),indexOutRang.end()
                       ,std::back_inserter(beRemoveData)
                       ,std::back_inserter(denoisingData));
    if(waveDenoising)
    {
        waveDenoising->setValueDatas(denoisingData);
    }
    if(waveRemove)
    {
        waveRemove->setValueDatas(beRemoveData);
    }
    if(outRangIndex)
    {
        outRangIndex->setValueDatas(indexOutRang);
    }
    if(inRangIndex)
    {
        inRangIndex->setValueDatas(indexInRang);
    }
    return true;
}



void saFun::sigmaDenoising(const QVector<double>& ys
                           , double sigma
                           , QVector<int> &index
                           )
{
    SA::get_out_n_sigma_rang(ys.begin (),ys.end ()
                                    ,sigma
                                    ,std::back_inserter(index));
}



bool saFun::pointSmooth(const QVector<double> &orData, int points, int power, QVector<double>& smoothY)
{
    smoothY.resize (orData.size ());
    if(3 == points && 1 == power)
        SA::SASmooth::linearSmooth3 (orData.cbegin (),orData.cend (),smoothY.begin ());
    else if(5 == points && 1 == power)
        SA::SASmooth::linearSmooth5 (orData.cbegin (),orData.cend (),smoothY.begin ());
    else if(7 == points && 1 == power)
        SA::SASmooth::linearSmooth7 (orData.cbegin (),orData.cend (),smoothY.begin ());
    else if(5 == points && 2 == power)
        SA::SASmooth::quadraticSmooth5 (orData.cbegin (),orData.cend (),smoothY.begin ());
    else if(7 == points && 2 == power)
        SA::SASmooth::quadraticSmooth7 (orData.cbegin (),orData.cend (),smoothY.begin ());
    else if(5 == points && 3 == power)
        SA::SASmooth::cubicSmooth5 (orData.cbegin (),orData.cend (),smoothY.begin ());
    else if(7 == points && 3 == power)
        SA::SASmooth::cubicSmooth7 (orData.cbegin (),orData.cend (),smoothY.begin ());
    else
    {
        saFun::setErrorString( (TR("can not deal [%1 points %2 power],"
                                   "m,n should be set to"
                                   " [m,n]={[3,1],[5,1],[7,1],[5,2],[7,2],[5,3],[7,3]").arg(points).arg(power)));
        return false;
    }
    return true;
}
