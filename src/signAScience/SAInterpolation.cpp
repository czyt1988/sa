#include "SAInterpolation.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>




#include "gsl/gsl_errno.h"
#include "gsl/gsl_vector.h" /* 提供了向量结构*/
#include "gsl/gsl_interp.h"
#include "gsl/gsl_spline.h" /* 样条插值支持 */

#include "gsl/gsl_spline.h"

class SA::SAInterpolationPrivate
{
    SA_IMPL_PUBLIC(SAInterpolation)
public:
    SAInterpolationPrivate(SAInterpolation* p);
    ~SAInterpolationPrivate();
    gsl_spline* m_spline;
    gsl_interp_accel *m_accel;
    static const gsl_interp_type* castInterpType2GslInterpType(INPUT SAInterpolation::InterpType type);
};

SA::SAInterpolationPrivate::SAInterpolationPrivate(SAInterpolation *p)
    :q_ptr(p),
     m_spline(nullptr),
     m_accel(nullptr)
{

}

SA::SAInterpolationPrivate::~SAInterpolationPrivate()
{
    if(m_spline != nullptr)
        gsl_spline_free (m_spline);
    if(m_accel != nullptr)
        gsl_interp_accel_free (m_accel);
}

const gsl_interp_type *SA::SAInterpolationPrivate::castInterpType2GslInterpType(SAInterpolation::InterpType type)
{
    switch(type)
    {
        case SAInterpolation::LINEAR:
            return gsl_interp_linear;
        case SAInterpolation::POLYNOMIAL:
            return gsl_interp_polynomial;
        case SAInterpolation::CSPLINE:
            return gsl_interp_cspline;
        case SAInterpolation::CSPLINE_PERIODIC:
            return gsl_interp_cspline_periodic;
        case SAInterpolation::AKIMA:
            return gsl_interp_akima;
        case SAInterpolation::AKIMA_PERIODIC:
            return gsl_interp_akima_periodic;
    }
    return nullptr;
}

SA::SAInterpolation::SAInterpolation():d_ptr(new SAInterpolationPrivate(this))
{

}

SA::SAInterpolation::~SAInterpolation()
{

}

/**
 * @brief 初始化，根据样本和插值的类型进行初始化
 * @param x 样本的x值
 * @param y 样本的y值
 * @param length 长度
 * @param type 插值的形式
 * @return
 */
bool SA::SAInterpolation::init(const double *x,
                               const double *y,
                               const size_t length,
                               SA::SAInterpolation::InterpType type)
{
    if(length <= 0)
        return false;
    gsl_spline* spl = gsl_spline_alloc (d_ptr->castInterpType2GslInterpType(type), length);
    if(nullptr == spl)
        return false;
    gsl_interp_accel* accel = gsl_interp_accel_alloc();//插值加速，用在gsl_spline_eval里
    if(nullptr == accel)
        return false;

    //样条插值初始化，根据插值的样式和长度分配空间
    if(d_ptr->m_spline != nullptr)
    {
        gsl_spline_free (d_ptr->m_spline);//说明spline已经分配了内存
    }
    d_ptr->m_spline = spl;
    if(d_ptr->m_accel != nullptr)
    {
        gsl_interp_accel_free (d_ptr->m_accel);
    }
    d_ptr->m_accel = accel;//插值加速，用在gsl_spline_eval里
    return true;
}

/**
 * @brief 根据x，插值计算y值
 * @param x需要计算的x值
 * @return 返回插值得到的y值
 * @note 此函数运行前，需要先确保运行了init函数
 */
double SA::SAInterpolation::getY(double x) const
{
    return gsl_spline_eval(d_ptr->m_spline, x, d_ptr->m_accel);
}

/**
 * @brief 插值计算
 * @param x 初始的x值
 * @param y 初始的y值
 * @param type 插值的形式 @see SA::SAInterpolation::InterpType
 * @param newx 要插值的x值
 * @param newy 根据插值样式计算的插值结果
 * @return
 */
bool SA::SAInterpolation::interp(const double *x,
                                 const double *y,
                                 size_t len,
                                 SA::SAInterpolation::InterpType type,
                                 const double *newx,
                                 size_t newlen,
                                 double *newy
                                 )
{
    //样条插值初始化，根据插值的样式和长度分配空间
    gsl_spline *spline = gsl_spline_alloc (SAInterpolationPrivate::castInterpType2GslInterpType(type), len);
    if(nullptr == spline)
        return false;
    gsl_interp_accel *acc = gsl_interp_accel_alloc();//插值加速，用在gsl_spline_eval里
    gsl_spline_init(spline,x,y,len);//计算插值的系数
    for(size_t i = 0; i<newlen; ++i)
    {
        newy[i] = gsl_spline_eval (spline, newx[i], acc);
    }
    gsl_spline_free (spline);
    gsl_interp_accel_free (acc);
    return true;
}




