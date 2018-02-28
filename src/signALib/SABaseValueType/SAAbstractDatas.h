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
    //获取文件类型
    virtual int getType() const = 0;
    //获取尺寸，dim是维度，对应1为行，2为列，3就是第三维
    virtual int getSize(int dim=SA::Dim1) const = 0;
    //获取维度 0代表点数，1代表向量，2代表表……
    virtual int getDim() const = 0;


    ///
    /// \brief 获取对应索引的内容
    /// \return
    ///
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const = 0;
    //相当于getAt({});适用0维单一数据
    QVariant getAt() const;
    //相当于getAt({dim1});适用1维向量数据
    QVariant getAt(size_t dim1) const;
    //相当于getAt({dim1,dim2});适用2维表数据
    QVariant getAt(size_t dim1,size_t dim2) const;


    ///
    /// \brief 用于显示对应的内容
    /// \param index
    /// \return
    ///
    virtual QString displayAt(const std::initializer_list<size_t>& index) const = 0;
    //用于显示,相当于displayAt({});适用0维单一数据
    QString displayAt() const;
    //用于显示,相当于displayAt({dim1});适用1维向量数据
    QString displayAt(size_t dim1) const;
    //用于显示,相当于displayAt({dim1,dim2});适用2维表数据
    QString displayAt(size_t dim1,size_t dim2) const;

    //用于编辑-返回true设置成功，返回false设置失败，默认SAAbstractDatas返回false不接受编辑
    virtual bool setAt(const QVariant& val,const std::initializer_list<size_t>& index);

    //根据类型判断是否是数据,如nan就返回true，如空的一维数据都返回true
    virtual bool isEmpty() const = 0;
    //从文件读取
    virtual void read(QDataStream & in);
    //写入文件
    virtual void write(QDataStream & out) const;

    //判断该数据在上次write之后是否内存有变更
    virtual bool isDirty() const = 0;
    //设置内存有变更
    virtual void setDirty(bool dirty) = 0;
public:
    static bool converToDoubleVector(const SAAbstractDatas* input,QVector<double>& data);
    static bool converToPointFVector(const SAAbstractDatas* input,QVector<QPointF>& data);
    static bool converToDouble(const SAAbstractDatas* input,double& val);
};

#endif // SAABSTRACTDATAS

