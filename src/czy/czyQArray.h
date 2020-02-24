#ifndef CZY
#define CZY
#endif
#ifndef CZY_QARRAY_H
#define CZY_QARRAY_H
#include <QVector>
#include <QStringList>
#include <QVariant>
#include <QPointF>
#include <QDebug>
#include "czyArray.h"
namespace SA
{
/**
 * @brief 此命名空间用于对Qt类型的数组扩展
 *
 * email：czy.t@163.com
 *
 * blog:http://blog.csdn.net/czyt1988
 *
 * @date 2014-02-05
 * @author 尘中远
 *
 */
namespace QArray {

    /**
     * @brief 把一个QStringList的文本转换为double，并存入IT的迭代器中
     * @param strList 要转换的QStringList
     * @param it_begin 输出的迭代器
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
}
}


#endif // QMYARRAYEX_H
