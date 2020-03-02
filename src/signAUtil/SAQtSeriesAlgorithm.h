#ifndef SAQTSERIESALGORITHM_H
#define SAQTSERIESALGORITHM_H

#include <QVector>
#include <QStringList>
#include <QVariant>
#include <QPointF>
#include <QDebug>
#include <algorithm>
#include <functional>
#include <vector>
#include <math.h>
#include "SAUtilGlobal.h"

namespace SA {
/**
 * @brief 把一个QStringList的文本转换为double，并存入IT的迭代器中
 *
 * 替代std::transform,此函数可以对转换失败的剔除
 * @param strList 要转换的QStringList
 * @param it_begin 输出的迭代器
 * @note 对于没有转换成功的会忽略，因此最终输出长度可能会小于strlist的长度
 * @code
 * QStringList sl;
 * sl << "1" << "2" << "3.1" << "4e3";
 * std::vector<double> res;
 * res.resize(sl.size());
 * qstringlist_to_double(sl,res.begin());
 * @endcode
 */
template<typename IT>
void qstringlist_to_double(const QStringList& strList,IT it_begin)
{
    int size = strList.size ();
    bool isOK;
    double data;
    for(int i=0;i<size;++i)
    {
        data = strList[i].toDouble (&isOK);
        if(isOK)
        {
            *it_begin = data;
            ++it_begin;
        }
    }
}

///
/// \brief 把序列转换为QStringList,前提是类型可转换
/// \code{.cpp}
///    std::vector<double> d;
///    d.push_back(1);
///    d.push_back(3.5);
///    d.push_back(5.4);
///    d.push_back(7.2);
///    qDebug()<<czy::QArray::array_to_qstringlist(d.begin(),d.end());
///    output：("1", "3.5", "5.4", "7.2")
/// \endcode
/// \param begin 数据开始迭代器
/// \param end 数据结束迭代器
/// \return 转换后的QStringList
///
template<typename IT>
QStringList array_to_qstringlist(INPUT const IT begin,INPUT const IT end)
{
    QStringList strL;
    strL.reserve(end - begin);
    IT ite = begin;
    for(;ite != end;++ite)
    {
        strL.append(QStringLiteral("%1").arg(*ite));
    }
    //std::transform(begin,end,strL.begin(),[](auto& d)->QString{return QStringLiteral("%1").arg(d)});
    //使用transform模板函数需要加多一个参数来代替lambda里的auto，为了更简洁，就不使用transform了
    return strL;
}
///
/// \brief 把序列转换为ARRAY<QVariant>类型的序列，可以是QVector<QVariant>,也可以是,前提是类型可转换
///
/// 如果类型是自定义类型，需要用宏Q_DECLARE_METATYPE告知QMetaType
///
/// \code{.cpp}
///struct MyStructZ
///{
///   int i;
///   ...
///};
///Q_DECLARE_METATYPE(MyStruct)
/// \endcode
/// \param in_begin 需要装换数据开始迭代器
/// \param in_end 需要装换数据结束迭代器
/// \param out_begin 结果数据开始迭代器
/// \param out_end 结果数据结束迭代器
///
template<typename IT_in,typename IT_QVariant>
void array_to_qvariantarray(INPUT const IT_in in_begin,INPUT const IT_in in_end
                        ,OUTPUT IT_QVariant out_begin)
{
    IT_in in_ite;IT_QVariant out_ite;
    for(in_ite = in_begin,out_ite = out_begin
        ;in_ite != in_end
        ;++in_ite,++out_ite)
    {
        *out_ite = QVariant::fromValue(*in_ite);
    }
}

///
/// \brief 把Variant的序列，转换为类型序列,前提是类型可转换
/// \code{.cpp}
///    std::vector<QVariant> d;
///    std::vector<double> res;
///    d.push_back(1);
///    d.push_back(3.5);
///    d.push_back(5.4);
///    d.push_back(7.2);
///    res.resize(d.size());
///    czy::QArray::qvariantarray_to_array<double>(
///                d.begin(),d.end(),res.begin(),res.end());
///    std::for_each(res.begin(),res.end(),[](double d){qDebug()<<d;});
/// \endcode
///
/// output:
/// 1
/// 3.5
/// 5.4
/// 7.2
///
/// \param in_begin Variant数据开始迭代器
/// \param in_end Variant数据结束迭代器
/// \param out_begin 结果数据开始迭代器
/// \param out_end 结果数据结束迭代器
///
template<typename DATA_TYPE,typename IT_in,typename IT_QVariant>
void qvariantarray_to_array(INPUT const IT_QVariant in_begin,INPUT const IT_QVariant in_end
                        ,OUTPUT IT_in out_begin)
{
    std::transform(in_begin,in_end,out_begin
                   ,[](const QVariant& v)->DATA_TYPE{
        return v.value<DATA_TYPE>();
    });
}
///
/// \brief 把qvector<任意类型>数组转换为QVector<QVariant>,前提是类型可转换
/// \param input_v qvector<任意类型>数组
/// \return 转换后的QVector<QVariant>数组
///
template<typename T>
QVector<QVariant> qvector_to_qvectorvariant(const QVector<T>& input_v)
{
    QVector<QVariant> datas;
    datas.resize(input_v.size());
    array_to_qvariantarray(input_v.begin(),input_v.end(),datas.begin());
    return datas;
}
///
/// \brief 把QVector<QVariant>转换为qvector<任意类型>数组,前提是类型可转换
/// \param input_v qvector<任意类型>数组
/// \return 转换后的QVector<QVariant>数组
///
template<typename T>
QVector<T> qvectorvariant_to_qvector(const QVector<QVariant>& input_v)
{
    QVector<T> datas;
    datas.resize(input_v.size());
    qvariantarray_to_array<T>(input_v.begin(),input_v.end()
                         ,datas.begin());
    return datas;
}

/**
 * @brief 把QVector<QPointF>的y提取成一个数组
 * @param points QVector<QPointF>的点集
 * @param begin 需要被提取的数组迭代器
 * @sa czy::SA::split_points_to_ys
 */
template<typename IT>
void get_qvectorpointf_y(const QVector<QPointF>& points,IT begin)
{
    std::transform(points.begin(),points.end(),begin
                   ,[](const QPointF& p)->double{
                       return p.y();
                   });
}


///
/// \brief realCompare 浮点数比较等于算法
/// \param a 浮点数a
/// \param b 浮点数b
/// \param zero 定义的0值，默认为1e-10
/// \return 两个浮点数差值小于zero则定义为相等
///
template<typename DOUBLE_TYPE>
static bool realCompare(const DOUBLE_TYPE& a,const DOUBLE_TYPE& b,const DOUBLE_TYPE& zero = 1e-10)
{
    if(fabs((double)(a - b)) < zero)
    {
        return true;
    }
    return false;
}

template<typename DOUBLE_TYPE>
double angle2radian(DOUBLE_TYPE angle)
{
    return ((double)angle * PI) / 180.0;
}

template<typename DOUBLE_TYPE>
double radian2angle(DOUBLE_TYPE radian)
{
    return ((double)radian * 180.0) / PI;
}

///
/// \brief 把所有点对指定的y值的x轴进行镜像
/// \param begin 点群的开始迭代器
/// \param end 点群的结束迭代器
/// \param mirrorYBase 镜像的x轴的y值，默认为0既是绕着x轴镜像
///
template<typename PointIte,typename DOUBLE_TYPE>
void mirrorByXAxis(PointIte begin,PointIte end,DOUBLE_TYPE mirrorYBase = (double)0)
{
    for(PointIte i = begin;i!=end;++i)
    {
        i->setY(((2.0 * mirrorYBase) - i->y()));
    }
}
///
/// \brief 找到点群的x y最大最小值
/// \param begin 点群迭代器起点
/// \param end 点群迭代器终点
/// \param xMin x最小值
/// \param xMax x最大值
/// \param yMin y最小值
/// \param yMax y最大值
///
template<typename PointIte,typename DOUBLE_TYPE>
void findPointsRang(PointIte begin,PointIte end
                   ,DOUBLE_TYPE& xMin
                   ,DOUBLE_TYPE& xMax
                   ,DOUBLE_TYPE& yMin
                   ,DOUBLE_TYPE& yMax)
{
    xMin = xMax = begin->x();
    yMin = yMax = begin->y();
    for(PointIte i = (begin+1);i!=end;++i)
    {
        if(i->x() < xMin)
        {
            xMin = i->x();
        }
        else if(i->x() > xMax)
        {
            xMax = i->x();
        }

        if(i->y() < yMin)
        {
            yMin = i->y();
        }
        else if(i->y() > yMax)
        {
            yMax = i->y();
        }
    }
}
///
/// \brief 计算保持长宽比下的缩放比例
/// \param base 基本尺寸,适用QSize，QRect,QRectF
/// \param realSize 实际尺寸,适用QSize，QRect,QRectF
/// \return 缩放比例，realSize * 缩放比例 = base，无论长宽总会用最适用的地方撑满base尺寸
///
template<typename SIZE_TYPE>
double calcScale(const SIZE_TYPE& base,const SIZE_TYPE& realSize)
{
    double scale = 1;
    if(realSize.width() > realSize.height())
    {
        //宽度大于高度，宽度方向要和设定的SIZE宽度方向一致
        scale = (double)base.width() / (double)realSize.width();
    }
    else
    {
        //高度大于宽度，高度方向要和设定的pattern高度方向一致
        scale = (double)base.height() / (double)realSize.height();
    }
    return scale;
}

///
/// \brief 计算点的偏移
/// \param base 基准，适用QPoint，QPointF，以及有x(),和y()函数的类
/// \param realPoint 点的位置，适用QPoint，QPointF，以及有x(),和y()函数的类
/// \return 偏移量的x，y，其中  realPoint + 偏移量 = base
///
template<typename POINT>
POINT calcOffset(const POINT& base,const POINT& realPoint)
{
    return POINT(base.x() - realPoint.x(),base.y() - realPoint.y());
}

///
/// \brief 求点b到点a的未开根号距离
/// \param a 点a，适用QPoint，QPointF，以及有x(),和y()函数的类
/// \param b 点b，适用QPoint，QPointF，以及有x(),和y()函数的类
/// \return 返回点b到点a的距离的平方，值开根号后既是距离
///
template<typename POINT>
double pointDistanceUnSqrt(const POINT& a,const POINT& b)
{
    return ((double)b.x() - a.x())*((double)b.x() - a.x())
            + ((double)b.y() - a.y())*((double)b.y() - a.y());
}
///
/// \brief 叉积，主要用于判断2向量的旋转方向,
/// \param a 向量中间点 适用QPoint，QPointF，以及有x(),和y()函数的类
/// \param b 第1点,ba组成第1个向量,适用QPoint，QPointF，以及有x(),和y()函数的类
/// \param c 第2点,ca组成第2个向量,适用QPoint，QPointF，以及有x(),和y()函数的类
/// \return 二维向量A和B的叉积就是A.x * B.y - A.y *B.x ，结果为正就表明A经过顺时针到达B（当然，小于180），反之则逆时针
///
template<class POINT>
double crossProduct(const POINT& base,const POINT& a,const POINT& b)
{
    return (((double)a.x()-base.x())*((double)b.y()-base.y())
            -((double)a.y()-base.y())*((double)b.x()-base.x()));
}

template<typename POINT,typename DATA_TYPE>
POINT pointOffset(const POINT& a,DATA_TYPE offsetX,DATA_TYPE offsetY)
{
    return POINT((a.x() + offsetX) , (a.y() + offsetY));
}
template<typename POINT,typename DATA_TYPE>
void pointOffseted(POINT& a,DATA_TYPE offsetX,DATA_TYPE offsetY)
{
    a.setX(a.x() + offsetX);
    a.setY(a.y() + offsetY);
}
template<typename POINT>
POINT pointScale(const POINT& a,double scale,const POINT& base = POINT(0,0))
{
    return POINT((a.x() - base.x())*scale + base.x(), (a.y() - base.y())*scale+base.y());
}

///
/// \brief 计算顺时针角度
/// 点以时钟12(0)点为基准
/// \param center 中心点
/// \param p 其他点
/// \return 顺时针的角度
///
template<class POINT>
double angleClockwise(const POINT &center, const POINT &p)
{
    double radian = -1;
    int rx = p.x() - center.x();
    int ry = p.y() - center.y();

    if(0 == rx)
    {
        if(0 == ry)
        {
            radian = 0;
        }
        else if(ry > 0)
        {
            radian = 0;
        }
        else
        {
            radian = PI;
        }
    }
    else if(0 == ry)
    {
        if(0 == rx)
        {
            radian = 0;
        }
        else if(rx > 0)
        {
            radian = PI/2.0;
        }
        else
        {
            radian = PI*3.0/2.0;
        }
    }
    else
    {
        double value = (double)(ry) / (double)(rx);
        radian = atan(fabs(value));

        if(rx > 0)
        {
            //1,2 象限
            if(ry > 0)
            {
                //1 象限
                radian = PI/2 - radian;
            }
            else
            {
                radian = PI/2 + radian;
            }
        }
        else
        {
            //3,4 象限
            if(ry > 0)
            {
                //4 象限
                radian = PI*3.0/2.0 + radian;
            }
            else
            {
                //3 象限
                radian = PI*3.0/2.0 - radian;
            }
        }
    }

    //弧度转角度
    return (radian * 180.0 / PI);
}




///
/// \brief 点集变换的仿函数
/// 可以使用std::for_each进行批量变换
///
template<class POINT>
struct ScalePoints
{
    ScalePoints(double scale,const POINT& scaleCenter):m_scale(scale)
      ,m_scaleCenter(scaleCenter)
    {
    }

