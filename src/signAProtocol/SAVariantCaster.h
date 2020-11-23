#ifndef SAVARIANTCASTER_H
#define SAVARIANTCASTER_H
#include <QVariant>
#include <QIODevice>
#include <QDataStream>
#include "SAProtocolGlobal.h"

/**
 * @brief 浮点数比较大小
 *
 * 此函数为了解决qFuzzyCompare过于精细问题，对于double的文本转换，会经常判断异常，使用此函数判断较为简便
 * 原理是|a-b| < esp 认为相等，可以适当调整esp的精度
 * @code
 * return (qAbs(a-b) < esp);
 * @endcode
 *
 * @param a 浮点数a
 * @param b 浮点数b
 * @param esp a,b差值对比结果
 * @return 近似相等返回true
 *
 */
bool SA_PROTOCOL_EXPORT saFuzzyCompare(double a, double b, double esp = 1e-6);

/**
 * @brief doubleToString 较为精确的把double转换为string
 * 对于QString::number(a,'g'),在double太大时,转换出的结果默认保留6位时会出现如下情况：
 *
 * @code
 * double dtest = 1019299.11232;
 * QString str = QString::number(dtest);
 * @endcode
 *
 * 输出：QString
 * 再读取回来：
 * @code
 * double dread = str.toDouble();
 * @endcode
 *
 * 这时两者通过qFuzzyCompare比较是不通过的：
 * @code
 * qFuzzyCompare(dtest,dread);
 * @endcode
 * 返回false,用saFuzzyCompare结果一样
 *
 * 两者相差：
 * @code
 * qDebug() << "qAbs(dtest2 - b)" << qAbs(dtest2-b);
 * @endcode
 * 输出为0.88768
 * 因此对特别大的数和特别小的数，在转换时都需要留意
 *
 * qFuzzyCompare的实现是通过两个浮点数的差值乘以1e12进行：
 * return (qAbs(p1 - p2) * 1000000000000. <= qMin(qAbs(p1),qAbs(p2)));
 * saFuzzyCompare的实现是通过两个浮点数的差值和一个较小的数对比：
 * (qAbs(a-b) < esp); //esp = 1e-6;
 * 要解决大浮点数的问题，需要从转换上下手
 *
 *  这是一个效率较低的转换，它为了识别精度，做了许多处理，好处是你不用管他要显示多少位，此函数会把
 * 足够精度的浮点数显示出来
 * @param a double
 * @return 精确的浮点数
 */
QString SA_PROTOCOL_EXPORT doubleToString(const double a);

///
/// \brief 扩展QVariant到字符的转换
///
class SA_PROTOCOL_EXPORT SAVariantCaster
{
public:
    SAVariantCaster();
    ///
    /// \brief QVariant转为QString
    ///
    /// 此转换会以尽量可以明文的形式把QVariant转换为字符串，有些如QByteArray，无法用明文的，会转换为Base64进行保存
    /// \param var QVariant值
    /// \return 转换好的字符串
    /// \see stringToVariant
    ///
    static QString variantToString(const QVariant& var);

    ///
    /// \brief variantToString的逆方法
    /// \param var 字面值
    /// \param typeName 参数类型
    /// \return 根据字面值转换回来的QVariant
    ///
    static QVariant stringToVariant(const QString& var, const QString& typeName);

    ///
    /// \brief 把QVariant转换为Base64字符
    /// \param var
    /// \return
    ///
    template<typename T>
    static QString converVariantToBase64String(const QVariant& var)
    {
        if (var.canConvert<T>()) {
            QByteArray byte;
            QDataStream st(&byte, QIODevice::ReadWrite);
            T ba = var.value<T>();
            st << ba;
            return (QString(byte.toBase64()));
        }
        return (QString());
    }


    ///
    /// \brief 把Base64字符转换为对应变量
    /// \param base64
    /// \return
    ///
    template<typename T>
    static QVariant converBase64StringToVariant(const QString& base64)
    {
        QByteArray byte = QByteArray::fromBase64(base64.toLocal8Bit());
        QDataStream st(&byte, QIODevice::ReadWrite);
        T ba;

        st >> ba;
        return (QVariant::fromValue(ba));
    }
};

#endif // SAVARIANTCASTER_H
