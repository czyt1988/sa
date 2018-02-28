#include "SAAbstractDatas.h"
#include "SADataReference.h"
#include "SAVectorDouble.h"
#include "SAVectorInt.h"
#include "SAVectorPointF.h"
#include "SAVariantDatas.h"
#include <QDebug>

SAAbstractDatas::SAAbstractDatas():SAItem()
{

}

SAAbstractDatas::SAAbstractDatas(const QString &text):SAItem(text)
{

}

SAAbstractDatas::~SAAbstractDatas()
{
    qDebug() << "destroy:" << getName();
}

///
/// \brief 相当于getAt({});适用0维单一数据
/// \return
///
QVariant SAAbstractDatas::getAt() const
{
    return getAt({});
}

///
/// \brief 相当于getAt({dim1});适用1维向量数据
/// \param dim1
/// \return
///
QVariant SAAbstractDatas::getAt(size_t dim1) const
{
    return getAt({dim1});
}

///
/// \brief 相当于getAt({dim1,dim2});适用2维表数据
/// \param dim1
/// \param dim2
/// \return
///
QVariant SAAbstractDatas::getAt(size_t dim1, size_t dim2) const
{
    return getAt({dim1,dim2});
}


QString SAAbstractDatas::displayAt() const
{
    return displayAt({});
}

QString SAAbstractDatas::displayAt(size_t dim1) const
{
    return displayAt({dim1});
}

QString SAAbstractDatas::displayAt(size_t dim1, size_t dim2) const
{
    return displayAt({dim1,dim2});
}
///
/// \brief 用于编辑，默认SAAbstractDatas返回false不接受编辑
/// \param val 需要设置的值
/// \param index 索引序列
/// \return 返回true设置成功，返回false设置失败
///
bool SAAbstractDatas::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    return false;
}



void SAAbstractDatas::read(QDataStream &in)
{
    QStandardItem::read(in);
    setID(int(this));//read后会把id复写为旧的id，这里重新设置id
}

void SAAbstractDatas::write(QDataStream &out) const
{
    QStandardItem::write(out);
}
///
/// \brief 从数组获取double vector
/// \param input 输入的数据
/// \param data 获取的double vector
/// \return 成功获取返回true
/// \see SAVectorDouble::toDoubleVector
///
bool SAAbstractDatas::converToDoubleVector(const SAAbstractDatas *input, QVector<double> &data)
{
    return SAVectorDouble::toDoubleVector(input,data);
}

///
/// \brief 转换为QPointF数组
/// \param ptr SAAbstractDatas指针
/// \param res 结果
/// \return 可以转换返回true
/// \see SAVectorPointF::toPointFVector
///
bool SAAbstractDatas::converToPointFVector(const SAAbstractDatas *input, QVector<QPointF> &data)
{
    return SAVectorPointF::toPointFVector(input,data);
}

///
/// \brief 转换为double如果不行返回false
/// \param input
/// \param val
/// \return 可以转换返回true
/// \see SAVariantDatas::toData
///
bool SAAbstractDatas::converToDouble(const SAAbstractDatas *input, double &val)
{
    return SAVariantDatas::toData<double>(input,val);
}



