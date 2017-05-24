#ifndef SAVECTORPOINTF_H
#define SAVECTORPOINTF_H

#include "SAVectorDatas.h"
#include <QPointF>
class SALIB_EXPORT SAVectorPointF : public SAVectorDatas<QPointF>
{
public:
    SAVectorPointF(){setData (getType (),SA_ROLE_DATA_TYPE);}
    SAVectorPointF(const QString & name):SAVectorDatas<QPointF>(name){setData (getType (),SA_ROLE_DATA_TYPE);}
    SAVectorPointF(const QString& name,const QVector<QPointF>& datas)
        :SAVectorDatas<QPointF>(name,datas)
    {
        setData (getType (),SA_ROLE_DATA_TYPE);
    }
    SAVectorPointF(const QString& name,const QVector<double>& xs,const QVector<double>& ys)
        :SAVectorDatas<QPointF>(name)
    {
        setData (getType (),SA_ROLE_DATA_TYPE);
        setXYValueDatas(xs,ys);
    }
    void setXYValueDatas(const QVector<double>& xs,const QVector<double>& ys);
    virtual ~SAVectorPointF(){}
    virtual int getType() const   {return SA::VectorPoint;}
    virtual QString getTypeName() const{return QString("point Vector");}
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const;
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
    void getYs(QVector<double>& data) const;
    void getXs(QVector<double>& data) const;

    //根据现有点序列创建一个新的点序列，并替换掉y值
    virtual SAVectorPointF* copyChangY(const QVector<double>& y) const;

    virtual void write(QDataStream & out) const;
    //转换为QPointF数组
    static bool toPointFVector(const SAAbstractDatas *ptr, QVector<QPointF> &data);
    ///
    /// \brief 获取点集的y值
    /// \param ptr 点集指针
    /// \param begin 存放y值的迭代器起始地址
    /// \param startIndex 点集提取数据的开始索引，如果为0，就是从第一个参数开始提取所有的y值
    /// \code
    /// QVector<double> ys;
    /// ys.reserve(wave->getSize());
    /// SAVectorPointF::getYs(wave,std::back_inserter(ys));
    /// \endcode
    ///
    template<typename IT>
    static void getYs(const SAVectorPointF* ptr ,IT begin,int startIndex = 0)
    {
        const QVector<QPointF>& points = ptr->getValueDatas ();
        std::transform(std::next(points.begin (),startIndex)
                       ,points.end (),begin,[](const QPointF& p)->double{
                           return p.y();
        });
    }
    ///
    /// \brief 获取点集的x值
    /// \param ptr 点集指针
    /// \param begin 存放y值的迭代器起始地址
    /// \param startIndex 点集提取数据的开始索引，如果为0，就是从第一个参数开始提取所有的y值
    /// \code
    /// QVector<double> ys;
    /// ys.reserve(wave->getSize());
    /// SAVectorPointF::getYs(wave,std::back_inserter(ys));
    /// \endcode
    ///
    template<typename IT>
    static void getXs(const SAVectorPointF* ptr ,IT begin,int startIndex = 0)
    {
        const QVector<QPointF>& points = ptr->getValueDatas ();
        std::transform(std::next(points.begin (),startIndex)
                       ,points.end (),begin,[](const QPointF& p)->double{
                           return p.x();
        });
    }

    ///
    /// \brief 替换点集的y值
    /// \param ptr 点集指针
    /// \param ys_begin 数组的开始迭代器
    /// \param ys_end 数组的结束迭代器
    /// \param startIndex 替换点集的开始地址
    /// \note 需要保证替换的长度不能超过点集的长度
    ///
    template<typename IT>
    static void replaceYs(SAVectorPointF* ptr ,IT ys_begin,IT ys_end,int startIndex = 0)
    {
        while(ys_begin != ys_end)
        {
            ptr->get(startIndex).setY(*ys_begin);
            ++startIndex;
            ++ys_begin;
        }
    }

    ///
    /// \brief 替换点集的x值
    /// \param ptr 点集指针
    /// \param ys_begin 数组的开始迭代器
    /// \param ys_end 数组的结束迭代器
    /// \param startIndex 替换点集的开始地址
    /// \note 需要保证替换的长度不能超过点集的长度
    ///
    template<typename IT>
    static void replaceXs(SAVectorPointF* ptr ,IT xs_begin,IT xs_end,int startIndex = 0)
    {
        while(xs_begin != xs_end)
        {
            ptr->get(startIndex).setX(*xs_begin);
            ++startIndex;
            ++xs_begin;
        }
    }
};
#endif // SAVECTORPOINTF_H
