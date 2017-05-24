#ifndef CZYMATH_H
#define CZYMATH_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <functional>
#include <math.h>
#include <memory>
#include <iterator>
#include "czy.h"


namespace czy{

///
/// \brief The Math namespace 用于处理简单数学计算
///
namespace Math
{
    ///
    /// \brief 对数组求和
    /// \code
    /// typedef  double DATA_TYPE;
    /// vector<DATA_TYPE> datas;
    /// cout << "datas:" << endl;
    /// for(int i=0;i<10;++i)
    /// {
    ///     datas.push_back(i);
    ///     cout << datas[i] << ",";
    /// }
    /// cout << endl;
    /// double res = czy::Math::sum(data.begin(),data.end());
    /// cout << "sum test:" << res << endl;
    /// //45
    /// \endcode
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    ///
    template <typename IT>
    double sum(INPUT const IT _begin,INPUT const IT _end)
    {
        IT it = _begin;
        double total(0.0);
        for(;it!=_end;++it){
            total += *it;
        }
        return total;
    }
    ///
    /// \brief 求平均值的模版函数
    /// \code
    /// void mean()
    /// {
    ///     typedef  double DATA_TYPE;
    ///     vector<DATA_TYPE> datas;
    ///     cout << endl;
    ///     cout << "datas:" << endl;
    ///     for(int i=0;i<10;++i)
    ///     {
    ///         datas.push_back(i);
    ///         cout << datas[i] << ",";
    ///     }
    ///     cout << endl;
    ///     double res = czy::Math::mean(datas.begin(),datas.end());
    ///     cout << "mean test:" << res << endl;
    ///     //4.5
    /// }
    /// \endcode
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    ///
    template <typename IT>
    double mean(INPUT const IT _begin,INPUT const IT _end)
    {
        return sum(_begin,_end)/(_end - _begin);
    }
    ///
    /// \brief 求序列的方差 - 为n-1类型既是序列是随机抽样不是固定值
    /// \code
    /// void var()
    /// {
    ///     typedef  double DATA_TYPE;
    ///     vector<DATA_TYPE> datas;
    ///     cout << endl;
    ///     cout << "datas:" << endl;
    ///     for(int i=0;i<10;++i)
    ///     {
    ///         datas.push_back(i);
    ///         cout << datas[i] << ",";
    ///     }
    ///     cout << endl;
    ///     double res = czy::Math::var(datas.begin(),datas.end());
    ///     cout << "var test:" << res << endl;
    ///     //9.1667
    /// }
    /// \endcode
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    ///
    template <typename IT>
    double var(INPUT const IT _begin,INPUT const IT _end)
    {
        double m = Math::mean(_begin,_end);
        double d(0);
        IT it = _begin;
        for(;it!=_end;++it)
        {
            d += ((m - (*it)) * (m - (*it)));
        }
        size_t length = std::distance(_begin,_end);
        if(length>1)
            length -= 1;//随机序列的方差要减去1
        return d/length;
    }
    ///
    /// \brief 求序列的标准差 - 为n-1类型既是序列是随机抽样不是固定值
    /// \code
    /// void std_var()
    /// {
    ///     typedef  double DATA_TYPE;
    ///     vector<DATA_TYPE> datas;
    ///     cout << endl;
    ///     cout << "datas:" << endl;
    ///     for(int i=0;i<10;++i)
    ///     {
    ///         datas.push_back(i);
    ///         cout << datas[i] << ",";
    ///     }
    ///     cout << endl;
    ///     double res = czy::Math::std_var(datas.begin(),datas.end());
    ///     cout << "std_var test:" << res << endl;
    ///     //3.02765
    /// }
    /// \endcode
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    ///
    template <typename IT>
    double std_var(INPUT const IT _begin,INPUT const IT _end)
    {
        double v = var(_begin,_end);
        return sqrt(v);
    }
    ///
    /// \brief 求序列的n阶中心矩
    ///
    /// 均值为1阶中心矩，方差为特殊的2阶矩
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    /// \param order 阶数
    ///
    template <typename IT>
    double central_moment(INPUT const IT _begin,INPUT const IT _end,INPUT unsigned order)
    {
        double m = mean(_begin,_end);
        double tmp(0),res(0);
        size_t length = _end - _begin;
        for(IT it = _begin;it!=_end;++it)
        {
            tmp = ((*it) - m);
            res += pow(tmp,int(order));
        }
        res /= length;
        return res;
    }
    ///
    /// \brief 求序列的n阶原点矩
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    /// \param order 阶数
    ///
    template <typename IT>
    double origin_moment(INPUT const IT _begin,INPUT const IT _end,INPUT unsigned order)
    {
        double res(0);
        size_t length = _end - _begin;
        for(IT it = _begin;it!=_end;++it)
        {
            res += pow(*it,int(order));
        }
        res /= length;
        return res;
    }

