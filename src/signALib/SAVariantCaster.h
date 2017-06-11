#ifndef SAVARIANTCASTER_H
#define SAVARIANTCASTER_H
#include <QVariant>
#include <QIODevice>
///
/// \brief 扩展QVariant的转换
///
class SAVariantCaster
{
public:
    SAVariantCaster();
    static QString variantToString(const QVariant& var);
    static QVariant stringToVariant(const QString& str,const QString& typeName);
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
};

#endif // SAVARIANTCASTER_H
