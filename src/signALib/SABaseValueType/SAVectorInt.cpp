#include "SAVectorInt.h"
#include "SADataHeader.h"


//bool SAVectorInt::toDoubleVector(QVector<double>& data) const
//{
//    int size = getSize ();
//    data.resize (size);
//    for(int i=0;i<size;++i)
//    {
//        data[i] = (double)getValue (i);
//    }
//    return true;
//}



///
/// \brief 写文件时，会额外加一个SADataTypeInfo，用于判断类型，在读取时，是不会读取这个SADataTypeInfo的，因为读取时需要
/// 先读取SADataTypeInfo，根据信息再来进行其它操作
/// \param out
/// \see read
///
void SAVectorInt::write(QDataStream &out) const
{
    SADataHeader type(this);
    out << type;
    SAAbstractDatas::write(out);
    out << getValueDatas();
}