    ///
    /// \brief 求序列的斜度 - 3次矩
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    ///
    template <typename IT>
    double skewness(INPUT const IT _begin,INPUT const IT _end)
    {
        double s = std_var(_begin,_end);
        double res = central_moment(_begin,_end,3);//先求3阶中心距
        res /= (s*s*s);
        return res;
    }

    ///
    /// \brief 求序列的峰度(峭度) - 4次矩
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    ///
    template <typename IT>
    double kurtosis(INPUT const IT _begin,INPUT const IT _end)
    {
        double v = var(_begin,_end);
        double res = central_moment(_begin,_end,4);//先求4阶中心距
        res /= (v*v);
        return res;
    }
    template <class ForwardIterator>
      std::pair<ForwardIterator,ForwardIterator>
        minmax_element (ForwardIterator first, ForwardIterator last)
      {
#if __cplusplus >= 201103L

        return std::minmax_element(first,last);
#else
        std::pair<ForwardIterator,ForwardIterator> res;
        res.first = std::min_element(first,last);
        res.second = std::max_element(first,last);
        return res;
#endif
      }

    ///
    /// \brief 求序列的峰峰值
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    ///
    template <typename IT>
    double peak_to_peak_value(INPUT const IT _begin,INPUT const IT _end)
    {

        std::pair<IT,IT> minmax = minmax_element(_begin,_end);
        return ((double)(*minmax.second) - (double)(*minmax.first));
    }
    ///
    /// \brief 获取统计参数，包括和，均值，方差，标准差，斜度，峭度
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    /// \param OUTPUT d_sum 序列的和
    /// \param OUTPUT d_mean 序列的均值
    /// \param OUTPUT d_var 序列的方差
    /// \param OUTPUT d_std_var 序列的标准差
    /// \param OUTPUT d_skewness 序列的斜度
    /// \param OUTPUT d_kurtosis 序列的峭度
    ///
    template <typename IT>
    void get_statistics(INPUT const IT _begin,INPUT const IT _end
                       ,OUTPUT double& d_sum
                       ,OUTPUT double& d_mean
                       ,OUTPUT double& d_var
                       ,OUTPUT double& d_std_var
                       ,OUTPUT double& d_skewness
                       ,OUTPUT double& d_kurtosis)
    {
        size_t length = std::distance(_begin,_end);
        IT it = _begin;
        double d(0.0),tmp(0.0);

        for(;it!=_end;++it){
            d += *it;
        }
        d_sum = d;
        //均值
        d_mean = d_sum / length;
        //方差
        for(d=0,it = _begin;it!=_end;++it)
        {
            d += ((d_mean - (*it)) * (d_mean - (*it)));
        }
        if(length>1)
            d_var = d/(length-1);//随机序列的方差要减去1
        else
            d_var = d/length;
        //标准差
        d_std_var = sqrt(d_var);
        //斜度,峭度
        double dk(0.0),tmp2(0.0);
        for(d=0,it = _begin;it!=_end;++it)
        {
            tmp = ((*it) - d_mean);
            tmp2 = tmp*tmp*tmp;
            d += tmp2;
            dk += tmp2*tmp;
        }
        d_skewness /= (length*d_std_var*d_std_var*d_std_var);
        d_kurtosis /= (length*d_var*d_var);
    }


//////////////////////////////////////////////////////////////////////////

    ///
    /// \brief 数据去均值,可用于信号平稳处理
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    /// \return 返回均值
    ///
    template <typename IT>
    double sub_mean(IN_OUTPUT IT _begin,IN_OUTPUT IT _end)
    {
        double m = mean<IT>(_begin,_end);
        for(IT i=_begin;i!=_end;++i)
        {
            (*i) = ((*i) - m);
        }
        return m;
    }

