#ifndef CZYARRAY_H
#define CZYARRAY_H
#include <math.h>
#include <algorithm>
#include <czy.h>
namespace SA
{
/**
 * \brief 此命名空间封装了数组的相关操作
 */
namespace Array {
    ///
    /// \brief 加一个常数的函数对象，类似于std::plus<T>()，不过此函数对象是用于对一个常数进行加法运算
    ///
    template <class T> struct plus_const : public std::unary_function <T,T> {
        plus_const(const T& data){m_data = data;}
      T operator() (const T& x) const {return x+m_data;}
      T m_data;
    };
    ///
    /// \brief 用于静态转换的一元仿函数,T1转换为T2
    ///
    template <class T1,class T2> struct cast_type : public std::unary_function<T1,T2> {
      T2 operator() (const T1& dataToBeCast) {return static_cast<T2>(dataToBeCast);}
    };
    ///
    /// \brief 序列加法运算，用于序列加上单一一个值
    /// \param begin 序列迭代器的起始
    /// \param end 序列迭代器的结尾
    /// \param beAddData 需要进行加法运算的值
    /// \note 此操作会直接修改原有序列值
    ///
    template<typename T,typename IT>
    void add(INPUT IT begin,INPUT IT end,T beAddData)
    {
        std::transform(begin,end,begin,plus_const<T>(beAddData));
    }
    ///
    /// \brief 序列加法运算，两个等长序列相加
    /// \param begin_addfont “加数”序列迭代器的起始
    /// \param end_addfont “加数”序列迭代器的结尾
    /// \param begin_addend “被加数”序列迭代器的起始
    /// \param begin_res 用于存放结果的序列的起始地址
    ///
    template<typename T,typename IT>
    void add(INPUT IT begin_addfont,INPUT IT end_addfont,IT begin_addend,IT begin_res)
    {
        std::transform(begin_addfont,end_addfont,begin_addend,begin_res
                       ,std::plus<T>());
    }
    ///
    /// \brief 序列减法运算，两个等长序列相减
    /// \param begin_addfont “加数”序列迭代器的起始
    /// \param end_addfont “加数”序列迭代器的结尾
    /// \param begin_addend “被加数”序列迭代器的起始
    /// \param begin_res 用于存放结果的序列的起始地址
    ///
    template<typename T,typename IT>
    void minus(INPUT IT begin_minusfont,INPUT IT end_minusfont,IT begin_minusend,IT begin_res)
    {
        std::transform(begin_minusfont,end_minusfont,begin_minusend,begin_res
                       ,std::minus<T>());
    }
    ///
    /// \brief 序列减法运算，用于序列减去单一一个值
    /// \param begin 序列迭代器的起始
    /// \param end 序列迭代器的结尾
    /// \param beAddData 需要进行减法运算的值
    /// \note 此操作会直接修改原有序列值
    ///
    template<typename T,typename IT>
    void minus(INPUT IT begin,INPUT IT end,T beMinusData)
    {
        std::transform(begin,end,begin,plus_const<T>(-beMinusData));
    }

    /**
     * @brief 用于把vector like数组里所有内容转换为另外一种类型输出
     * @param begin 输入迭代器开始地址
     * @param end 输入迭代器结束
     * @param beCastData 输出的开始迭代器
     * @note beCastData长度必须是大于等于end-begin
     */
    template<typename Type,typename Cast2Type,typename Ite1,typename Ite2>
    void transform_cast_type(INPUT Ite1 begin,INPUT Ite1 end,INPUT Ite2 beCastData)
    {
        std::transform(begin,end,beCastData,cast_type<Type,Cast2Type>());
    }

