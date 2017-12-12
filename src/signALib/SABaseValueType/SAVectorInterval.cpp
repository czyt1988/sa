#include "SAVectorInterval.h"
#include "SADataHeader.h"

SAVectorInterval::SAVectorInterval()
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}

SAVectorInterval::SAVectorInterval(const QString &name):SAVectorDatas<QwtIntervalSample>(name)
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}

SAVectorInterval::SAVectorInterval(const QString &name, const QVector<QwtIntervalSample> &datas)
    :SAVectorDatas<QwtIntervalSample>(name,datas)
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}



QVariant SAVectorInterval::getAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
    {
        const QwtIntervalSample& sam = get(*index.begin());
        return QVariant::fromValue<QwtIntervalSample>(sam);
    }
    return QVariant();
}

QString SAVectorInterval::displayAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
    {
        const QwtIntervalSample& sam = get(*index.begin());
        return QString("%1(%2,%3)").arg(sam.value).arg(sam.interval.minValue ()).arg(sam.interval.maxValue ());
    }
    return QString();
}


void SAVectorInterval::write(QDataStream &out) const
{
    SADataHeader type(this);
    out << type;
    SAAbstractDatas::write(out);
    out << getValueDatas();
}


///
/// \brief QwtIntervalSample序列化支持
/// \param out
/// \param item
/// \return
///
QDataStream &operator<<(QDataStream & out, const QwtIntervalSample & item)
{
    out << item.value << item.interval;
    return out;
}
///
/// \brief QwtIntervalSample序列化支持
/// \param out
/// \param item
/// \return
///
QDataStream &operator>>(QDataStream & in, QwtIntervalSample & item)
{
    in >> item.value >> item.interval;
    return in;
}
///
/// \brief QwtInterval序列化支持
/// \param out
/// \param item
/// \return
///
QDataStream &operator<<(QDataStream & out, const QwtInterval & item)
{
    out << item.minValue() << item.maxValue() << item.borderFlags();
    return out;
}
///
/// \brief QwtInterval序列化支持
/// \param out
/// \param item
/// \return
///
QDataStream &operator>>(QDataStream & in, QwtInterval & item)
{
    int flag;
    double min,max;
    in >> min >> max >> flag;
    item.setMinValue(min);
    item.setMaxValue(max);
    item.setBorderFlags(static_cast<QwtInterval::BorderFlags>(flag));
    return in;
}
