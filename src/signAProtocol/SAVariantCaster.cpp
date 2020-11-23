#include "SAVariantCaster.h"
#include <QByteArray>
#include <QDataStream>
#include <QBitArray>
#include <QBitmap>
#include <QDate>
#include <QEasingCurve>
#include <QBrush>
#include <QCursor>
#include <QUuid>
#include <QFont>
#include <QIcon>
#include <QImage>
#include <QKeySequence>
#include <QLocale>
#include <QTransform>
#include <QMatrix>
#include <QMatrix4x4>
#include <QPolygon>
#include <QQuaternion>
#include <QPalette>
#include <QPen>
#include <QRegularExpression>
#include <QSizePolicy>
#include <QTextLength>
#include <QTextFormat>
#include <QUrl>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QStringList>

bool saFuzzyCompare(double a, double b, double esp)
{
    return (qAbs(a-b) < esp);
}


SAVariantCaster::SAVariantCaster()
{
}


QString SAVariantCaster::variantToString(const QVariant& var)
{
    switch (var.type())
    {
    case QVariant::Invalid:
        return (QString());

    case QVariant::BitArray:
        return (converVariantToBase64String<QBitArray>(var));

    case QVariant::Bitmap:
        return (converVariantToBase64String<QBitmap>(var));

    case QVariant::Bool:
        return (var.toBool() ? "1" : "0");

    case QVariant::Brush:
        return (converVariantToBase64String<QBrush>(var));

    case QVariant::ByteArray:
        return (converVariantToBase64String<QByteArray>(var));

    case QVariant::Char:
        return (var.toChar());

    case QVariant::Color:
    {
        QColor clr = var.value<QColor>();
        return (clr.name(QColor::HexArgb));
    }

    case QVariant::Cursor:
        return (converVariantToBase64String<QCursor>(var));

    case QVariant::Date:
    {
        QDate d = var.toDate();
        return (d.toString(Qt::ISODate));
    }

    case QVariant::DateTime:
    {
        QDateTime d = var.toDateTime();
        return (d.toString(Qt::ISODate));
    }

    case QVariant::Double:
    {
        double d = var.toDouble();
        return (doubleToString(d));//针对double，用非科学计数法会对小数丢失精度，因此采样g最合理，但小数点较多时需要适当处理
    }

    case QVariant::EasingCurve:
    {
        return (converVariantToBase64String<QEasingCurve>(var));
    }

    case QVariant::Uuid:
    {
        return (var.toUuid().toString());
    }

    case QVariant::Font:
    {
        return (converVariantToBase64String<QFont>(var));
    }

    case QVariant::Hash:
    {
        return (converVariantToBase64String<QVariantHash>(var));
    }

    case QVariant::Icon:
    {
        return (converVariantToBase64String<QIcon>(var));
    }

    case QVariant::Image:
    {
        return (converVariantToBase64String<QImage>(var));
    }

    case QVariant::Int:
    {
        return (QString::number(var.toInt()));
    }

    case QVariant::KeySequence:
    {
        QKeySequence d = var.value<QKeySequence>();
        return (d.toString(QKeySequence::NativeText));
    }

    case QVariant::Line:
    {
        QLine d = var.toLine();
        return (QString("%1;%2;%3;%4")
               .arg(d.x1()).arg(d.y1())
               .arg(d.x2()).arg(d.y2()));
    }

    case QVariant::LineF:
    {
        QLineF d = var.toLineF();
        return (QString("%1;%2;%3;%4")
               .arg(doubleToString(d.x1()))
               .arg(doubleToString(d.y1()))
               .arg(doubleToString(d.x2()))
               .arg(doubleToString(d.y2())));
    }

    case QVariant::List:
    {
        return (converVariantToBase64String<QVariantList>(var));
    }

    case QVariant::Locale:
    {
        return (var.toLocale().name());
    }

    case QVariant::LongLong:
    {
        return (QString::number(var.toLongLong()));
    }

    case QVariant::Map:
    {
        return (converVariantToBase64String<QVariantMap>(var));
    }

    case QVariant::Matrix:
    {
        QMatrix d = var.value<QMatrix>();
        return (QString("%1;%2;%3;%4;%5;%6").arg(d.m11()).arg(d.m12()).arg(d.m21()).arg(d.m22()).arg(d.dx()).arg(d.dy()));
    }

    case QVariant::Transform:
    {
        QTransform d = var.value<QTransform>();
        return (QString("%1;%2;%3;%4;%5;%6;%7;%8;%9")
               .arg(d.m11()).arg(d.m12()).arg(d.m13())
               .arg(d.m21()).arg(d.m22()).arg(d.m23())
               .arg(d.m31()).arg(d.m32()).arg(d.m33()));
    }

    case QVariant::Matrix4x4:
    {
        QMatrix4x4 d = var.value<QMatrix4x4>();
        return (QString("%1;%2;%3;%4;%5;%6;%7;%8;%9;%10;%11;%12;%13;%14;%15;%16")
               .arg(d(0, 0)).arg(d(0, 1)).arg(d(0, 2)).arg(d(0, 3))
               .arg(d(1, 0)).arg(d(1, 1)).arg(d(1, 2)).arg(d(1, 3))
               .arg(d(2, 0)).arg(d(2, 1)).arg(d(2, 2)).arg(d(2, 3))
               .arg(d(3, 0)).arg(d(3, 1)).arg(d(3, 2)).arg(d(3, 3))
               );
    }

    case QVariant::Palette:
    {
        return (converVariantToBase64String<QPalette>(var));
    }

    case QVariant::Pen:
    {
        return (converVariantToBase64String<QPen>(var));
    }

    case QVariant::Pixmap:
    {
        return (converVariantToBase64String<QPixmap>(var));
    }

    case QVariant::Point:
    {
        QPoint d = var.toPoint();
        return (QString("%1;%2").arg(d.x()).arg(d.y()));
    }

    case QVariant::PointF:
    {
        QPointF d = var.toPointF();
        return (QString("%1;%2").arg(doubleToString(d.x())).arg(doubleToString(d.y())));
    }

    case QVariant::Polygon:
    {
        QPolygon d = var.value<QPolygon>();
        QString str;
        if (!d.isEmpty()) {
            str += QString("%1;%2").arg(d[0].x()).arg(d[0].y());
        }
        for (int i = 1; i < d.size(); ++i)
        {
            str += QString("|%1;%2").arg(d[i].x()).arg(d[i].y());
        }
        return (str);
    }

    case QVariant::PolygonF:
    {
        QPolygonF d = var.value<QPolygonF>();
        QString str;
        if (!d.isEmpty()) {
            ////用g,而不用f，f会导致小数位过多，并不适合协议传输，但针对double类型，默认是用f
            str += QString("%1;%2").arg(doubleToString(d[0].x())).arg(doubleToString(d[0].y()));
        }
        //用非科学计数法转换，避免精度的丢失
        for (int i = 1; i < d.size(); ++i)
        {
            str += QString("|%1;%2").arg(doubleToString(d[i].x())).arg(doubleToString(d[i].y()));
        }
        return (str);
    }

    case QVariant::Quaternion:
    {
        return (converVariantToBase64String<QQuaternion>(var));
    }

    case QVariant::Rect:
    {
        QRect d = var.toRect();
        return (QString("%1;%2;%3;%4")
               .arg(d.x()).arg(d.y()).arg(d.width()).arg(d.height()));
    }

    case QVariant::RectF:
    {
        QRectF d = var.toRectF();
        return (QString("%1;%2;%3;%4")
               .arg(doubleToString(d.x()))
               .arg(doubleToString(d.y()))
               .arg(doubleToString(d.width()))
               .arg(doubleToString(d.height())));
    }

    case QVariant::RegExp:
    {
        return (converVariantToBase64String<QRegExp>(var));
    }

    case QVariant::RegularExpression:
    {
        return (converVariantToBase64String<QRegularExpression>(var));
    }

    case QVariant::Region:
    {
        return (converVariantToBase64String<QRegion>(var));
    }

    case QVariant::Size:
    {
        QSize d = var.toSize();
        return (QString("%1;%2")
               .arg(d.width()).arg(d.height()));
    }

    case QVariant::SizeF:
    {
        QSizeF d = var.toSizeF();
        return (QString("%1;%2")
               .arg(doubleToString(d.width()))
               .arg(doubleToString(d.height())));
    }

    case QVariant::SizePolicy:
    {
        return (converVariantToBase64String<QSizePolicy>(var));
    }

    case QVariant::String:
    {
        return (var.toString());
    }

    case QVariant::StringList:
    {
        return (converVariantToBase64String<QStringList>(var));
    }

    case QVariant::TextFormat:
    {
        return (converVariantToBase64String<QTextFormat>(var));
    }

    case QVariant::TextLength:
    {
        return (converVariantToBase64String<QTextLength>(var));
    }

    case QVariant::Time:
    {
        return (var.toTime().toString(Qt::ISODate));
    }

    case QVariant::UInt:
    {
        return (QString::number(var.toUInt()));
    }

    case QVariant::ULongLong:
    {
        return (QString::number(var.toULongLong()));
    }

    case QVariant::Url:
    {
        return (var.toUrl().toString());
    }

    case QVariant::Vector2D:
    {
        return (converVariantToBase64String<QVector2D>(var));
    }

    case QVariant::Vector3D:
    {
        return (converVariantToBase64String<QVector3D>(var));
    }

    case QVariant::Vector4D:
    {
        return (converVariantToBase64String<QVector4D>(var));
    }

    default:
        return (QString());
    }
    return (QString());
}


