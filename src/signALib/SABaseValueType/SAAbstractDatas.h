#ifndef SAABSTRACTDATAS_H
#define SAABSTRACTDATAS_H

#include "SALibGlobal.h"
#include "SAItem.h"
#include <QList>
#include <QDataStream>
#include <initializer_list>
class SADataReference;
///
/// \brief sa的变量接口
///
/// 关于序列化问题：
///     由于SAItem的ID是在其生成时自动用其指针作为id，序列化时会把这个id保存起来，但
/// 读取时，旧的id会保存在一个临时表里，主要是SAData里有个特殊的类型：SADataReference，这个是一个引用类型
/// ，此类型保存了引用的指针，在序列化时会只保留其关联对象的id，读取数据时，最后处理SADataReference的内容，此时可以查找旧的ID表
/// ，查找对应的指针，再调用createReference进行引用的创建
/// \author czy -> czy.t@163.com
/// \date
///
class SALIB_EXPORT SAAbstractDatas : public SAItem
{
public:
    SAAbstractDatas();
    SAAbstractDatas(const QString & text);
    virtual ~SAAbstractDatas();
    virtual int getType() const = 0;
    //获取尺寸，dim是维度，对应1为行，2为列，3就是第三维
    virtual int getSize(int dim=SA::Dim1) const = 0;
    //获取维度 0代表点数，1代表向量，2代表表……
    virtual int getDim() const = 0;

    virtual QVariant getAt(const std::initializer_list<size_t>& index) const = 0;
    QVariant getAt() const;
    QVariant getAt(size_t dim1) const;
    QVariant getAt(size_t dim1,size_t dim2) const;

    virtual QString displayAt(const std::initializer_list<size_t>& index) const = 0;
    QString displayAt() const;
    QString displayAt(size_t dim1) const;
    QString displayAt(size_t dim1,size_t dim2) const;

    virtual bool isEmpty() const;
    //是否是向量
    static bool isVector(SAAbstractDatas* data);

    virtual void read(QDataStream & in);
    virtual void write(QDataStream & out) const;

    static bool converToDoubleVector(const SAAbstractDatas* input,QVector<double>& data);
    bool converToDoubleVector(QVector<double>& data) const;
    static bool converToPointFVector(const SAAbstractDatas* input,QVector<QPointF>& data);
    bool converToPointFVector(QVector<QPointF>& data) const;
    static bool converToDouble(const SAAbstractDatas* input,double& val);
    bool converToDouble(double& val) const;
};

#endif // SAABSTRACTDATAS

