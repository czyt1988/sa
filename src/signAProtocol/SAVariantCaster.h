#ifndef SAVARIANTCASTER_H
#define SAVARIANTCASTER_H
#include <QVariant>
#include <QIODevice>
#include "SAProtocolGlobal.h"
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
        if(var.canConvert<T>())
        {
            QByteArray byte;
            QDataStream st(&byte,QIODevice::ReadWrite);
            T ba = var.value<T>();
            st << ba;
            return QString(byte.toBase64());
        }
        return QString();
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
        QDataStream st(&byte,QIODevice::ReadWrite);
        T ba;
        st >> ba;
        return QVariant::fromValue(ba);
    }


};

#endif // SAVARIANTCASTER_H
