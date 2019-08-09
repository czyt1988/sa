#ifndef CZYSA_H
#define CZYSA_H
#include <czy.h>
#include <czyMath.h>
#include <czyMath_DSP.h>
#include <vector>
namespace czy
{
namespace SA
{
///
/// \brief 用于把一个点序列拆分成一个x序列，点序列必须有x()方法
///
/// \param in_pointbegin 点序列数据开始迭代器
/// \param in_pointend 点序列数据结束迭代器
/// \param out_pointbeginX x序列开始迭代器
/// \note 由于对输出的x,y没有结束迭代器，因此必须对长度进行预分配
///
template <typename ITPOint,typename ITXY>
void split_points_to_xs(INPUT const ITPOint in_pointbegin,INPUT const ITPOint in_pointend
                 ,OUTPUT ITXY out_pointbeginX)
{
    std::transform(in_pointbegin,in_pointend,out_pointbeginX
                   ,[](const ITPOint& p)->double{
                       return p.x();
                   });
}
///
/// \brief 用于把一个点序列拆分成一个y序列，点序列必须有y()方法
///
/// \param in_pointbegin 点序列数据开始迭代器
/// \param in_pointend 点序列数据结束迭代器
/// \param out_pointbeginY y序列开始迭代器
/// \note 由于对输出的x,y没有结束迭代器，因此必须对长度进行预分配
///
template <typename ITPOint,typename ITXY>
void split_points_to_ys(INPUT const ITPOint in_pointbegin,INPUT const ITPOint in_pointend
                 ,OUTPUT ITXY out_pointbeginY)
{
    ITPOint i(in_pointbegin);
    ITXY yi(out_pointbeginY);
    for(;i!=in_pointend;++i,++yi)
    {
        *yi = i->y();
    }
}
///
/// \brief 用于把一个点序列拆分成一个x序列和y序列，点序列必须有x()方法和y()方法
///
/// \param in_pointbegin 点序列数据开始迭代器
/// \param in_pointend 点序列数据结束迭代器
/// \param out_pointbeginX x序列开始迭代器
/// \param out_pointbeginY y序列开始迭代器
/// \note 由于对输出的x,y没有结束迭代器，因此必须对长度进行预分配
///
template <typename ITPOint,typename ITXY>
void split_points(INPUT const ITPOint in_pointbegin,INPUT const ITPOint in_pointend
                 ,OUTPUT ITXY out_pointbeginX,OUTPUT ITXY out_pointbeginY)
{
    ITPOint i(in_pointbegin);
    ITXY xi(out_pointbeginX);
    ITXY yi(out_pointbeginY);
    for(;i!=in_pointend;++i,++xi,++yi)
    {
        *xi = i->x();
        *yi = i->y();
    }
}

}
}

#endif // CZYSA_H
