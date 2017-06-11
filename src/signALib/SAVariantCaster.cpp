#include "SAVariantCaster.h"
#include <QByteArray>
#include <QDataStream>
#include <QBitArray>
#include <QBitmap>
#include <QDate>
#include <QEasingCurve>
#include <QBrush>
#include <QCursor>
SAVariantCaster::SAVariantCaster()
{

}

QString SAVariantCaster::variantToString(const QVariant &var)
{
    switch(var.type())
    {
    case QVariant::Invalid:
        return QString();
    case QVariant::BitArray:
        return converVariantToBase64String<QBitArray>(var);
    case QVariant::Bitmap:
        return converVariantToBase64String<QBitmap>(var);
    case QVariant::Bool:
        return var.toBool() ? "1" : "0";
    case QVariant::Brush:
        return converVariantToBase64String<QBrush>(var);
    case QVariant::ByteArray:
        return converVariantToBase64String<QByteArray>(var);
    case QVariant::Char:
        return var.toChar();
    case QVariant::Color:
    {
        QColor clr = var.value<QColor>();
        return clr.name(QColor::HexArgb);
    }
    case QVariant::Cursor:
        return converVariantToBase64String<QCursor>(var);
    case QVariant::Date:
    {
        QDate d = var.toDate();
        return d.toString(Qt::ISODate);
    }
    case QVariant::DateTime:
    {
        QDateTime d = var.toDateTime();
        return d.toString(Qt::ISODate);
    }
    case QVariant::Double:
    {
        double d = var.toDouble();
        return QString::number(d);
    }
    case QVariant::EasingCurve:
    {
        return converVariantToBase64String<QEasingCurve>(var);
    }
    }
    return QString();
}

QVariant SAVariantCaster::stringToVariant(const QString &str, const QString &typeName)
{

}
