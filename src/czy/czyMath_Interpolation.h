#ifndef CZYMATH_INTERPOLATION_H
#define CZYMATH_INTERPOLATION_H
///
/// \date 2014-09-09
/// \author 尘中远
///
#include <czyMath.h>
namespace gsl{
    #include <gsl/gsl_errno.h>
    #include <gsl/gsl_vector.h> /* 提供了向量结构*/
    #include <gsl/gsl_interp.h>
    #include <gsl/gsl_spline.h> /* 样条插值支持 */
}
//#include <QDebug>
namespace czy {
///
/// \brief The Math class 用于处理简单数学计算
///
    namespace Math{
    ///
    /// \brief 插值类
    ///
        using namespace gsl;
        using namespace std;
    ///
    /// \brief 用于插值计算的C++封装
    ///
    /// 假如有已知样本x,y,需要计算xi三次样条插值的内容yi,可以如下：
    /// Interpolation inter;
    /// inter.init(x,y,Interpolation::CSPLINE);
    /// inter.getY(xi);
    ///
        class Interpolation
        {
        private:
            gsl_spline* m_spline;
            gsl_interp_accel *m_accel;
        public:
            enum InterpType{LINEAR,///< 线性插值
                            POLYNOMIAL,///< 多项式插值
                            CSPLINE,///< 三次样条插值
                            CSPLINE_PERIODIC,///< 周期三次样条插值
                            AKIMA,///< 阿基马Akima插值
                            AKIMA_PERIODIC,///< 周期阿基马Akima插值
                           };
            Interpolation():m_spline(nullptr)
              ,m_accel(nullptr)
            {

            }
            ~Interpolation()
            {
                if(m_spline != nullptr)
                    gsl_spline_free (m_spline);
                if(m_accel != nullptr)
                    gsl_interp_accel_free (m_accel);
            }

            ///
            /// \brief 根据x，插值计算y值
            /// \param x 需要计算的x值
            /// \return 返回插值得到的y值
            /// \note 此函数运行前，需要先确保运行了init函数
            ///
            double getY(double x) const
            {
                return gsl_spline_eval(m_spline, x, m_accel);
            }

            void getYs(INPUT const vector<double>& x
                         ,OUTPUT vector<double>& y) const
            {
                auto ite_end = x.end();
                for(auto i=x.begin();i != ite_end;++i)
                {
                    y.push_back(gsl_spline_eval(m_spline, *i, m_accel));
                }
            }
            ///
            /// \brief 初始化，根据样本和插值的类型进行初始化
            /// \param input x 初始的x值
            /// \param input y 初始的y值
            /// \param input type 插值的形式，有以下选择：
            ///
            /// LINEAR 对应 gsl::gsl_interp_linear;
            ///
            /// POLYNOMIAL 对应 gsl::gsl_interp_polynomial;
            ///
            /// CSPLINE 对应 gsl::gsl_interp_cspline;
            ///
            /// CSPLINE_PERIODIC 对应 gsl::gsl_interp_cspline_periodic;
            ///
            /// AKIMA 对应 gsl::gsl_interp_akima;
            ///
            /// AKIMA_PERIODIC 对应 gsl::gsl_interp_akima_periodic.
            ///
            bool init(INPUT const vector<double>& x
                      ,INPUT const vector<double>& y
                      ,INPUT InterpType type)
            {
                size_t length = min(x.size(),y.size());
                if(length <= 0)
                    return false;
                return init(&x[0],&y[0],length,type);
            }
            ///
            /// \brief 初始化，根据样本和插值的类型进行初始化
            /// \param input x 样本的x值
            /// \param input y 样本的y值
            /// \param input length 长度
            /// \param input type 插值的形式
            ///
            bool init(INPUT const double* x
                      ,INPUT const double* y
                      ,INPUT const size_t length
                      ,INPUT InterpType type)
            {
                if(length <= 0)
                    return false;
                //样条插值初始化，根据插值的样式和长度分配空间
                if(m_spline != nullptr)
                {
                    gsl_spline_free (m_spline);//说明spline已经分配了内存
                    m_spline = nullptr;
                }
                m_spline = gsl_spline_alloc (castInterpType2GslInterpType(type), length);
                if(nullptr == m_spline)
                    return false;
                if(m_accel != nullptr)
                {
                    gsl_interp_accel_free (m_accel);
                    m_accel = nullptr;
                }
                m_accel = gsl_interp_accel_alloc();//插值加速，用在gsl_spline_eval里
                if(nullptr == m_accel)
                    return false;
                gsl_spline_init(m_spline,x,y,length);//计算插值的系数
                return true;
            }

