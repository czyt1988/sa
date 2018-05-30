#include "sa_fun_fit.h"
#include "sa_fun_core.h"
#include <QVector>
#include "SAVectorPointF.h"
#include "SAVectorDouble.h"
#include "czyMath_Fit.h"
#include "SATableVariant.h"
#include "SAValueManager.h"
#include <QCoreApplication>
#define TR(str)\
    QCoreApplication::translate("sa_fun_fit", str, 0)

std::tuple<std::shared_ptr<SAVectorDouble>, std::shared_ptr<SATableVariant> >
saFun::polyfit(const SAAbstractDatas *wave, int n)
{
    QVector<double> xs,ys;
    if(!saFun::getPointFVectorXYData(wave,xs,ys))
    {
        return std::make_tuple(nullptr,nullptr);
    }
    czy::Math::PolyFit fit;
    if(!fit.polyfit(xs.data(),ys.data(),xs.size(),n))
    {
        saFun::setErrorString(TR("can not polyfit"));
        return std::make_tuple(nullptr,nullptr);
    }
    std::shared_ptr<SAVectorDouble> factor = SAValueManager::makeData<SAVectorDouble>(QString("%1_%2PolyFitFactor")
                                                                                      .arg(wave->getName()).arg(n));
    const size_t factorSize = fit.getFactorSize();
    factor->clear();
    factor->reserve(factorSize);
    for(size_t f = 0;f < factorSize;++f)
    {
        factor->append(fit.getFactor(f));
    }
    std::shared_ptr<SATableVariant> info = SAValueManager::makeData<SATableVariant>(QString("%1_%2PolyFitErrInfo")
                                                                                      .arg(wave->getName()).arg(n));
    setFitInfo(info.get(),&fit);
    return std::make_tuple(factor,info);
}

std::tuple<std::shared_ptr<SAVectorDouble>, std::shared_ptr<SATableVariant> >
saFun::polyfit(const SAAbstractDatas *x, const SAAbstractDatas *y, int n)
{
    QVector<double> xs,ys;
    ys.reserve(x->getSize());
    xs.reserve(y->getSize());
    if(!saFun::getDoubleVector(x,xs))
    {
        return std::make_tuple(nullptr,nullptr);
    }
    if(!saFun::getDoubleVector(y,ys))
    {
        return std::make_tuple(nullptr,nullptr);
    }
    saFun::fixSizeXYVector(xs,ys);
    czy::Math::PolyFit fit;
    if(!fit.polyfit(xs.data(),ys.data(),xs.size(),n))
    {
        saFun::setErrorString(TR("can not polyfit"));
        return std::make_tuple(nullptr,nullptr);
    }
    std::shared_ptr<SAVectorDouble> factor = SAValueManager::makeData<SAVectorDouble>(QString("%1-%2_%3PolyFitFactor")
                                                                                      .arg(x->getName()).arg(y->getName()).arg(n));
    const size_t factorSize = fit.getFactorSize();
    factor->clear();
    factor->reserve(factorSize);
    for(size_t f = 0;f < factorSize;++f)
    {
        factor->append(fit.getFactor(f));
    }
    std::shared_ptr<SATableVariant> info = SAValueManager::makeData<SATableVariant>(QString("%1-%2_%3PolyFitErrInfo")
                                                                                      .arg(x->getName()).arg(y->getName()).arg(n));
    setFitInfo(info.get(),&fit);
    return std::make_tuple(factor,info);
}

std::tuple<std::shared_ptr<SAVectorDouble>, std::shared_ptr<SATableVariant> >
saFun::polyfit(const QVector<double> &xs, const QVector<double> &ys, int n)
{
    czy::Math::PolyFit fit;
    if(!fit.polyfit(xs.data(),ys.data(),xs.size(),n))
    {
        saFun::setErrorString(TR("can not polyfit"));
        return std::make_tuple(nullptr,nullptr);
    }
    std::shared_ptr<SAVectorDouble> factor = SAValueManager::makeData<SAVectorDouble>();
    const size_t factorSize = fit.getFactorSize();
    factor->clear();
    factor->reserve(factorSize);
    for(size_t f = 0;f < factorSize;++f)
    {
        factor->append(fit.getFactor(f));
    }
    std::shared_ptr<SATableVariant> info = SAValueManager::makeData<SATableVariant>();
    setFitInfo(info.get(),&fit);
    return std::make_tuple(factor,info);
}


std::tuple<std::shared_ptr<SAAbstractDatas> > saFun::polyval(const SAAbstractDatas *wave, const SAVectorDouble *factor)
{
    QVector<double> xs;
    if(saFun::getDoubleVector(wave,xs))
    {
        std::shared_ptr<SAVectorDouble> res = SAValueManager::makeData<SAVectorDouble>(QString("%1_polyFitY").arg(wave->getName()));
        polyval(xs,factor,res.get());
        return std::make_tuple(SAValueManager::castPointToBase(res));
    }
    if(saFun::getPointFVectorXData(wave,xs))
    {
        std::shared_ptr<SAVectorPointF> res = SAValueManager::makeData<SAVectorPointF>(QString("%1_polyFit").arg(wave->getName()));
        polyval(xs,factor,res.get());
        return std::make_tuple(SAValueManager::castPointToBase(res));
    }
    return std::make_tuple(nullptr);
}

void saFun::polyval(const QVector<double> &x, const SAVectorDouble *factor,SAVectorDouble* res)
{
    czy::Math::PolyFit fit;
    fit.setFactors(factor->cbegin(),factor->cend());
    if(res)
    {
        res->resize(x.size());
        fit.getYis(x.begin(),x.end(),res->begin());
    }
}



void saFun::polyval(const QVector<double> &x, const SAVectorDouble *factor, SAVectorPointF *res)
{
    czy::Math::PolyFit fit;
    fit.setFactors(factor->cbegin(),factor->cend());
    if(res)
    {
        QVector<double> ys;
        ys.resize(x.size());
        fit.getYis(x.begin(),x.end(),ys.begin());
        res->setXYValueDatas(x,ys);
    }
}



void saFun::setFitInfo(SATableVariant *info, const czy::Math::PolyFit *fit)
{
    int row=0;
    info->setTableData(row,0,"SSR");info->setTableData(row,1,fit->getSSR());++row;
    info->setTableData(row,0,"SSE");info->setTableData(row,1,fit->getSSE());++row;
    info->setTableData(row,0,"SST");info->setTableData(row,1,fit->getSST());++row;
    info->setTableData(row,0,"RMSE");info->setTableData(row,1,fit->getRMSE());++row;
    info->setTableData(row,0,"RSquare");info->setTableData(row,1,fit->getRSquare());++row;
    info->setTableData(row,0,"Goodness");info->setTableData(row,1,fit->getGoodness());++row;
    info->setTableData(0,2,"factor:");
    for(size_t i=0;i<fit->getFactorSize();++i)
    {
        info->setTableData(i+1,2,QString("factor[%1]").arg(i));
        info->setTableData(i+1,3,fit->getFactor(i));
    }
}
