#include "SAVectorVariant.h"
#include "SADataHeader.h"


SAVectorVariant::SAVectorVariant()
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}

SAVectorVariant::SAVectorVariant(const QString &name)
    :SAVectorDatas<QVariant>(name)
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}

SAVectorVariant::SAVectorVariant(const QString &name, const QVector<QVariant> &datas)
    :SAVectorDatas<QVariant>(name,datas)
{
    setData (getType(),SA_ROLE_DATA_TYPE);
}

QString SAVectorVariant::displayAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
        return get(*index.begin()).toString();
    return QString();
}


void SAVectorVariant::setVariantList(const QVariantList& var)
{
    int size = var.size ();
    for(int i=0;i<size;++i)
    {
        append(var[i]);
    }
}

void SAVectorVariant::write(QDataStream &out) const
{
    SADataHeader type(this);
    out << type;
    SAAbstractDatas::write(out);
    out << getValueDatas();
}



