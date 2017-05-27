#ifndef SAVECTORINT_H
#define SAVECTORINT_H

#include "SAVectorDatas.h"


class SALIB_EXPORT SAVectorInt : public SAVectorDatas<int>
{
public:
    SAVectorInt(){setData (getType(),SA_ROLE_DATA_TYPE);}
    SAVectorInt(const QString & name):SAVectorDatas<int>(name){setData (getType(),SA_ROLE_DATA_TYPE);}
    SAVectorInt(const QString& name,const QVector<int>& datas)
        :SAVectorDatas<int>(name,datas){setData (getType(),SA_ROLE_DATA_TYPE);}
    virtual ~SAVectorInt(){}
    virtual int getType() const   {return SA::VectorInt;}
    virtual QString getTypeName() const{return QString("int Array");}
    virtual void write(QDataStream & out) const;
};

#endif // SAVECTORINT_H