    void operator ()(POINT& a)
    {
        a = pointScale(a,m_scale,m_scaleCenter);
    }
private:
    double m_scale;
    POINT m_scaleCenter;
};


///
/// \brief 点集乘以同一个系数的仿函数
/// 适用std::for_each
///
template<class POINT,class FACTOR_TYPE>
struct MultipliesFactor
{
    MultipliesFactor(const FACTOR_TYPE& scale)
      :m_factor(scale)
    {
    }
    void operator ()(POINT& a)
    {
        a = (a * m_factor);
    }
private:
    FACTOR_TYPE m_factor;
};

///
/// \brief 点集变换的仿函数
/// 可以使用std::for_each进行批量变换
///
template<class POINT>
struct OffsetPoints
{
    OffsetPoints(const POINT& offset)
    {
        m_offset = offset;
    }
    void operator ()(POINT& a)
    {
        a = pointOffset(a,m_offset.x(),m_offset.y());
    }
private:
    POINT m_offset;
};
///
/// \brief 二维凸包算法的大小排序仿函数
/// 用于找出最左下的点
///
template<class POINT>
struct ComparePositionGraham
{
    bool operator ()(const POINT& a, const POINT& b)
    {
        return (a.y() < b.y()) || (a.y() == b.y() && a.x() < b.x());
    }
};



///
/// \brief 二维凸包算法的点排序仿函数
/// 用于对点集进行排序
///
template<class POINT>
struct CompareAngleGraham
{
    CompareAngleGraham(const POINT& originPoint)
    {
        m_originPoint = originPoint;
    }

