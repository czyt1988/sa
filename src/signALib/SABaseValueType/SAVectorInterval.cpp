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

int SAVectorInterval::getDim() const
{
    return SA::Dim2;
}

int SAVectorInterval::getSize(int dim) const
{
    if(dim==SA::Dim1)
    {
        return this->SAVectorDatas<QwtIntervalSample>::getSize(dim);
    }
    else if(SA::Dim2 == dim)
    {
        return 3;
    }
    return 0;
}


///
/// \brief 2维数据 dim1为行。dim2第一列为value第二列为minValue，第三列为maxValue
/// \param index
/// \return
///
QVariant SAVectorInterval::getAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
    {
        const QwtIntervalSample& sam = get(*index.begin());
        return QVariant::fromValue<double>(sam.value);
    }
    else if(index.size()>=2)
    {
        for(auto i=(index.begin()+2);i!=index.end();++i)
        {
            if(0!=(*i))
            {
                return QVariant();
            }
        }
        int r = (*index.begin());
        if(r >= getValueDatas().size() || r<0)
        {
            return QVariant();
        }
        const QwtIntervalSample& sam = get(r);
        int c = *(index.begin()+1);
        if(0 == c)
        {
            return sam.value;
        }
        else if(1 == c)
        {
            return sam.interval.minValue();
        }
        else if(2 == c)
        {
            return sam.interval.maxValue();
        }
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
    else if(index.size()>=2)
    {
        for(auto i=(index.begin()+2);i!=index.end();++i)
        {
            if(0!=(*i))
            {
                return QString();
            }
        }
        int r = (*index.begin());
        if(r >= getValueDatas().size() || r<0)
        {
            return QString();
        }
        const QwtIntervalSample& sam = get(r);
        int c = *(index.begin()+1);
        if(0 == c)
        {
            return QString::number(sam.value);
        }
        else if(1 == c)
        {
            return QString::number(sam.interval.minValue());
        }
        else if(2 == c)
        {
            return QString::number(sam.interval.maxValue());
        }
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

bool SAVectorInterval::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    if(1 == index.size())
    {
        return SAVectorDatas<QwtIntervalSample>::setAt(val,index);
    }
    else if(index.size()>=2)
    {
        for(auto i=(index.begin()+2);i!=index.end();++i)
        {
            if(0!=(*i))
            {
                return false;
            }
        }
        int r = *(index.begin());
        if(r < 0 || r >  getValueDatas().size())
        {
            return false;
        }
        QwtIntervalSample& sam = get(r);
        int c = *(index.begin()+1);
        bool isOK = false;
        double d = val.toDouble(&isOK);
        if(!isOK)
            return false;
        if(0 == c)
        {
            sam.value = d;
        }
        else if(1 == c)
        {
            sam.interval.setMinValue(d);
        }
        else if(2 == c)
        {
            sam.interval.setMaxValue(d);
        }
        else
        {
            return false;
        }
        setDirty(true);
        return true;
    }
    return false;
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

QDebug operator<<(QDebug debug, const QwtIntervalSample &c)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << c.value << ",[" << c.interval.minValue() << '~' << c.interval.maxValue() << "])";
    return debug.space();
}