QVariant SAVariantCaster::stringToVariant(const QString& var, const QString& typeName)
{
    QVariant::Type type = QVariant::nameToType(typeName.toLocal8Bit().data());

    switch (type)
    {
    case QVariant::Invalid:
        return (QVariant());

    case QVariant::BitArray:
        return (converBase64StringToVariant<QBitArray>(var));

    case QVariant::Bitmap:
        return (converBase64StringToVariant<QBitmap>(var));

    case QVariant::Bool:
    {
        bool d = var.toInt();
        return (d);
    }

    case QVariant::Brush:
        return (converBase64StringToVariant<QBrush>(var));

    case QVariant::ByteArray:
        return (converBase64StringToVariant<QByteArray>(var));

    case QVariant::Char:
    {
        if (var.size() <= 0) {
            return (QVariant());
        }
        return (QChar(var[0]));
    }

    case QVariant::Color:
    {
        QColor clr;
        clr.setNamedColor(var);
        return (clr);
    }

    case QVariant::Cursor:
        return (converBase64StringToVariant<QCursor>(var));

    case QVariant::Date:
    {
        QDate d;
        d.fromString(var, Qt::ISODate);
        return (d);
    }

    case QVariant::DateTime:
    {
        QDateTime d;
        d.fromString(var, Qt::ISODate);
        return (d);
    }

    case QVariant::Double:
    {
        double d = var.toDouble();
        return (d);
    }

    case QVariant::EasingCurve:
    {
        return (converBase64StringToVariant<QEasingCurve>(var));
    }

    case QVariant::Uuid:
    {
        QUuid d(var);
        return (d);
    }

    case QVariant::Font:
    {
        return (converBase64StringToVariant<QFont>(var));
    }

    case QVariant::Hash:
    {
        return (converBase64StringToVariant<QVariantHash>(var));
    }

    case QVariant::Icon:
    {
        return (converBase64StringToVariant<QIcon>(var));
    }

    case QVariant::Image:
    {
        return (converBase64StringToVariant<QImage>(var));
    }

    case QVariant::Int:
    {
        return (QString::number(var.toInt()));
    }

    case QVariant::KeySequence:
    {
        QKeySequence d(var, QKeySequence::NativeText);
        return (d);
    }

    case QVariant::Line:
    {
        QStringList list = var.split(';');
        if (list.size() != 4) {
            return (QVariant());
        }
        QLine d(list[0].toInt(), list[1].toInt(), list[2].toInt(), list[3].toInt());
        return (d);
    }

    case QVariant::LineF:
    {
        QStringList list = var.split(';');
        if (list.size() != 4) {
            return (QVariant());
        }
        QLineF d(list[0].toDouble(), list[1].toDouble(), list[2].toDouble(), list[3].toDouble());
        return (d);
    }

    case QVariant::List:
    {
        return (converBase64StringToVariant<QVariantList>(var));
    }

    case QVariant::Locale:
    {
        return (QLocale(var));
    }

    case QVariant::LongLong:
    {
        return (QString::number(var.toLongLong()));
    }

    case QVariant::Map:
    {
        return (converBase64StringToVariant<QVariantMap>(var));
    }

    case QVariant::Matrix:
    {
        QStringList list = var.split(';');
        if (list.size() != 6) {
            return (QVariant());
        }
        QMatrix d(list[0].toDouble(), list[1].toDouble(), list[2].toDouble(), list[3].toDouble()
            , list[4].toDouble(), list[5].toDouble());
        return (d);
    }

    case QVariant::Transform:
    {
        QStringList list = var.split(';');
        if (list.size() != 9) {
            return (QVariant());
        }
        QTransform d(list[0].toDouble(), list[1].toDouble(), list[2].toDouble()
            , list[3].toDouble(), list[4].toDouble(), list[5].toDouble()
            , list[6].toDouble(), list[7].toDouble(), list[8].toDouble());
        return (d);
    }

    case QVariant::Matrix4x4:
    {
        QStringList list = var.split(';');
        if (list.size() != 16) {
            return (QVariant());
        }
        QMatrix4x4 d(list[0].toDouble(), list[1].toDouble(), list[2].toDouble(), list[3].toDouble()
            , list[4].toDouble(), list[5].toDouble(), list[6].toDouble(), list[7].toDouble()
            , list[8].toDouble(), list[9].toDouble(), list[10].toDouble(), list[11].toDouble()
            , list[12].toDouble(), list[13].toDouble(), list[14].toDouble(), list[15].toDouble()
            );
        return (d);
    }

    case QVariant::Palette:
    {
        return (converBase64StringToVariant<QPalette>(var));
    }

    case QVariant::Pen:
    {
        return (converBase64StringToVariant<QPen>(var));
    }

    case QVariant::Pixmap:
    {
        return (converBase64StringToVariant<QPixmap>(var));
    }

    case QVariant::Point:
    {
        QStringList list = var.split(';');
        if (list.size() != 2) {
            return (QVariant());
        }
        QPoint d(list[0].toInt(), list[1].toInt());
        return (d);
    }

    case QVariant::PointF:
    {
        QStringList list = var.split(';');
        if (list.size() != 2) {
            return (QPointF());
        }
        QPointF d(list[0].toDouble(), list[1].toDouble());
        return (d);
    }

    case QVariant::Polygon:
    {
        QPolygon d;
        QStringList list = var.split('|');
        if (0 == list.size()) {
            list = var.split(';');
            if (list.size() == 2) {
                d<<QPoint(list[0].toDouble(), list[1].toDouble());
            }
            return (d);
        }
        for (int i = 0; i < list.size(); ++i)
        {
            QStringList plist = list[i].split(';');
            if (2 == plist.size()) {
                d.append(QPoint(plist[0].toDouble(), plist[1].toDouble()));
            }
        }
        return (d);
    }

    case QVariant::PolygonF:
    {
        QStringList list = var.split('|');
        QPolygonF d;
        if (0 == list.size()) {
            list = var.split(';');
            if (list.size() == 2) {
                d<<QPointF(list[0].toDouble(), list[1].toDouble());
            }
            return (d);
        }
        for (int i = 0; i < list.size(); ++i)
        {
            QStringList plist = list[i].split(';');
            if (2 == plist.size()) {
                d.append(QPointF(plist[0].toDouble(), plist[1].toDouble()));
            }
        }
        return (d);
    }

    case QVariant::Quaternion:
    {
        return (converBase64StringToVariant<QQuaternion>(var));
    }

    case QVariant::Rect:
    {
        QStringList list = var.split(';');
        if (list.size() != 4) {
            return (QVariant());
        }
        QRect d(list[0].toInt(), list[1].toInt(), list[2].toInt(), list[3].toInt());
        return (d);
    }

    case QVariant::RectF:
    {
        QStringList list = var.split(';');
        if (list.size() != 4) {
            return (QVariant());
        }
        QRectF d(list[0].toDouble(), list[1].toDouble(), list[2].toDouble(), list[3].toDouble());
        return (d);
    }

    case QVariant::RegExp:
    {
        return (converBase64StringToVariant<QRegExp>(var));
    }

    case QVariant::RegularExpression:
    {
        return (converBase64StringToVariant<QRegularExpression>(var));
    }

    case QVariant::Region:
    {
        return (converBase64StringToVariant<QRegion>(var));
    }

    case QVariant::Size:
    {
        QStringList list = var.split(';');
        if (list.size() != 2) {
            return (QVariant());
        }
        QSize d(list[0].toInt(), list[1].toInt());
        return (d);
    }

    case QVariant::SizeF:
    {
        QStringList list = var.split(';');
        if (list.size() != 2) {
            return (QVariant());
        }
        QSizeF d(list[0].toDouble(), list[1].toDouble());
        return (d);
    }

    case QVariant::SizePolicy:
    {
        return (converBase64StringToVariant<QSizePolicy>(var));
    }

    case QVariant::String:
    {
        return (var);
    }

    case QVariant::StringList:
    {
        return (converBase64StringToVariant<QStringList>(var));
    }

    case QVariant::TextFormat:
    {
        return (converBase64StringToVariant<QTextFormat>(var));
    }

    case QVariant::TextLength:
    {
        return (converBase64StringToVariant<QTextLength>(var));
    }

    case QVariant::Time:
    {
        QTime t;
        t.fromString(var, Qt::ISODate);
        return (t);
    }

    case QVariant::UInt:
    {
        return (var.toUInt());
    }

    case QVariant::ULongLong:
    {
        return (var.toULongLong());
    }

    case QVariant::Url:
    {
        return (QUrl(var));
    }

    case QVariant::Vector2D:
    {
        return (converBase64StringToVariant<QVector2D>(var));
    }

    case QVariant::Vector3D:
    {
        return (converBase64StringToVariant<QVector3D>(var));
    }

    case QVariant::Vector4D:
    {
        return (converBase64StringToVariant<QVector4D>(var));
    }

    default:
        return (QVariant());
    }
    return (QVariant());
}


