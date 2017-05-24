#ifndef SASINGLEDATAS
#define SASINGLEDATAS
#include "SAAbstractDatas.h"

class SADataReference;

///
/// \brief 基本数据容器
///
template<typename DATA_TYPE>
class SALIB_EXPORT SASingleDatas : public SAAbstractDatas
{
public:
    virtual int getSize(int dim=SA::Dim1) const
    {
        Q_UNUSED(dim);
        if(SA::Dim0 == dim || SA::Dim1== dim  || SA::Dim2== dim)
        {
            return 1;
        }
        return 0;
    }
    //获取维度
    virtual int getDim() const {return 1;}
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const
    {
        if(0 == index.size())
        {
            return QVariant::fromValue<DATA_TYPE>(m_d);
        }
        return QVariant();
    }
    virtual QString displayAt(const std::initializer_list<size_t>& index) const
    {
        return getAt(index).toString();
    }
    DATA_TYPE& innerData(){return m_d;}
    const DATA_TYPE& innerData() const {return m_d;}

    virtual void read(QDataStream & in)
    {
        SAAbstractDatas::read(in);
        in >> m_d;
    }

    virtual void write(QDataStream & out) const
    {
        SAAbstractDatas::write(out);
        out << m_d;
    }
protected:
    DATA_TYPE m_d;
};



#endif // SASINGLEDATAS



