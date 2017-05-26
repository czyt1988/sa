#include "SATableDouble.h"


SATableDouble::SATableDouble():SATableData<double>()
{
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATableDouble::SATableDouble(const QString &name):SATableData<double>(name)
{
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATableDouble::~SATableDouble()
{

}

int SATableDouble::getType() const
{
    return SA::TableDouble;
}

QString SATableDouble::getTypeName() const
{
    return QString("double table");
}
