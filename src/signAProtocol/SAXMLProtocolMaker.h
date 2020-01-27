#ifndef SAXMLPROTOCOLMAKER_H
#define SAXMLPROTOCOLMAKER_H
#include "SAProtocolGlobal.h"
#include "SAAbstractProtocolMaker.h"
#include <QVariant>


class SAXMLProtocolMakerPrivate;

#ifndef SA_XML_FUNTION_2D_POINTS_DESCRIBE
#define SA_XML_FUNTION_2D_POINTS_DESCRIBE "2d-points-describe"
#endif
#ifndef SA_XML_FUNTION_2D_POINTS_DESCRIBE_RES
#define SA_XML_FUNTION_2D_POINTS_DESCRIBE_RES "2d-points-describe-res"
#endif
/**
 * @brief 组成sa xml协议的实现类
 * 形如下面的xml各式：
 *
 * @code
 * <sa>
 *     <values>
 *         <group name="">
 *             <item type='int'>5<item>
 *         </group>
 *     </values>
 * </sa>
 * @endcode
 *
 * item 的type支持如下内容
 * bool
 * int
 * uint
 * qlonglong
 * qulonglong
 * double
 * QChar
 * QVariantMap
 * QVariantList
 * QString
 * QStringList
 * QByteArray
 * QBitArray
 * QDate
 * QTime
 * QDateTime
 * QUrl
 * QLocale
 * QRect
 * QRectF
 * QSize
 * QSizeF
 * QLine
 * QLineF
 * QPoint
 * QPointF
 * QRegExp
 * QRegularExpression
 * QVariantHash
 * QEasingCurve
 * QUuid
 * QModelIndex
 * QPersistentModelIndex
 * std::nullptr_t
 * QFont
 * QPixmap
 * QBrush
 * QColor
 * QPalette
 * QImage
 * QPolygon
 * QRegion
 * QBitmap
 * QCursor
 * QKeySequence
 * QPen
 * QTextLength
 * QTextFormat
 * QMatrix
 * QTransform
 * QMatrix4x4
 * QVector2D
 * QVector3D
 * QVector4D
 * QQuaternion
 * QPolygonF
 * QIcon
 * QPolygonF
 * QSizePolicy
 * QWidget*
 *
 */
class SAXMLProtocolMaker : public SAAbstractProtocolMaker
{
    SA_IMPL(SAXMLProtocolMaker)
public:
    SAXMLProtocolMaker(const QString& roottype = "",QObject* par = nullptr);
    ~SAXMLProtocolMaker();
    virtual void setValue(const QString& groupName, const QString& keyName, const QVariant& var);
    // 复杂度O(1)
    virtual QStringList getGroupNames() const;
    // 复杂度O(n)
    virtual QStringList getKeyNames(const QString& groupName) const;
    virtual QString toString() const;
protected:

};

#endif // SAXMLPROTOCOLMAKER_H
