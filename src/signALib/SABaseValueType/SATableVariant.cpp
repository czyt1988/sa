#include "SATableVariant.h"
#include "SATableDouble.h"
#include "SAVectorDouble.h"
SATableVariant::SATableVariant()
{
    setData (getType (),SA_ROLE_DATA_TYPE);
}

SATableVariant::SATableVariant(const QString &name)
    :SATableData<QVariant>(name)
{
    setData (getType (),SA_ROLE_DATA_TYPE);
}

QVariant SATableVariant::getAt(const std::initializer_list<size_t> &index) const
{
    if(2 == index.size())
        return SATableData<QVariant>::getValue (*index.begin(),*(index.begin()+1));
    return QVariant();
}


bool SATableVariant::toDouble(SATableDouble *doubleTable)
{
    const int row = rowCount();
    const int col = columnCount();
    double d = 0;
    bool isOK(false);
    bool isSuccess = false;
    QVariant var;
    doubleTable->setRowCount(row);
    doubleTable->setColumnCount(col);
    for(int i= 0;i<col;++i)
    {
        for(int j = 0;j<row;++j)
        {
            var = getValue(j,i);
            if(!var.isValid())
                continue;
            d = var.toDouble(&isOK);
            if(!isOK)
                continue;
            doubleTable->setTableData (j,i,d);
            isSuccess = true;
        }
    }
    return isSuccess;
}

bool SATableVariant::getColumnDatas(int col, SAVectorDouble *res)
{
    int row = rowCount();
    if(col >= columnCount())
    {
        return false;
    }
    QVariant var;
    double d;
    bool isOK(false);
    bool isSuccess(false);
    for(int i=0;i<row;++i)
    {
        var = getValue(i,col);
        if(var.isValid())
        {
            d = var.toDouble(&isOK);
            if(isOK)
            {
                res->append(d);
                isSuccess = true;
            }
        }
    }
    return isSuccess;
}

bool SATableVariant::getRowDatas(int row, SAVectorDouble *res)
{
    int col = columnCount();
    if(row >= rowCount())
    {
        return false;
    }
    QVariant var;
    double d;
    bool isOK(false);
    bool isSuccess(false);
    for(int i=0;i<col;++i)
    {
        var = getValue(row,i);
        if(var.isValid())
        {
            d = var.toDouble(&isOK);
            if(isOK)
            {
                res->append(d);
                isSuccess = true;
            }
        }
    }
    return isSuccess;
}

