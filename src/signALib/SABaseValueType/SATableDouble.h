#ifndef SATABLEDOUBLE_H
#define SATABLEDOUBLE_H

#include "SATableData.h"

class SALIB_EXPORT SATableDouble : public SATableData<double>
{
public:
    SATableDouble();
    SATableDouble(const QString & name);
    //TODO : override toDoubleVector toPointFVector isCanCast2DoubleVector isCanCast2PointFVector
    virtual ~SATableDouble();
    virtual int getType() const;
    virtual QString getTypeName() const;
    SA_TABLE_WRITE(SATableDouble)
};

#endif // SATABLEDOUBLE_H

