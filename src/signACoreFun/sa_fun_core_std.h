#ifndef SACOREFUNCTION_IMPL
#define SACOREFUNCTION_IMPL
#include "SACoreFunGlobal.h"

#include <QList>
#include <QString>
#include "SAAbstractDatas.h"
#include "SAValueManager.h"
#include "SAVectorDouble.h"
#include "SAVariantDatas.h"
#include "SADataConver.h"

///
/// \brief transform 类似std::transform
/// 与std的不同是它支持 a(1) transform b(n) 以及 a(n) transform b(1) 和 a(n) transform b(n)
/// \param a
/// \param b
/// \param fun 二元函数[](const double& a,const double& b)->double{}
/// \param info
/// \return
///
template<class FUN_2>
SA_CORE_FUN__EXPORT std::shared_ptr<SAAbstractDatas> double_transform(SAAbstractDatas* a,SAAbstractDatas* b,FUN_2 fun)
{
    std::shared_ptr<SAAbstractDatas> res = nullptr;
    if(SA::Dim1 == a->getDim() && SA::Dim1 == b->getDim())
    {
        if(a->getSize(SA::Dim1) != b->getSize(SA::Dim1))
        {
            return nullptr;
        }
        QVector<double> va,vb;
        if(!SADataConver::converToDoubleVector(a,va))
        {
            return nullptr;
        }
        if(!SADataConver::converToDoubleVector(b,vb))
        {
            return nullptr;
        }
        if(va.size() != vb.size())
        {
            return nullptr;
        }
        //相加同时把结果给a
        std::transform(va.begin(),va.end(),vb.begin(),va.begin(),fun);
        res = std::static_pointer_cast<SAAbstractDatas>(SAValueManager::makeData<SAVectorDouble>("tmp_double_transform",va));
        return res;
    }
    else if(SA::Dim1 == a->getDim() && SA::Dim0 == b->getDim())
    {
        QVector<double> va;
        if(!SADataConver::converToDoubleVector(a,va))
        {
            return nullptr;
        }
        if(va.size() <= 0)
        {
            return nullptr;
        }
        QVariant varb = b->getAt(0);
        bool isOK = false;
        double vb = varb.toDouble(&isOK);
        if(!isOK)
        {
            return nullptr;
        }
        //相加同时把结果给a
        std::for_each(va.begin(),va.end(),[vb,fun](double& _a){
            _a = fun(_a,vb);
        });
        res = std::static_pointer_cast<SAAbstractDatas>(SAValueManager::makeData<SAVectorDouble>("tmp_double_transform",va));
        return res;
    }
    else if(SA::Dim0 == a->getDim() && SA::Dim1 == b->getDim())
    {
        QVector<double> vb;
        if(!SADataConver::converToDoubleVector(b,vb))
        {
            return nullptr;
        }
        if(vb.size() <= 0)
        {
            return nullptr;
        }
        QVariant vara = a->getAt(0);
        bool isOK = false;
        double va = vara.toDouble(&isOK);
        if(!isOK)
        {
            return nullptr;
        }
        //相加同时把结果给a
        std::for_each(vb.begin(),vb.end(),[va,fun](double& _b){
            _b = fun(va,_b);
        });
        res = std::static_pointer_cast<SAAbstractDatas>(SAValueManager::makeData<SAVectorDouble>("tmp_double_transform",vb));
        return res;
    }
    else if(SA::Dim0 == a->getDim() && SA::Dim0 == b->getDim())
    {
        QVariant vara = a->getAt(0);
        QVariant varb = b->getAt(0);
        bool isOK = false;
        double va = vara.toDouble(&isOK);
        if(!isOK)
        {
            return nullptr;
        }
        double vb = varb.toDouble(&isOK);
        if(!isOK)
        {
            return nullptr;
        }
        res = std::static_pointer_cast<SAAbstractDatas>(SAValueManager::makeData<SAVariantDatas>(va+vb));
        res->setName("tmp_double_transform");
        return res;
    }
    return nullptr;
}

#endif // SACOREFUNCTION_IMPL