    ///
    /// \brief 获取在n倍sigma范围外的数据索引
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    /// \param n sigma的倍数n 1sigma概率为0.6526 , 2sigma概率为0.9544,3sigma概率为0.9974
    /// \param index 保存找到的索引
    /// \param indexStart 索引开始的序号，默认为0，如果_begin不是begin()，那么就要修改索引的开始序号，否则索引和迭代器对应不上
    ///
    template <typename InputIterator,typename OutputIterator>
    void get_out_n_sigma_rang(INPUT InputIterator _begin,INPUT InputIterator _end,double n
                              ,OUTPUT OutputIterator outIndexBegin
                              ,size_t indexStart = 0)
    {
        double v = std_var(_begin,_end);//计算sigma
        double m = mean(_begin,_end);
        double min = m - n*v;
        double max = m + n*v;
        while(_begin!=_end)
        {
            if((*_begin) > max || (*_begin) < min)
            {
                (*outIndexBegin) = (indexStart);
                ++outIndexBegin;
            }
            ++_begin;
            ++indexStart;
        }
    }
    ///
    /// \brief 获取在n倍sigma范围外和范围内的数据索引
    /// \param _begin 数据开始迭代器
    /// \param _end 数据结束迭代器
    /// \param n sigma的倍数n 1sigma概率为0.6526 , 2sigma概率为0.9544,3sigma概率为0.9974
    /// \param outIndexBegin 保存sigma范围外的索引
    /// \param innerIndexBegin 保存sigma范围内的索引
    /// \param indexStart 索引开始的序号，默认为0，如果_begin不是begin()，那么就要修改索引的开始序号，否则索引和迭代器对应不上
    ///
    template <typename InputIterator,typename OutputIterator>
    void get_n_sigma_rang(INPUT InputIterator _begin,INPUT InputIterator _end,double n
                              ,OUTPUT OutputIterator outIndexBegin
                              ,OUTPUT OutputIterator innerIndexBegin
                          ,size_t indexStart = 0)
    {
        double v = std_var(_begin,_end);//计算sigma
        double m = mean(_begin,_end);
        double min = m - n*v;
        double max = m + n*v;
        while(_begin!=_end)
        {
            if((*_begin) > max || (*_begin) < min)
            {
                (*outIndexBegin) = (indexStart);
                ++outIndexBegin;
            }
            else
            {
                (*innerIndexBegin) = (indexStart);
                ++innerIndexBegin;
            }
            ++_begin;
            ++indexStart;
        }
    }
    ///
    /// \brief 1阶差分运算
    /// \param in_begin 数据开始迭代器
    /// \param in_end 数据结束迭代器
    /// \param out_begin 结果数据开始迭代器
    /// \return 返回实际计算的个数
    /// \note n阶差分后，结果数据会比原始数据少n个点，如果是1阶差分，原始数据100个点，结果数据就是99个点
    ///
    template <typename IT,typename IT2>
    size_t difference(INPUT IT in_begin,INPUT IT in_end,OUTPUT IT2 out_begin)
    {
        size_t count = 0;
        IT j=in_begin;
        IT2 r = out_begin;
        IT j_end = (in_end-1);
        for(;j != j_end;++j,++r)
        {
            (*r) = (*(j+1)) - (*(j));
            ++count;
        }
        return count;
    }
    ///
    /// \brief n阶差分运算
    /// \param in_begin 数据开始迭代器
    /// \param in_end 数据结束迭代器
    /// \param out_begin 结果数据开始迭代器
    /// \param n 差分阶数
    /// \return 返回实际计算的个数
    /// \note n阶差分后，结果数据会比原始数据少n个点，如果是1阶差分，原始数据100个点，结果数据就是99个点
    ///
    template <typename IT,typename IT2>
    size_t difference(INPUT IT in_begin,INPUT IT in_end,OUTPUT IT2 out_begin,unsigned n)
    {
        size_t count = 0;
        for(unsigned i=0;i<n;++i)
        {
            count = difference(in_begin,in_end,out_begin);
        }
        return count;
    }