    /**
     * @brief 查找数组的上峰值
     * @param begin 数组的开始迭代器
     * @param end 数组的结束迭代器
     * @param begin_out 输出数组的开始迭代器
     */
    template<typename IT1,typename IT2>
    void find_upper_sharp_peak(INPUT IT1 begin,INPUT IT1 end,INPUT IT2 begin_out)
    {
        if(std::distance(begin,end) < 3)
        {
            return;
        }
        IT1 i=begin+1;
        IT2 o = begin_out;
        for(;(i+1)!=end;++i)
        {
            if((*i > *(i-1)) && (*i > *(i+1)))
            {
                *o = *i;
                ++o;
            }
        }
    }
    /**
     * @brief 查找数组的上峰值
     * @param begin 数组的开始迭代器
     * @param end 数组的结束迭代器
     * @param begin_out 输出数组的开始迭代器
     * @param fun_greater 函数指针 fun_greater(IT1 a,IT1 b) ->bool,如果a>b 返回true
     * @example
     * @code
     * void SAPointSeriesStatisticProcess::getSharpPeakPoint(QVector<QPointF> &sharpPoints
     *                                                       , const QVector<QPointF> &points
     *                                                       , bool isUpperPeak)
     * {
     *     sharpPoints.clear();
     *     sharpPoints.reserve(int(points.size()/2));
     *     if(isUpperPeak)
     *     {
     *         czy::Array::find_upper_sharp_peak(points.begin(),points.end()
     *                                           ,std::back_inserter(sharpPoints)
     *                                           ,[](const QPointF& a,const QPointF& b)->bool{
     *             return a.y() > b.y();
     *         });
     *     }
     *     else
     *     {
     *         czy::Array::find_lower_sharp_peak(points.begin(),points.end()
     *                                           ,std::back_inserter(sharpPoints)
     *                                           ,[](const QPointF& a,const QPointF& b)->bool{
     *             return a.y() < b.y();
     *         });
     *     }
     * }
     * @endcode
     */
    template<typename IT1,typename IT2,typename FUN>
    void find_upper_sharp_peak(INPUT IT1 begin,INPUT IT1 end,INPUT IT2 begin_out,FUN fun_greater)
    {
        if(std::distance(begin,end) < 3)
        {
            return;
        }
        IT1 i=begin+1;
        IT2 o = begin_out;
        for(;(i+1)!=end;++i)
        {
            if(fun_greater(*i,*(i-1)) && fun_greater(*i,*(i+1)))
            {
                *o = *i;
                ++o;
            }
        }
    }
    /**
     * @brief 查找数组的下峰值
     * @param begin 数组的开始迭代器
     * @param end 数组的结束迭代器
     * @param begin_out 输出数组的开始迭代器
     */
    template<typename IT1,typename IT2>
    void find_lower_sharp_peak(INPUT IT1 begin,INPUT IT1 end,INPUT IT2 begin_out)
    {
        if(std::distance(begin,end) < 3)
        {
            return;
        }
        IT1 i=begin+1;
        IT2 o = begin_out;
        for(;(i+1)!=end;++i)
        {
            if((*i < *(i-1)) && (*i < *(i+1)))
            {
                *o = *i;
                ++o;
            }
        }
    }
    /**
     * @brief 查找数组的下峰值
     * @param begin 数组的开始迭代器
     * @param end 数组的结束迭代器
     * @param begin_out 输出数组的开始迭代器
     * @param fun_small 函数指针 fun_small(IT1 a,IT1 b) ->bool,如果a<b 返回true
     * @example
     * @code
     * void SAPointSeriesStatisticProcess::getSharpPeakPoint(QVector<QPointF> &sharpPoints
     *                                                       , const QVector<QPointF> &points
     *                                                       , bool isUpperPeak)
     * {
     *     sharpPoints.clear();
     *     sharpPoints.reserve(int(points.size()/2));
     *     if(isUpperPeak)
     *     {
     *         czy::Array::find_upper_sharp_peak(points.begin(),points.end()
     *                                           ,std::back_inserter(sharpPoints)
     *                                           ,[](const QPointF& a,const QPointF& b)->bool{
     *             return a.y() > b.y();
     *         });
     *     }
     *     else
     *     {
     *         czy::Array::find_lower_sharp_peak(points.begin(),points.end()
     *                                           ,std::back_inserter(sharpPoints)
     *                                           ,[](const QPointF& a,const QPointF& b)->bool{
     *             return a.y() < b.y();
     *         });
     *     }
     * }
     * @endcode
     */
    template<typename IT1,typename IT2,typename FUN>
    void find_lower_sharp_peak(INPUT IT1 begin,INPUT IT1 end,INPUT IT2 begin_out,FUN fun_small)
    {
        if(std::distance(begin,end) < 3)
        {
            return;
        }
        IT1 i=begin+1;
        IT2 o = begin_out;
        for(;(i+1)!=end;++i)
        {
            if(fun_small(*i,*(i-1)) && fun_small(*i,*(i+1)))
            {
                *o = *i;
                ++o;
            }
        }
    }
}
}
#endif // CZYARRAY_H
