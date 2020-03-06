#include "SAPolyFit.h"
#include <map>
namespace gsl{
    #include <gsl/gsl_fit.h>
    #include <gsl/gsl_cdf.h>    // 提供了 gammaq 函数
    #include <gsl/gsl_vector.h> // 提供了向量结构
    #include <gsl/gsl_matrix.h>
    #include <gsl/gsl_multifit.h>
}
using namespace gsl;

class SA::SAPolyFitPrivate
{
    SA_IMPL_PUBLIC(SAPolyFit)
public:
    SAPolyFitPrivate(SAPolyFit* p);
    std::map<SAPolyFit::FACTOR_TYPE,double> m_factor;//记录各个点的系数，key中0是0次方，1是1次方，value是对应的系数
    std::map<SAPolyFit::FACTOR_TYPE,double> m_err;
    double m_cov;//相关度
    double m_ssr;//回归平方和
    double m_sse;//(剩余平方和)
    double m_rmse;//RMSE均方根误差
    double m_wssr;
    double m_goodness;//基于wssr的拟合优度

};

SA::SAPolyFitPrivate::SAPolyFitPrivate(SAPolyFit *p):q_ptr(p)
{

}

SA::SAPolyFit::SAPolyFit():d_ptr(new SAPolyFitPrivate(this))
{

}

SA::SAPolyFit::~SAPolyFit()
{

}

/**
 * @brief 判断当前是否是有效的计算
 * @return
 */
bool SA::SAPolyFit::isValid() const
{
    return (d_ptr->m_factor.size() > 0);
}

/**
 * @brief 获取拟合的系数
 * @param n 0是0次方，1是1次方，value是对应的系数
 * @return 次幂对应的系数
 */
double SA::SAPolyFit::getFactor(SA::SAPolyFit::FACTOR_TYPE n) const
{
    auto ite = d_ptr->m_factor.find(n);
    if (ite == d_ptr->m_factor.end())
        return 0.0;
    return ite->second;
}

/**
 * @brief 获取系数的个数
 * @return
 */
size_t SA::SAPolyFit::getFactorSize() const
{
    return d_ptr->m_factor.size();
}

///
/// \brief  线性拟合
/// \param x 拟合的x值
/// \param y 拟合的y值
/// \param n x,y值对应的长度
/// \return
///
bool SA::SAPolyFit::linearFit(const double *x, const double *y, size_t n)
{
    clearAll();
    d_ptr->m_factor[0]=0;
    d_ptr->m_err[0]=0;
    d_ptr->m_factor[1]=1;
    d_ptr->m_err[1]=0;
    int r = linearFit(x,1,y,1,n
        ,d_ptr->m_factor[0],d_ptr->m_factor[1],d_ptr->m_err[0],d_ptr->m_err[1],d_ptr->m_cov,d_ptr->m_wssr);
    if (0 != r)
        return false;
    d_ptr->m_goodness = gsl_cdf_chisq_Q(d_ptr->m_wssr/2.0,(n-2)/2.0);//计算优度
    {
        std::vector<double> yi;
        yi.resize(n);
        getYis(x,x+n,yi.begin());
        double t;
        getDeterminateOfCoefficient(y,&yi[0],n,d_ptr->m_ssr,d_ptr->m_sse,t,d_ptr->m_rmse,t);
    }
    return true;
}

/**
 * @brief 多项式拟合
 * @param x
 * @param y
 * @param xyLength
 * @param poly_n 阶次如c0+C1x是1，若c0+c1x+c2x^2则poly_n是2
 * @param out_factor
 * @param out_chisq
 * @return
 */
int SA::SAPolyFit::polyfit(const double *x, const double *y,
                           size_t xyLength,
                           unsigned poly_n,
                           std::vector<double> &out_factor,
                           double &out_chisq)
{
    gsl_matrix *XX = gsl_matrix_alloc(xyLength, poly_n + 1);
    gsl_vector *c = gsl_vector_alloc(poly_n + 1);
    gsl_matrix *cov = gsl_matrix_alloc(poly_n + 1, poly_n + 1);
    gsl_vector *vY = gsl_vector_alloc(xyLength);

    for(size_t i = 0; i < xyLength; i++)
    {
        gsl_matrix_set(XX, i, 0, 1.0);
        gsl_vector_set (vY, i, y[i]);
        for(unsigned j = 1; j <= poly_n; j++)
        {
            gsl_matrix_set(XX, i, j, pow(x[i], int(j) ));
        }
    }
    gsl_multifit_linear_workspace *workspace = gsl_multifit_linear_alloc(xyLength, poly_n + 1);
    int r = gsl_multifit_linear(XX, vY, c, cov, &out_chisq, workspace);
    gsl_multifit_linear_free(workspace);
    out_factor.resize(c->size,0);
    for (size_t i=0;i<c->size;++i)
    {
        out_factor[i] = gsl_vector_get(c,i);
    }

    gsl_vector_free(vY);
    gsl_matrix_free(XX);
    gsl_matrix_free(cov);
    gsl_vector_free(c);

    return r;
}
/**
 * @brief SA::SAPolyFit::polyfit
 * @param x
 * @param y
 * @param xyLength
 * @param poly_n
 * @return
 */
