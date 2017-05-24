#ifndef SATABLEVARIANT_H
#define SATABLEVARIANT_H

#include "SATableData.h"
#include <QVariant>
class SATableDouble;
class SAVectorDouble;
class SALIB_EXPORT SATableVariant : public SATableData<QVariant>
{
public:
    typedef czy::QtApp::QczyHashTable<QVariant> Table;
public:
    SATableVariant();
    SATableVariant(const QString & name);
    virtual ~SATableVariant(){}
    virtual int getType() const   {return SA::TableVariant;}
    virtual QString getTypeName() const{return QString("variant table");}
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const;
    //把variant table转换为double table
    bool toDouble(SATableDouble* doubleTable);
    //获取一列数据
    bool getColumnDatas(int col, SAVectorDouble* res);
    //获取一行数据
    bool getRowDatas(int row, SAVectorDouble* res);
    //
    SA_TABLE_WRITE(SATableVariant)
};
#endif // SATABLEVARIANT_H