    bool operator ()(const POINT& a, const POINT& b)
    {
        int c = crossProduct(m_originPoint, a, b);
        return c > 0 ||
                (c == 0 && (pointDistanceUnSqrt(m_originPoint, a) < pointDistanceUnSqrt(m_originPoint, b)));
    }
private:
    POINT m_originPoint;
};

///
/// \brief 使用Graham 算法求凸包边界
///
template<class POINT>
class DGrahamScan
{
public:
    DGrahamScan()
    {

    }

    void setPointArray(POINT* begin,POINT* end)
    {
        size_t len = std::distance(begin,end);
        m_points.resize(len);
        std::copy(begin,end,m_points.begin());
        std::swap(m_points[0],*(std::min_element(m_points.begin(),m_points.end(),ComparePositionGraham<POINT>())));
        m_originPoint = m_points[0];
        std::sort(m_points.begin()+1, m_points.end(), CompareAngleGraham<POINT>(m_originPoint));
    }

    int getResult(POINT* reslutBegin)
    {

        // 直接把中心點作為起點，開始包覆，逆時針方向。O(N)
        *(m_points.end()-1) = m_points[0];// 讓程式方便處理包至最後一點的情況。
        size_t N = m_points.size();
        typename std::vector<POINT>::iterator begin = m_points.begin();
        int m = 0;// m 為凸包頂點數目
        std::vector<POINT> outline;
        outline.resize(N);
        for (int i=0; i<=N; ++i)
        {
            // 掃除凹陷的點
            while (m >= 2 && crossProduct(outline[m-2], outline[m-1], *(begin+i)) <= 0) m--;
            // 添加新的點
            outline[m++] = *(begin+i);
        }
        m--;	// 最後一個點是重複出現的起點，故要減一。
        std::copy(outline.begin(),outline.begin()+m+1,reslutBegin);
        return m;
    }

public:



private:
    std::vector<POINT> m_points;
    POINT m_originPoint;
};


}
#endif // SAQTSERIESALGORITHM_H
