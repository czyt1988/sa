#include "SADataHeader.h"
#include "SAVectorDatas.h"
#include "SAVectorInt.h"
#include "SAVectorDouble.h"
#include "SAVectorInterval.h"
#include "SAVectorPointF.h"
#include "SAVectorVariant.h"
#include "SAVectorOHLCDatas.h"

SAVectorOHLCDatas::SAVectorOHLCDatas():SAVectorDatas<QwtOHLCSample>()
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}

SAVectorOHLCDatas::SAVectorOHLCDatas(const QString &name):SAVectorDatas<QwtOHLCSample>(name)
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}

SAVectorOHLCDatas::SAVectorOHLCDatas(const QString &name, const QVector<QwtOHLCSample> &datas)
:SAVectorDatas<QwtOHLCSample>(name,datas)
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}

SAVectorOHLCDatas::~SAVectorOHLCDatas()
{

}

QVariant SAVectorOHLCDatas::getAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
    {
        const QwtOHLCSample& sam = get(*index.begin());
        return QVariant::fromValue<QwtOHLCSample>(sam);
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
        if(r >= getValueDatas().size() || r <0)
        {
            return QVariant();
        }
        const QwtOHLCSample& sam = get(r);
        switch(*(index.begin()+1))
        {
        case 0:return sam.time;
        case 1:return sam.open;
        case 2:return sam.high;
        case 3:return sam.low;
        case 4:return sam.close;
        default:return QVariant();
        }
    }
    return QVariant();
}

QString SAVectorOHLCDatas::displayAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
    {
        const QwtOHLCSample& sam = get(*index.begin());
        return QString("%1(O:%2,H:%3,L:%4,C:%5)")
                .arg(sam.time)
                .arg(sam.open).arg(sam.high).arg(sam.low).arg(sam.close)
                ;
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
        if(r >= getValueDatas().size() || r <0)
        {
            return QString();
        }
        const QwtOHLCSample& sam = get(r);
        switch(*(index.begin()+1))
        {
        case 0:return QString::number(sam.time);
        case 1:return QString::number(sam.open);
        case 2:return QString::number(sam.high);
        case 3:return QString::number(sam.low);
        case 4:return QString::number(sam.close);
        default:return QString();
        }
    }
    return QString();
}

void SAVectorOHLCDatas::write(QDataStream &out) const
{
    SADataHeader type(this);
    out << type;
    SAAbstractDatas::write(out);
    out << getValueDatas();
}

int SAVectorOHLCDatas::getDim() const
{
    return SA::Dim2;
}

int SAVectorOHLCDatas::getSize(int dim) const
{
    if(dim==SA::Dim1)
    {
        return SAVectorDatas<QwtOHLCSample>::getSize(dim);
    }
    else if(dim==SA::Dim2)
    {
        return 5;
    }
    return 0;
}

bool SAVectorOHLCDatas::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    if(1 == index.size())
    {
        return SAVectorDatas<QwtOHLCSample>::setAt(val,index);
    }
    else if(index.size()>=2)
    {
        bool isOK = false;
        double d = val.toDouble(&isOK);
        if(!isOK)
            return false;
        int r = (*index.begin());
        if(r >= getValueDatas().size()|| r <0)
        {
            return false;
        }
        QwtOHLCSample& f = get (r);
        int c = *(index.begin()+1);
        if(c >= 2)
            return false;
        for(auto i=(index.begin()+2);i!=index.end();++i)
        {
            if(0!=(*i))
            {
                return false;
            }
        }
        switch(*(index.begin()+1))
        {
        case 0:f.time = d;break;
        case 1:f.open = d;break;
        case 2:f.high = d;break;
        case 3:f.low = d;break;
        case 4:f.close = d;break;
        default:return false;
        }
        setDirty(true);
        return true;
    }
    return false;
}

QDataStream &operator<<(QDataStream &out, const QwtOHLCSample &item)
{
    out << item.time << item.open << item.high << item.low << item.close;
    return out;
}

QDataStream &operator>>(QDataStream &in, QwtOHLCSample &item)
{
    in >> item.time >> item.open >> item.high >> item.low >> item.close;
    return in;
}

QDebug operator<<(QDebug debug, const QwtOHLCSample &c)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << 'QwtOHLCSample(t:' << c.time
                    << ",open:" << c.open
                    << ",high:"<<c.high
                    << ",low:" << c.low
                    << ",close:" << c.close
                    << ")";
    return debug;
}
