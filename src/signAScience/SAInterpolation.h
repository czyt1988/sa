#ifndef SAINTERPOLATION_H
#define SAINTERPOLATION_H
#include "SAScienceGlobal.h"
#include <algorithm>
#include <vector>
namespace SA {

SA_IMPL_FORWARD_DECL(SAInterpolation)

class SASCIENCE_API SAInterpolation
{
    SA_IMPL(SAInterpolation)
public:
    enum InterpType{LINEAR,///< 线性插值
                    POLYNOMIAL,///< 多项式插值
                    CSPLINE,///< 三次样条插值
                    CSPLINE_PERIODIC,///< 周期三次样条插值
                    AKIMA,///< 阿基马Akima插值
                    AKIMA_PERIODIC,///< 周期阿基马Akima插值
                   };
    SAInterpolation();
    ~SAInterpolation();
    //初始化插值
    bool init(INPUT const double* x
              ,INPUT const double* y
              ,INPUT const size_t length
              ,INPUT InterpType type);
    //此函数会创建临时空间，如果是能直接获取double指针，直接使用第一种方法
    template<typename ITE_DOUBLELIKE_X,typename ITE_DOUBLELIKE_Y>
    bool init(INPUT ITE_DOUBLELIKE_X xbegin,INPUT ITE_DOUBLELIKE_X xend
              ,INPUT ITE_DOUBLELIKE_Y ybegin,INPUT ITE_DOUBLELIKE_Y yend
              ,INPUT InterpType type);
    //根据x，插值计算y值
    double getY(double x) const;
    //批量获取插值结果
    template<typename ITE_DOUBLELIKE_X,typename ITE_DOUBLELIKE_Y>
    void getYs(INPUT ITE_DOUBLELIKE_X xbegin,INPUT ITE_DOUBLELIKE_X xend
               ,OUTPUT ITE_DOUBLELIKE_Y ybegin);
public:
    //插值的静态函数
    static bool interp(INPUT const double* x,
                       INPUT const double* y,
                       INPUT size_t len,
                       INPUT InterpType type,
                       INPUT const double* newx,
                       INPUT size_t newlen,
                       OUTPUT double* newy
                       );
};

} // namespace SA

template<typename ITE_DOUBLELIKE_X, typename ITE_DOUBLELIKE_Y>
bool SA::SAInterpolation::init(ITE_DOUBLELIKE_X xbegin, ITE_DOUBLELIKE_X xend, ITE_DOUBLELIKE_Y ybegin, ITE_DOUBLELIKE_Y yend, SAInterpolation::InterpType type)
{
    auto len = std::min(std::distance(xbegin,xend),std::distance(ybegin,yend));
    std::vector<double> xs(len),ys(len);
    std::copy(xbegin,xbegin+len,xs.begin());
    std::copy(ybegin,ybegin+len,ys.begin());
    return init(xs.data(),ys.data(),len,type);
}


template<typename ITE_DOUBLELIKE_X, typename ITE_DOUBLELIKE_Y>
void SA::SAInterpolation::getYs(ITE_DOUBLELIKE_X xbegin, ITE_DOUBLELIKE_X xend, ITE_DOUBLELIKE_Y ybegin)
{
    while(xbegin < xend)
    {
        *ybegin = getY(*xbegin);
        ++xbegin;
        ++ybegin;
    }
}

#endif // SAINTERPOLATION_H
