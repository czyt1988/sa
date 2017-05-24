#ifndef SAVECTORVARIANT_H
#define SAVECTORVARIANT_H

#include "SAVectorDatas.h"

class SALIB_EXPORT SAVectorVariant : public SAVectorDatas<QVariant>
{
public:
    SAVectorVariant();
    SAVectorVariant(const QString & name);
    SAVectorVariant(const QString& name,const QVector<QVariant>& datas);

    virtual ~SAVectorVariant(){}
    virtual int getType() const   {return SA::VectorVariant;}
    virtual QString getTypeName() const{return QString("variant Vector");}
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
    void setVariantList(const QVariantList& var);
    virtual void write(QDataStream & out) const;

};

#endif // SAVECTORVARIANT_H
