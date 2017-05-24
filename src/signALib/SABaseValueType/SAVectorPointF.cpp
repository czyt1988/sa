#include "SAVectorPointF.h"
#include "SADataHeader.h"


void SAVectorPointF::setXYValueDatas(const QVector<double> &xs, const QVector<double> &ys)
{
    QVector<QPointF> ps;
    int minSize = xs.size() < ys.size() ? xs.size() : ys.size();
    ps.reserve(minSize);
    for(int i=0;i<minSize;++i)
    {
        ps.append(QPointF(xs[i],ys[i]));
    }
    setValueDatas(ps);
}

QVariant SAVectorPointF::getAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
    {
        return QVariant::fromValue<QPointF>(get (*index.begin()));
    }
    return QVariant();
}

QString SAVectorPointF::displayAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
    {
        const QPointF& f = get (*index.begin());
        return QString("%1,%2").arg(f.x()).arg(f.y());
    }
    return QString();
}

void SAVectorPointF::getYs(QVector<double>& data) const
{
    const QVector<QPointF>& points = getValueDatas ();
    data.resize (points.size ());
    std::transform(points.begin (),points.end (),data.begin (),[](const QPointF& p)->double{
                       return p.y();
    });
}

void SAVectorPointF::getXs(QVector<double>& data) const
{
    const QVector<QPointF>& points = getValueDatas ();
    data.resize (points.size ());
    std::transform(points.begin (),points.end (),data.begin (),[](const QPointF& p)->double{
                       return p.x();
    });
}

///
/// \brief 注意，是工厂函数
/// \param y
/// \return
///
SAVectorPointF*SAVectorPointF::copyChangY(const QVector<double>& y) const
{
     if(y.size () == 0)
         return nullptr;
     SAVectorPointF* points = new SAVectorPointF();
     points->reserve(y.size ());
     for(int i=0;i<y.size ();++i)
     {
         points->append (QPointF(get (i).x (),y[i]));
     }
     return points;
}

void SAVectorPointF::write(QDataStream &out) const
{
    SADataHeader type(this);
    out << type;
    SAAbstractDatas::write(out);
    out << getValueDatas();
}
///
/// \brief 转换为QPointF数组
/// \param ptr SAAbstractDatas指针
/// \param res 结果
/// \return 可以转换返回true
///
bool SAVectorPointF::toPointFVector(const SAAbstractDatas *ptr, QVector<QPointF> &data)
{
    if(ptr->getDim() != SA::Dim1)
    {
        return false;
    }
    const SAVectorPointF* pointVector = dynamic_cast<const SAVectorPointF*>(ptr);
    if(pointVector)
    {
        pointVector->getValueDatas(data);
        return true;
    }

    //处理其它情况
    bool isSuccess = false;
    const int size = ptr->getSize(SA::Dim1);
    QVariant var;
    for(int i=0;i<size;++i)
    {
        var = ptr->getAt(i,0);
        if(var.canConvert<QPointF>())
        {
            data.append(var.toPointF());
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

