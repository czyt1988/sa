#ifndef SATABLEDOUBLE_H
#define SATABLEDOUBLE_H

#include "SATableData.h"

class SALIB_EXPORT SATableDouble : public SATableData<double>
{
public:
    typedef czy::QtApp::QczyHashTable<double> Table;
public:
    SATableDouble()
    {
        setData (getType (),SA_ROLE_DATA_TYPE);
    }
    SATableDouble(const QString & name):SATableData<double>(name){setData (getType (),SA_ROLE_DATA_TYPE);}
    //TODO : override toDoubleVector toPointFVector isCanCast2DoubleVector isCanCast2PointFVector
    virtual ~SATableDouble(){}
    virtual int getType() const   {return SA::TableDouble;}
    virtual QString getTypeName() const{return QString("double table");}
    SA_TABLE_WRITE(SATableDouble)
};

#endif // SATABLEDOUBLE_H
