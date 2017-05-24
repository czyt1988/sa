#include "SAAbstractDatas.h"
#include "SADataReference.h"
#include "SAVectorDouble.h"
#include "SAVectorInt.h"
#include "SAVectorPointF.h"
#include "SAVariantDatas.h"
SAAbstractDatas::SAAbstractDatas():SAItem()
{

}

SAAbstractDatas::SAAbstractDatas(const QString &text):SAItem(text)
{

}

SAAbstractDatas::~SAAbstractDatas()
{
}

QVariant SAAbstractDatas::getAt() const
{
    return getAt({});
}

QVariant SAAbstractDatas::getAt(size_t dim1) const
{
    return getAt({dim1});
}

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
/// \brief 根据类型判断是否是数据，数据是包含数据的类型，有些如tdms的组，这些不属于数据
/// \param type 数据类型
/// \return
///
bool SAAbstractDatas::isEmpty() const
{
    int dim = getDim();
    if(0 == dim)
    {
        return false;
    }
    for(int i=1;i<dim;++i)
    {
        if(getSize(i) > 0)
        {
            return true;
        }
    }
    return false;
}




///
/// \brief 是否是向量，也就是是否是1维数据
/// \param data 数据指针
/// \return
///
bool SAAbstractDatas::isVector(SAAbstractDatas *data)
{
    const int r = data->getSize(SA::Row);
    const int c = data->getSize(SA::Column);
    if((r > 0 && 0 == c)
            ||
        (0 == r && c > 0)
      )
    {
        return true;
    }
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
/// \brief 从数组获取double vector
/// \param data 获取的double vector
/// \return 成功获取返回true
/// \see SAVectorDouble::toDoubleVector
///
bool SAAbstractDatas::converToDoubleVector(QVector<double> &data) const
{
    return SAAbstractDatas::converToDoubleVector(this,data);
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
/// \brief 转换为QPointF数组
/// \param data 结果
/// \return 可以转换返回true
/// \see SAVectorPointF::toPointFVector
///
bool SAAbstractDatas::converToPointFVector(QVector<QPointF> &data) const
{
    return SAAbstractDatas::converToPointFVector(this,data);
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
///
/// \brief 转换为double如果不行返回false
/// \param val
/// \return 可以转换返回true
/// \see SAVariantDatas::toData
///
bool SAAbstractDatas::converToDouble(double &val) const
{
    return SAAbstractDatas::converToDouble(this,val);
}