            const gsl_interp_type* castInterpType2GslInterpType(INPUT InterpType type) const
            {
                switch(type)
                {
                    case LINEAR:
                        return gsl_interp_linear;
                    case POLYNOMIAL:
                        return gsl_interp_polynomial;
                    case CSPLINE:
                        return gsl_interp_cspline;
                    case CSPLINE_PERIODIC:
                        return gsl_interp_cspline_periodic;
                    case AKIMA:
                        return gsl_interp_akima;
                    case AKIMA_PERIODIC:
                        return gsl_interp_akima_periodic;
                }
                return nullptr;
            }

            ///
            /// \brief interp 插值计算
            /// \param input x 初始的x值
            /// \param input y 初始的y值
            /// \param input type 插值的形式，有以下选择：
            ///
            /// LINEAR 对应 gsl::gsl_interp_linear;
            ///
            /// POLYNOMIAL 对应 gsl::gsl_interp_polynomial;
            ///
            /// CSPLINE 对应 gsl::gsl_interp_cspline;
            ///
            /// CSPLINE_PERIODIC 对应 gsl::gsl_interp_cspline_periodic;
            ///
            /// AKIMA 对应 gsl::gsl_interp_akima;
            ///
            /// AKIMA_PERIODIC 对应 gsl::gsl_interp_akima_periodic.
            ///
            /// \param input new_x 要插值的x值，
            /// \param output new_y 根据插值样式计算的插值结果
            ///
            static bool interp(INPUT const vector<double>& x
                        ,INPUT const vector<double>& y
                        ,INPUT InterpType type
                        ,INPUT const vector<double>& new_x
                        ,OUTPUT vector<double>& new_y)
            {
                switch(type)
                {
                    case LINEAR:
                        return interp(x,y,gsl_interp_linear,new_x,new_y);
                    case POLYNOMIAL:
                        return interp(x,y,gsl_interp_polynomial,new_x,new_y);
                    case CSPLINE:
                        return interp(x,y,gsl_interp_cspline,new_x,new_y);
                    case CSPLINE_PERIODIC:
                        return interp(x,y,gsl_interp_cspline_periodic,new_x,new_y);
                    case AKIMA:
                        return interp(x,y,gsl_interp_akima,new_x,new_y);
                    case AKIMA_PERIODIC:
                        return interp(x,y,gsl_interp_akima_periodic,new_x,new_y);
                }
                return false;
            }
            ///
            /// \brief interp 插值计算
            /// \param input x 初始的x值
            /// \param input y 初始的y值
            /// \param input type 插值的形式，有以下选择：
            ///
            /// gsl::gsl_interp_linear;
            ///
            /// gsl::gsl_interp_polynomial;
            ///
            /// gsl::gsl_interp_cspline;
            ///
            /// gsl::gsl_interp_cspline_periodic;
            ///
            /// gsl::gsl_interp_akima;
            ///
            /// gsl::gsl_interp_akima_periodic.
            ///
            /// \param input new_x 要插值的x值，
            /// \param output new_y 根据插值样式计算的插值结果
            ///
            static bool interp(INPUT const vector<double>& x
                        ,INPUT const vector<double>& y
                        ,INPUT const gsl_interp_type* type
                        ,INPUT const vector<double>& new_x
                        ,OUTPUT vector<double>& new_y)
            {

                size_t length = min(x.size(),y.size());
                if(length <= 0)
                    return false;
                //样条插值初始化，根据插值的样式和长度分配空间
                gsl_spline *spline = gsl_spline_alloc (type, length);
                if(nullptr == spline)
                    return false;
                gsl_interp_accel *acc = gsl_interp_accel_alloc();//插值加速，用在gsl_spline_eval里
                gsl_spline_init(spline,&x[0],&y[0],length);//计算插值的系数
                auto ite_end = new_x.end();
                for(auto i = new_x.begin();i!=ite_end;++i)
                {
                    new_y.push_back(gsl_spline_eval (spline, *i, acc));//获取插值的内容
                }
                gsl_spline_free (spline);
                gsl_interp_accel_free (acc);
                return true;
            }
        };
    }
}
#endif // CZYMATH_INTERPOLATION_H