bool SA::SAPolyFit::polyfit(const double *x, const double *y,
                            size_t xyLength,
                            unsigned poly_n)
{
    double chisq;
    std::vector<double> factor;
    int r = polyfit(x,y,xyLength,poly_n,factor,chisq);
    if (0 != r)
        return false;
    d_ptr->m_goodness = gsl_cdf_chisq_Q(chisq/2.0,(xyLength-2)/2.0);//计算优度

    clearAll();
    for (unsigned i=0;i<poly_n+1;++i)
    {
        d_ptr->m_factor[i]=factor[i];
    }
    std::vector<double> yi;
    yi.resize(xyLength);
    getYis(x,x+xyLength,yi.begin());
    double t;//由于没用到，所以都用t代替
    getDeterminateOfCoefficient(y,&yi[0],xyLength,d_ptr->m_ssr,d_ptr->m_sse,t,d_ptr->m_rmse,t);

    return true;
}

/**
 * @brief SA::SAPolyFit::getYi
 * @param x
 * @return
 */
double SA::SAPolyFit::getYi(double x) const
{
    double ans(0);
    for (auto ite = d_ptr->m_factor.begin();ite != d_ptr->m_factor.end();++ite)
    {
        ans += (ite->second)*pow(x,ite->first);
    }
    return ans;
}

/**
 * @brief 获取斜率
 * @return 斜率值
 */
double SA::SAPolyFit::getSlope() const
{
    return d_ptr->m_factor[1];
}

/**
 * @brief 获取截距
 * @return 截距值
 */
double SA::SAPolyFit::getIntercept() const
{
    return d_ptr->m_factor[0];
}

double SA::SAPolyFit::getSSR() const
{
    return d_ptr->m_ssr;
}

double SA::SAPolyFit::getSSE() const
{
    return d_ptr->m_sse;
}

double SA::SAPolyFit::getSST() const
{
    return (d_ptr->m_ssr) + (d_ptr->m_sse);
}

double SA::SAPolyFit::getRMSE() const
{
    return d_ptr->m_rmse;
}

double SA::SAPolyFit::getRSquare() const
{
    return 1.0-(getSSE()/(getSSR()+getSSE()));
}

double SA::SAPolyFit::getGoodness() const
{
    return  d_ptr->m_goodness;
}

void SA::SAPolyFit::setFactor(SA::SAPolyFit::FACTOR_TYPE order, double f)
{
    d_ptr->m_factor[order] = f;
}

/**
 * @brief 计算拟合的显著性
 * @param y
 * @param yi
 * @param length
 * @param out_ssr
 * @param out_sse
 * @param out_sst
 * @param out_rmse
 * @param out_RSquare
 */
void SA::SAPolyFit::getDeterminateOfCoefficient(const double *y, const double *yi,
                                                size_t length,
                                                double &out_ssr,
                                                double &out_sse,
                                                double &out_sst,
                                                double &out_rmse,
                                                double &out_RSquare)
{
    double y_mean = SA::mean(y,y+length);
    out_ssr = 0.0;
    for (size_t i =0;i<length;++i)
    {
        out_ssr += ((yi[i]-y_mean)*(yi[i]-y_mean));
        out_sse += ((y[i] - yi[i])*(y[i] - yi[i]));
    }
    out_sst = out_ssr + out_sse;
    out_rmse = sqrt(out_sse/(double(length)));
    out_RSquare = out_ssr/out_sst;
}

///
/// \brief linearFit 线性拟合的静态函数
/// \param x 数据点的横坐标值数组
/// \param xstride 横坐标值数组索引步长 xstride 与 ystride 的值设为 1，表示数据点集 {(xi,yi)|i=0,1,⋯,n−1} 全部参与直线的拟合；
/// \param y 数据点的纵坐标值数组
/// \param ystride 纵坐标值数组索引步长
/// \param n 数据点的数量
/// \param out_intercept 计算的截距
/// \param out_slope 计算的斜率
/// \param out_interceptErr 计算的截距误差
/// \param out_slopeErr 计算的斜率误差
/// \param out_cov 计算的斜率和截距的相关度
/// \param out_wssr 拟合的wssr值
/// \return
///
int SA::SAPolyFit::linearFit(const double *x,
                             const size_t xstride,
                             const double *y,
                             const size_t ystride,
                             size_t n,
                             double &out_intercept,
                             double &out_slope,
                             double &out_interceptErr,
                             double &out_slopeErr,
                             double &out_cov,
                             double &out_wssr)
{
    return gsl_fit_linear(x,xstride,y,ystride,n
        ,&out_intercept,&out_slope,&out_interceptErr,&out_slopeErr,&out_cov,&out_wssr);
}

void SA::SAPolyFit::clearAll()
{
    d_ptr->m_factor.clear();
    d_ptr->m_err.clear();
}

void SA::SAPolyFit::clearFactor()
{
    d_ptr->m_factor.clear();
}


