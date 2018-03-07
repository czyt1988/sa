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

int SAVectorPointF::getSize(int dim) const
{
    if(dim==SA::Dim1)
    {
        return this->SAVectorDatas<QPointF>::getSize(dim);
    }
    else if(SA::Dim2 == dim)
    {
        return 2;
    }
    return 0;
}

int SAVectorPointF::getDim() const
{
    return SA::Dim2;
}
///
/// \brief 若调调用dim1，将返回QVariant(QPointF),若调用(dim1,dim2)将返回QVariant(double)
///
/// 如：
/// \code
/// SAVectorPointF vf;
/// ......
/// QVariant var = vf.getAt({1});
/// //将返回第二个QPointF
/// var = vf.getAt({1,1});
/// //将返回第二个QPointF的y值，此时var是double
/// \endcode
/// \param index
/// \return
///
QVariant SAVectorPointF::getAt(const std::initializer_list<size_t> &index) const
{
    if(1 == index.size())
    {
        const QPointF& f = get (*index.begin());
        return QVariant::fromValue<double>(f.x());
    }
    else if(index.size()>=2)
    {
        const QPointF& f = get (*index.begin());
        int c = *(index.begin()+1);
        bool isZeroIndex = true;
        for(auto i=(index.begin()+2);i!=index.end();++i)
        {
            if(0!=(*i))
            {
                isZeroIndex = false;
                break;
            }
        }
        if(!isZeroIndex)
            return QVariant();

        if(0 == c)
        {
            return f.x();
        }
        else if(1 == c)
        {
            return f.y();
        }
        return QVariant();
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
    else if(2 == index.size())
    {
        const QPointF& f = get (*index.begin());
        size_t dim2 = *(index.begin()+1);
        return ((0 == dim2) ? QString::number(f.x()) : QString::number(f.y()));
    }
    return QString();
}
///
/// \brief setAt(double,{row,col})此时可以作为一个二维表格设置double
/// \param val
/// \param index
/// \return
///
bool SAVectorPointF::setAt(const QVariant &val, const std::initializer_list<size_t> &index)
{
    if(1 == index.size())
    {
        return SAVectorDatas<QPointF>::setAt(val,index);
    }
    else if(index.size()>=2)
    {
        bool isOK = false;
        double d = val.toDouble(&isOK);
        if(!isOK)
            return false;
        QPointF& f = get (*index.begin());
        int c = *(index.begin()+1);
        if(c >= 2)
            return false;
        for(auto i=(index.begin()+2);i!=index.end();++i)
        {
            if(0!=(*i))
            {
                return false;
            }
        }
        if(0 == c)
        {
            f.setX(d);
        }
        else if(1 == c)
        {
            f.setY(d);
        }
        return true;
    }
    return false;
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
    const SAVectorPointF* pointVector = dynamic_cast<const SAVectorPointF*>(ptr);
    if(pointVector)
    {
        pointVector->getValueDatas(data);
        return true;
    }
    if(ptr->getDim() != SA::Dim2)
    {
        return false;
    }

    //处理其它情况
    bool isXSuccess = false,isYSuccess = false;
    const int size = ptr->getSize(SA::Dim1);
    QVariant var;
    bool isOK = false;
    for(int i=0;i<size;++i)
    {
        var = ptr->getAt(i,0);
        double x = var.toDouble(&isXSuccess);
        var = ptr->getAt(i,1);
        double y = var.toDouble(&isYSuccess);
        if(isXSuccess && isYSuccess)
        {
            isOK = true;
            data.append(QPointF(x,y));
        }
    }
    return isOK;
}

