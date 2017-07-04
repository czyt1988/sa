#ifndef SAVARIANTCASTER_H
#define SAVARIANTCASTER_H
#include <QVariant>
#include <QIODevice>
#include "SALibGlobal.h"
///
/// \brief 扩展QVariant的转换
///
class SALIB_EXPORT SAVariantCaster
{
public:
    SAVariantCaster();
    static QString variantToString(const QVariant& var);
    static QVariant stringToVariant(const QString& var, const QString& typeName);
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
    template<typename T>
    static QVariant converBase64StringToVariant(const QString& base64)
    {
        QByteArray byte;
        byte.fromBase64(base64.toLocal8Bit());
        QDataStream st(&byte,QIODevice::ReadWrite);
        T ba;
        st >> ba;
        return QVariant::fromValue(ba);
    }
};

#endif // SAVARIANTCASTER_H