QString doubleToString(const double a)
{
    char f = 'g';
    int precision = 6;

    if (a > 1e6) {
        //当数据非常大时，精度需要根据大小进行调整
        int tmp = a/1e6;
        while (tmp / 10 != 0 && precision < 16)// precision不大于16
        {
            tmp /= 10;
            ++precision;
        }
        //精度还可以扩充，继续去处理小数位
        if (precision < 16) {
            //取出double的小数位
            double decimal = a - floor(a);
            //把小数转换为最大可处理的整形以便处理
            tmp = decimal * pow(10, 16-precision);
            //把整形的小位的0去除
            while (tmp)//precision不大于16
            {
                if (0 != tmp % 10) {
                    break;
                }
                tmp /= 10;
            }
            //把整形剩余位取出，作为精度
            while (tmp / 10 != 0 && precision < 16)//precision不大于16
            {
                tmp /= 10;
                ++precision;
            }
        }
    }else if (a < 1e-6) {
        //当数据非常小时
        //先把数据上调
        double ta = a;
        while (ta < 1)
        {
            ta *= 10;
        }
        precision = 16;
        int tmp = ta * 1e16;
        //把整形的小位的0去除
        while (tmp && precision > 6)//precision不小于6
        {
            if (0 == tmp % 10) {
                --precision;
            }
            tmp /= 10;
        }
    }
    return (QString::number(a, f, precision));
}
