#include "SADataConver.h"
#include "SAVectorDouble.h"
#include "SAVectorInt.h"
#include "SAVectorPointF.h"
#include "SAVariantDatas.h"
#include "SAVectorInterval.h"
#include <QScopedPointer>
SADataConver::SADataConver()
{

}

///
/// \brief 从数组获取double vector
/// \param input 输入的数据
/// \param data 获取的double vector
/// \return 成功获取返回true
/// \see SAVectorDouble::toDoubleVector
///
bool SADataConver::converToDoubleVector(const SAAbstractDatas *input, QVector<double> &data)
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
bool SADataConver::converToPointFVector(const SAAbstractDatas *input, QVector<QPointF> &data)
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
bool SADataConver::converToDouble(const SAAbstractDatas *input, double &val)
{
    return SAVariantDatas::toData<double>(input,val);
}
///
/// \brief 把三个线性序列绑定为VectorInterval
/// \param x x值
/// \param yMin y最小值
/// \param yMax y最大值
/// \return
///
SAVectorInterval *SADataConver::bindToVectorInterval(const SAAbstractDatas *x, const SAAbstractDatas *yMin, const SAAbstractDatas *yMax)
{
    QScopedPointer<SAVectorInterval> ptr;
    if(SA::VectorDouble == x->getType()
        && SA::VectorDouble == yMin->getType()
        && SA::VectorDouble == yMax->getType()
            )
    {
        QVector<QwtIntervalSample> datas;
        SAVectorDouble* sax = static_cast<SAVectorDouble*>(x);
        SAVectorDouble* sayMin = static_cast<SAVectorDouble*>(yMin);
        SAVectorDouble* sayMax = static_cast<SAVectorDouble*>(yMax);
        int minSize = qMin(sax->getValueDatas().size(),sayMin->getValueDatas().size());
        minSize = qMin(minSize,sayMax->getValueDatas().size());
        datas.resize(minSize);
        for(int i=0;i<minSize;++i)
        {
            datas[i] = QwtIntervalSample(sax->getValueDatas().at(i)
                                         ,sayMin->getValueDatas().at(i)
                                         ,sayMax->getValueDatas().at(i)
                                         );
        }
        if(datas.size() > 0)
        {
            ptr.reset(new SAVectorInterval(datas));
        }
    }
    else
    {
        if(SA::Dim1 == x->getDim()
            && SA::Dim1 == yMin->getDim()
            && SA::Dim1 == yMax->getDim()
                )
        {
            QVector<QwtIntervalSample> datas;
            int minSize = qMin(x->getSize(),yMin->getSize());
            minSize = qMin(minSize,yMax->getSize());
            datas.reserve(minSize);
            double dx,dyMin,dyMax;
            for(int i=0;i<minSize;++i)
            {
                bool isOK = false;
                QVariant var = x->getAt(i);
                dx = var.toDouble(&isOK);
                if(!isOK)
                {
                    continue;
                }
                var = yMin->getAt(i);
                dyMin = var.toDouble(&isOK);
                if(!isOK)
                {
                    continue;
                }
                var = yMax->getAt(i);
                dyMax = var.toDouble(&isOK);
                if(!isOK)
                {
                    continue;
                }
                datas.push_back(QwtIntervalSample(dx,dyMin,dyMax));
            }
            if(datas.size() > 0)
            {
                ptr.reset(new SAVectorInterval(datas));
            }
        }
    }
    return ptr.take();
}