    ///
    /// \brief 统计序列出现的频率
    /// \param in_begin 数据开始迭代器
    /// \param in_end 数据结束迭代器
    /// \param res 统计结果，key为值，value为出现的频率
    /// \return 统计的个数
    ///
    template <typename IT,typename DATA_TYPE>
    size_t count_frequency(INPUT IT in_begin,INPUT IT in_end,OUTPUT std::map<DATA_TYPE,size_t>& res)
    {

        typename std::map<DATA_TYPE,size_t>::iterator res_i = res.end ();
        size_t c=0;
        for(IT i = in_begin;i!=in_end;++i)
        {
             res_i = res.find (*i);
             if(res_i != res.end ())
                 ++(res_i->second);
             else
             {
                 ++c;
                res[*i] = 1;
             }
        }
        return c;
    }
    ///
    /// \brief 统计序列出现的频率
    /// \param in_begin 数据开始迭代器
    /// \param in_end 数据结束迭代器
    /// \param section 要分的段数
    /// \param sectionRange 分段结果，长度是section+1
    /// \param frequencyCount 频率统计结果，长度是section
    /// \return 统计的个数
    ///
    template <typename IT_INPUT,typename IT_OUTPUT1,typename IT_OUTPUT2>
    void count_frequency(INPUT IT_INPUT in_begin
                           ,INPUT IT_INPUT in_end
                           ,size_t section
                           ,OUTPUT IT_OUTPUT1 sectionRange_begin
                           ,OUTPUT IT_OUTPUT2 frequencyCount_begin)
    {
        std::pair<IT_INPUT,IT_INPUT> ite_pp = minmax_element(in_begin,in_end);
        const double detal = double(*ite_pp.second-*ite_pp.first)/double(section);
        *sectionRange_begin = *ite_pp.first;
        IT_OUTPUT1 sectionIte=sectionRange_begin;
        //typedef std::iterator_traits<typename IT_OUTPUT1>::value_type VT1;
        double last=0;
        for(size_t i=0;i<section;++i)
        {//计算各段
            last = *sectionIte;
            ++sectionIte;
            *sectionIte = last+detal;
        }
        IT_OUTPUT1 sectionEnd=sectionIte+1;
        for(IT_INPUT i = in_begin;i!=in_end;++i)
        {
            sectionIte = std::lower_bound(sectionRange_begin,sectionEnd,*i);
            if(sectionIte!=sectionEnd)
            {
                size_t dis = std::distance(sectionRange_begin,sectionIte);
                if(dis<section)
                    ++(*(frequencyCount_begin+dis));
            }
        }
    }

    ///
    /// \brief 修剪数据
    /// 如果数据大于指定的最大或小于指定的最小值，那么大于最大值的数据将赋最大值，小于最小值的数据将赋最小值
    /// \param in_begin 数据开始迭代器
    /// \param in_end 数据结束迭代器
    /// \param min 数据允许的最小值
    /// \param max 数据允许的最大值
    ///
    template <typename IT_INPUT,typename VALUE_TYPE>
    void clip(INPUT IT_INPUT in_begin,INPUT IT_INPUT in_end,VALUE_TYPE min,VALUE_TYPE max)
    {
        for(;in_begin != in_end;++in_begin)
        {
            if(*in_begin<min)
                *in_begin = min;
            else if(*in_begin>max)
                *in_begin = max;
        }
    }
    ///
    /// \brief 修剪下限数据
    /// 如果小于指定的最小值，那么小于最小值的数据将赋最小值
    /// \param in_begin 数据开始迭代器
    /// \param in_end 数据结束迭代器
    /// \param min 数据允许的最小值
    ///
    template <typename IT_INPUT,typename VALUE_TYPE>
    void clip_bottom(INPUT IT_INPUT in_begin,INPUT IT_INPUT in_end,VALUE_TYPE min)
    {
        for(;in_begin != in_end;++in_begin)
        {
            if(*in_begin<min)
                *in_begin = min;
        }
    }
    ///
    /// \brief 修剪上限数据
    /// 如果数据大于指定的最大值，那么大于最大值的数据将赋最大值
    /// \param in_begin 数据开始迭代器
    /// \param in_end 数据结束迭代器
    /// \param max 数据允许的最大值
    ///
    template <typename IT_INPUT,typename VALUE_TYPE>
    void clip_up(INPUT IT_INPUT in_begin,INPUT IT_INPUT in_end,VALUE_TYPE max)
    {
        for(;in_begin != in_end;++in_begin)
        {
            if(*in_begin>max)
                *in_begin = max;
        }
    }
}
}





#endif // CZYMATH_H
