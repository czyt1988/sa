#ifndef SADATAPACKAGE_H
#define SADATAPACKAGE_H
#include "SALibGlobal.h"
#include "SAItem.h"
#include "SAData.h"

///
/// \brief 数据包的接口
///
class SALIB_EXPORT SAABstractDataPackage : public SAAbstractDatas
{
public:
    SAABstractDataPackage();
    SAABstractDataPackage(const QString & text);

    //插入数据
    void appendData(SAAbstractDatas* datas);
    //查询是否存在名字的子数据
    bool isfield(const QString& fieldName);
};
///
/// \brief 普通数据包
///
class SALIB_EXPORT SADataPackage : public SAABstractDataPackage
{
public:
    SADataPackage();
    SADataPackage(const QString & text);
    virtual int getType() const{return SA::NormalDataPackage;}
    virtual QString getTypeName() const{return "Data Package";}
    virtual int getSize(int sizeType=SA::Row) const;
    virtual QVariant getAt(size_t index,size_t index2=0) const;
    virtual bool toDoubleVector(QVector<double>& data,const QVector<int>* index=nullptr) const;
    virtual bool isCanCast2DoubleVector() const;
    virtual bool toPointFVector(QVector<QPointF>& data,const QVector<int>* index=nullptr) const;
    virtual bool isCanCast2PointFVector() const;

};


#endif // SADATAPACKAGE_H
