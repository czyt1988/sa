#include "sa_fun_core.h"

#include <QVector>
#include <iterator>
#include "SAValueManager.h"
#include "SAAbstractDatas.h"
#include "SAVectorDouble.h"
#include "SAVariantDatas.h"
#include "SADataConver.h"
#include <QCoreApplication>
///
/// \brief 激记录错误信息
///
static QString s_error_info = QString("");

#define TR(str)\
    QCoreApplication::translate("sa_fun_core", str, 0)

///
/// \brief 获取数组，如果没能获取，会写入错误信息
/// \param input 输入数据
/// \param vd 获取结果
/// \return 成功返回true，否则可以通过getLastErrorString获取错误信息
///
bool saFun::getDoubleVector(const SAAbstractDatas* input,QVector<double>& vd)
{
    if(!SADataConver::converToDoubleVector(input,vd))
    {
        setErrorString(TR("can not conver %1 to vector").arg(input->getName()));
        return false;
    }
    return true;
}
///
/// \brief 获取点数组，如果没能获取，会写入错误信息
/// \param input 输入数据
/// \param vd 获取结果
/// \return 成功返回true，否则可以通过getLastErrorString获取错误信息
///
bool saFun::getPointFVector(const SAAbstractDatas *input, QVector<QPointF> &vd)
{
    if(!SADataConver::converToPointFVector(input,vd))
    {
        setErrorString(TR("can not conver %1 to points vector").arg(input->getName()));
        return false;
    }
    return true;
}
//获取错误信息
QString saFun::getLastErrorString()
{
    return s_error_info;
}




void saFun::setErrorString(const QString &str)
{
    s_error_info = str;
}


///
/// \brief 获取点集的y值
/// \param input 输入的参数
/// \param vd 向量
/// \return 成功获取返回true
///
bool saFun::getPointFVectorYData(const SAAbstractDatas *input, QVector<double> &vd)
{
    if(input->getType() == SA::VectorPoint)
    {
        vd.resize(input->getSize());
        SAVectorPointF::getYs(static_cast<const SAVectorPointF*>(input),vd.begin());
        return true;
    }
    QVector<QPointF> ps;
    if(saFun::getPointFVector(input,ps))
    {
        vd.reserve(ps.size());
        std::for_each(ps.begin(),ps.end(),[&vd](const QPointF& p){
            vd.append(p.y());
        });
        return true;
    }
    return false;
}
///
/// \brief 获取点集的y值
/// \param input
/// \param vd
///
void saFun::getPointFVectorYData(const QVector<QPointF> &input, QVector<double> &vd)
{
    vd.reserve(input.size());
    std::for_each(input.begin(),input.end(),[&vd](const QPointF& p){
        vd.append(p.y());
    });
}

///
/// \brief 获取点集的x值
/// \param input 输入的参数
/// \param vd 向量
/// \return 成功获取返回true
///
bool saFun::getPointFVectorXData(const SAAbstractDatas *input, QVector<double> &vd)
{
    if(input->getType() == SA::VectorPoint)
    {
        vd.resize(input->getSize());
        SAVectorPointF::getXs(static_cast<const SAVectorPointF*>(input),vd.begin());
        return true;
    }
    QVector<QPointF> ps;
    if(saFun::getPointFVector(input,ps))
    {
        vd.reserve(ps.size());
        std::for_each(ps.begin(),ps.end(),[&vd](const QPointF& p){
            vd.append(p.x());
        });
        return true;
    }
    return false;
}
///
/// \brief 获取点集的x值
/// \param input 输入的参数
/// \param vd 向量
///
void saFun::getPointFVectorXData(const QVector<QPointF> &input, QVector<double> &vd)
{
    vd.reserve(input.size());
    std::for_each(input.begin(),input.end(),[&vd](const QPointF& p){
        vd.append(p.x());
    });
}
///
/// \brief 获取点集的x,y值
/// \param input 输入的参数
/// \param xs x值
/// \param ys y值
/// \return 成功获取返回true
///
bool saFun::getPointFVectorXYData(const SAAbstractDatas *input, QVector<double> &xs, QVector<double> &ys)
{
    if(input->getType() == SA::VectorPoint)
    {
        SAVectorPointF::getXs(static_cast<const SAVectorPointF*>(input),xs.begin());
        SAVectorPointF::getYs(static_cast<const SAVectorPointF*>(input),ys.begin());
        return true;
    }
    QVector<QPointF> ps;
    if(saFun::getPointFVector(input,ps))
    {
        xs.reserve(ps.size());
        ys.reserve(ps.size());
        std::for_each(ps.begin(),ps.end(),[&xs,&ys](const QPointF& p){
            xs.append(p.x());
            ys.append(p.y());
        });
        return true;
    }
    return false;
}
///
/// \brief 把x，y数组设置为等长，长度为最小一组的长度
/// \param xs x数组
/// \param ys y数组
///
void saFun::fixSizeXYVector(QVector<double> &xs, QVector<double> &ys)
{
    if(xs.size() != ys.size())
    {
        int minSize = std::min(xs.size(),ys.size());
        if(xs.size() != minSize)
        {
            xs.resize(minSize);
        }
        if(ys.size() != minSize)
        {
            ys.resize(minSize);
        }
    }
}

///
/// \brief saFun::makeVectorPointF
/// \param x
/// \param y
/// \param xys
///
void saFun::makeVectorPointF(const QVector<double> &x, const QVector<double> &y, QVector<QPointF> &xys)
{
    const int minSize = qMin(x.size(),y.size());
    xys.resize(minSize);
    for(int i=0;i<minSize;++i)
    {
        xys[i] = QPointF(x[i],y[i]);
    }
}




