#include "SAVectorOHLCDatas.h"
#include "SADataHeader.h"
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
    else if(2 == index.size())
    {
        const QwtOHLCSample& sam = get(*index.begin());
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
    else if(2 == index.size())
    {
        const QwtOHLCSample& sam = get(*index.begin());
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
