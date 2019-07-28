#include "SATableDouble.h"


SATableDouble::SATableDouble():SATableData<double>()
{
    setProperty (getType (),SA_ROLE_DATA_TYPE);
}

SATableDouble::SATableDouble(const QString &name):SATableData<double>(name)
{
    setProperty (getType (),SA_ROLE_DATA_TYPE);
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
