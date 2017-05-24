#include "SAVectorDouble.h"
#include "SADataHeader.h"
#include <memory>
#include "SAValueManager.h"

///
/// \brief 转换为double vector
/// \param data 输入的数据
/// \return 如果转换失败，返回nullptr，如果成功，返回一个指针，此指针需要无拥有者，需要交给saValueManager或者其他进行管理
///
//SAVectorDouble *SAVectorDouble::createFrom(const SAAbstractDatas *data)
//{
//    if(data->getDim() != 1)
//    {
//        return nullptr;
//    }
//    SAVectorDouble* res = SAValueManager::makeData_s<SAVectorDouble>();
//    if(data->getType() == SA::VectorDouble)
//    {
//        *res = *(static_cast<const SAVectorDouble*>(data));
//        return res;
//    }
//    else
//    {
//        const int size = data->getSize(0);
//        res->reserve(size);
//        double d;
//        bool isOK = false;
//        QVariant var;
//        for(int i=0;i<size;++i)
//        {
//            var = data->getAt(i,0);
//            d = var.toDouble(&isOK);
//            if(isOK)
//            {
//                res->append(d);
//            }
//        }
//        if(res->getSize(0) > 0)
//        {
//            return res;
//        }
//    }
//    if(nullptr != res)
//    {
//        SAValueManager::deleteData_s(res);
//    }
//    return nullptr;
//}

void SAVectorDouble::write(QDataStream &out) const
{
    SADataHeader type(this);
    out << type;
    SAAbstractDatas::write(out);
    out << getValueDatas();
}
///
/// \brief 转换为double数组
/// \param ptr SAAbstractDatas指针
/// \param res 结果
/// \return 可以转换返回true
///
bool SAVectorDouble::toDoubleVector(const SAAbstractDatas *ptr, QVector<double> &data)
{
    if(ptr->getDim() != SA::Dim1)
    {
        return false;
    }
    const SAVectorDouble* doubleVector = dynamic_cast <const SAVectorDouble*>(ptr);
    if(doubleVector)
    {
        doubleVector->getValueDatas(data);
        return true;
    }
    const SAVectorInt* intVector = dynamic_cast <const SAVectorInt*>(ptr);
    if(intVector)
    {
        //处理int数组情况
        const int size = intVector->getSize();
        data.resize(size);
        for(int i=0;i<size;++i)
        {
            data[i] = intVector->get(i);
        }
        return true;
    }

    //处理其它情况
    bool isSuccess = false;
    const int size = ptr->getSize(SA::Dim1);
    QVariant var;
    for(int i=0;i<size;++i)
    {
        var = ptr->getAt(i,0);
        if(var.canConvert<double>())
        {
            data.append(var.toDouble());
            isSuccess = true;
        }
        else
        {
            //只要有一个元素无法转换返回false
            return false;
        }
    }
    return isSuccess;
}
