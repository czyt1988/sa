#ifndef SAVECTORDOUBLE_H
#define SAVECTORDOUBLE_H

#include "SAVectorDatas.h"

class SALIB_EXPORT SAVectorDouble : public SAVectorDatas<double>
{
public:
    SAVectorDouble()
        :SAVectorDatas<double>(){setData (SA::VectorDouble,SA_ROLE_DATA_TYPE);}
    SAVectorDouble(const QString & name)
        :SAVectorDatas<double>(name){setData (SA::VectorDouble,SA_ROLE_DATA_TYPE);}
    SAVectorDouble(const QString& name,const QVector<double>& datas)
        :SAVectorDatas<double>(name,datas){setData (SA::VectorDouble,SA_ROLE_DATA_TYPE);}
    SAVectorDouble(const QVector<double>& datas)
        :SAVectorDatas<double>(datas){setData (SA::VectorDouble,SA_ROLE_DATA_TYPE);}
    virtual ~SAVectorDouble(){}
    virtual int getType() const   {return SA::VectorDouble;}
    virtual QString getTypeName() const{return QString("double Vector");}
    virtual void write(QDataStream & out) const;
    //转换为double数组
    static bool toDoubleVector(const SAAbstractDatas* ptr,QVector<double>& data);
};

#endif // SAVECTORDOUBLE_H
