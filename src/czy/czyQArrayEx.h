/*Qt 数组扩展
 *日期：2014-02-05
 *作者：尘中远
 *email：czy.t@163.com
 *blog:http://blog.csdn.net/czyt1988
 */
#ifndef CZY
#define CZY
#endif
#ifndef CZY_QARRAYEX_H
#define CZY_QARRAYEX_H
#include <QVector>
#include <QStringList>
#include <QVariant>
#include <QPointF>
#include <QDebug>
#include <math.h>
#include <algorithm>
#include <czy.h>
namespace czy
{

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

template<typename Type,typename Cast2Type,typename Ite1,typename Ite2>
void transform_cast_type(INPUT Ite1 begin,INPUT Ite1 end,INPUT Ite2 beCastData)
{
    std::transform(begin,end,beCastData,cast_type<Type,Cast2Type>());
}
}

namespace QArray {

    template<typename IT>
    void qStringList2Double(const QStringList& strList,IT it_begin)
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
    ///    qDebug()<<czy::QArray::array2stringList(d.begin(),d.end());
    ///    output：("1", "3.5", "5.4", "7.2")
    /// \endcode
    /// \param begin 数据开始迭代器
    /// \param end 数据结束迭代器
    /// \return 转换后的QStringList
    ///
    template<typename IT>
    QStringList array2stringList(INPUT const IT begin,INPUT const IT end)
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
        return std::move(strL);
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
    void array2arrayVariant(INPUT const IT_in in_begin,INPUT const IT_in in_end
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
    ///    czy::QArray::arrayVariant2array<double>(
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
    void arrayVariant2array(INPUT const IT_QVariant in_begin,INPUT const IT_QVariant in_end
                            ,OUTPUT IT_in out_begin)
    {
        std::transform(in_begin,in_end
                       ,out_begin
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
    QVector<QVariant> vectorType2vectorVariant(const QVector<T>& input_v)
    {
        QVector<QVariant> datas;
        datas.resize(input_v.size());
        array2arrayVariant(input_v.begin(),input_v.end(),datas.begin());
        return std::move(datas);
    }
    ///
    /// \brief 把QVector<QVariant>转换为qvector<任意类型>数组,前提是类型可转换
    /// \param input_v qvector<任意类型>数组
    /// \return 转换后的QVector<QVariant>数组
    ///
    template<typename T>
    QVector<T> vectorVariant2vectorType(const QVector<QVariant>& input_v)
    {
        QVector<T> datas;
        datas.resize(input_v.size());
        arrayVariant2array<T>(input_v.begin(),input_v.end()
                             ,datas.begin());
        return std::move(datas);
    }
}
	class QArrayEx
	{
	public:
		QArrayEx(){

		}
		/// 未验证
		template<typename A,typename B>
		static QVector<B> static_cast_VectorA2VectorB(const QVector<A> input_A_beCasted)
		{
            QVector<B> dataB;
			dataB.reserve(input_A_beCasted.size());
			auto ite_end = input_A_beCasted.end();
			for (auto ite = input_A_beCasted.begin();ite != ite_end;++ite)
			{
                dataB.append(static_cast<B>(*ite));
			}
			return dataB;
		}

		template<typename A,typename B>
		static void static_cast_VectorA2VectorB(const QVector<A>& input_A_beCasted,QVector<B>& output_B)
		{
			output_B.resize(input_A_beCasted.size());
			auto count = input_A_beCasted.size();
			for (auto i=0;i<count;++i)
			{
                output_B[i] = static_cast<B>(input_A_beCasted[i]);
			}
		}
	};
}


#endif // QMYARRAYEX_H
