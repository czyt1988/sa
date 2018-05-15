#ifndef SADATACONVER_H
#define SADATACONVER_H
#include "SALibGlobal.h"
#include <QVector>
#include <QPointF>
#include "SAAbstractDatas.h"
class SAVectorInterval;
///
/// \brief  负责处理
///
class SALIB_EXPORT SADataConver
{
public:
    SADataConver();
    //从数组获取double vector
    static bool converToDoubleVector(const SAAbstractDatas* input,QVector<double>& data);
    //转换为QPointF数组
    static bool converToPointFVector(const SAAbstractDatas* input,QVector<QPointF>& data);
    //转换为double如果不行返回false
    static bool converToDouble(const SAAbstractDatas* input,double& val);
    //把三个线性序列绑定为VectorInterval
    static SAVectorInterval* bindToVectorInterval(const SAAbstractDatas* x,const SAAbstractDatas *yMin,const SAAbstractDatas *yMax);
};

#endif // SADATACONVER_H
