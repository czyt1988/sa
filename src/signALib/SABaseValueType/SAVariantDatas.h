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
    SAVariantDatas(const QVariant& d);
    SAVariantDatas(int d);
    SAVariantDatas(float d);
    SAVariantDatas(double d);
    SAVariantDatas(char d);
    SAVariantDatas(unsigned int d);
    SAVariantDatas(unsigned char d);
    SAVariantDatas(const QString& d);
    SAVariantDatas(const QPoint& d);
    SAVariantDatas(const QPointF& d);
    SAVariantDatas(const QHash<QString, QVariant>& d);
    SAVariantDatas(const QMap<QString, QVariant>& d);
    SAVariantDatas(const QList<QVariant>& d);
    virtual int getType() const;
    virtual QString getTypeName() const;

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


