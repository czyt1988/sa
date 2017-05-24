#ifndef SAVARIANTDATAS_H
#define SAVARIANTDATAS_H
#include "SASingleDatas.h"
#include <QVariant>
#include <QPoint>
#include <QPointF>
#include <QString>
#include <QHash>
#include <QMap>
#include <QList>
///
/// \brief sa的基本数据，用于保存0维点数据
///
class SALIB_EXPORT SAVariantDatas : public SASingleDatas<QVariant>
{
public:
    SAVariantDatas(const QVariant& d){m_d = d;}
    SAVariantDatas(int d){m_d = d;}
    SAVariantDatas(float d){m_d = d;}
    SAVariantDatas(double d){m_d = d;}
    SAVariantDatas(char d){m_d = d;}
    SAVariantDatas(unsigned int d){m_d = d;}
    SAVariantDatas(unsigned char d){m_d = d;}
    SAVariantDatas(const QString& d){m_d = d;}
    SAVariantDatas(const QPoint& d){m_d = d;}
    SAVariantDatas(const QPointF& d){m_d = d;}
    SAVariantDatas(const QHash<QString, QVariant>& d){m_d = d;}
    SAVariantDatas(const QMap<QString, QVariant>& d){m_d = d;}
    SAVariantDatas(const QList<QVariant>& d){m_d = d;}
    virtual int getType() const{return SA::Variant;}
    virtual QString getTypeName() const{return "var";}

    ///
    /// \brief 转换为某数据，需要调用canConvert进行预先判断
    /// \return
    ///
    template<typename DATA_TYPE>
    DATA_TYPE toData() const
    {
        return innerData().value<DATA_TYPE>();
    }

    ///
    /// \brief toData 转换的静态函数
    /// \note will call dynamic_cast
    /// \param dptr SAAbstractDatas的指针
    /// \param data
    /// \return
    ///
    template<typename DATA_TYPE>
    static bool toData(const SAAbstractDatas* dptr, DATA_TYPE& data)
    {
        const SAVariantDatas* var = dynamic_cast<const SAVariantDatas*>(dptr);
        if(var == nullptr)
        {
            return false;
        }
        if(!var->canConvert<DATA_TYPE>())
        {
            return false;
        }
        data = var->toData<DATA_TYPE>();
        return true;
    }

    ///
    /// \brief 用于判断是否可以转换为某种类型
    /// \return
    ///
    template<typename DATA_TYPE>
    bool canConvert() const
    {
        return innerData().canConvert<DATA_TYPE>();
    }
};



#endif // SAINT_H
