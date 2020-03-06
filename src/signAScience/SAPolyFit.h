#ifndef SAPOLYFIT_H
#define SAPOLYFIT_H
#include "SAMath.h"
#include "SAScienceGlobal.h"

namespace SA {
SA_IMPL_FORWARD_DECL(SAPolyFit)
/**
 * @brief 多项式拟合相关类，内部使用gsl进行拟合
 *
 * 例子：
 * @code
 *
 * QVector<double> xs,ys;
 * ......
 * SA::SAPolyFit fit;
 * if(!fit.polyfit(xs.data(),ys.data(),xs.size(),n))
 * {
 *  return;
 * }
 * const size_t factorSize = fit.getFactorSize();
 * QVector<double> factor;
 * for(size_t f = 0;f < factorSize;++f)
 * {
 *  factor->append(fit.getFactor(f));
 * }
 * double rs = fit.getRSquare();
 * double g = fit.getGoodness();
 * @endcode
 *
 * 或者通过设置参数来获取对应的y值，不进行任何拟合操作：
 *
 * @code
 * QVector<double> factor;
 * QVector<double> xs;
 * ......//factor = {} ... xs = {} ...
 *
 * QVector<double> ys;
 * ys.resize(xs.size());
 * SA::SAPolyFit fit;
 * fit.setFactors(factor->cbegin(),factor->cend());
 * fit.getYis(xs.begin(),xs.end(),ys.begin());
 * //ys
 * @endcode
 */
class SASCIENCE_API SAPolyFit
{
    SA_IMPL(SAPolyFit)
public:
    typedef size_t FACTOR_TYPE;
    SAPolyFit();
    ~SAPolyFit();
    //判断当前是否是有效的计算
    bool isValid() const;
    //获取拟合的系数
    double getFactor(FACTOR_TYPE n) const;
    //获取系数的个数
    size_t getFactorSize() const;
    //线性拟合
    bool linearFit(const double *x,const double *y,size_t n);
    ///
    /// \brief 线性拟合
    /// \param x_begin 拟合的x值开始迭代器
    /// \param x_end 拟合的x值结束迭代器
    /// \param y_begin 拟合的y值开始迭代器
    /// \return
    /// \note 需要保证y的长度和x一致或者超过x的长度
    ///
    template<typename ITE_X,typename ITE_Y>
    bool linearFit(ITE_X x_begin,ITE_X x_end,ITE_Y y_begin);
    //多项式拟合
    static int polyfit(const double *x
        ,const double *y
        ,size_t xyLength
        ,unsigned poly_n
        ,std::vector<double>& out_factor
        ,double& out_chisq);//拟合曲线与数据点的优值函数最小值 ,χ2 检验
    //多项式拟合
    bool polyfit(const double *x,const double *y
        ,size_t xyLength,unsigned poly_n);
    //获取y值
    double getYi(double x) const;
    //根据拟合的系数，传入x值，计算y值
    template<typename ITX,typename ITY>
    void getYis(ITX x_first,ITX x_end,ITY res_fisrt) const;
    //获取斜率
    double getSlope() const;
    //获取截距
    double getIntercept() const;
    //
    double getSSR() const;
    //
    double getSSE() const;
    //
    double getSST() const;
    //
    double getRMSE() const;
    //
    double getRSquare() const;
    //
    double getGoodness() const;
    //
    template<typename ITFactorFirst,typename ITFactorEnd>
    void setFactors(ITFactorFirst first,ITFactorEnd end);
    //设置多项式的因子，用于获取拟合值
    void setFactor(FACTOR_TYPE order,double f);
public:
    //计算拟合的显著性
    static void getDeterminateOfCoefficient(
        const double* y,const double* yi,size_t length
        ,double& out_ssr,double& out_sse,double& out_sst,double& out_rmse,double& out_RSquare);
    //linearFit 线性拟合的静态函数
    static int linearFit(const double *x
                         ,const size_t xstride
                         ,const double *y
                         ,const size_t ystride
                         ,size_t n
                         ,double& out_intercept
                         ,double& out_slope
                         ,double& out_interceptErr
                         ,double& out_slopeErr
                         ,double& out_cov
                         ,double& out_wssr);
private:
    void clearAll();
    void clearFactor();

};

template<typename ITFactorFirst, typename ITFactorEnd>
void SAPolyFit::setFactors(ITFactorFirst first, ITFactorEnd end)
{
    clearFactor();
    const size_t size = std::distance(first,end);
    for(FACTOR_TYPE i=0;i<size;++i,++first)
    {
        setFactor(i,*(first));
    }
}

template<typename ITX, typename ITY>
void SAPolyFit::getYis(ITX x_first, ITX x_end, ITY res_fisrt) const
{
    while(x_first != x_end)
    {
        *res_fisrt = getYi(*x_first);
        ++x_first;
        ++res_fisrt;
    }
}

template<typename ITE_X, typename ITE_Y>
bool SAPolyFit::linearFit(ITE_X x_begin, ITE_X x_end, ITE_Y y_begin)
{
    size_t n = std::distance(x_begin,x_end);
    return linearFit(x_begin,y_begin,n);
}
}


#endif // SAPOLYFIT_H
