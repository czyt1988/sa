#ifndef SAVECTORINTERVAL_H
#define SAVECTORINTERVAL_H

#define SA_QWT

#include "SAVectorDatas.h"

#ifdef SA_QWT
#include "qwt_samples.h"
#endif
///
/// \brief 保存区间数据的数组
///
class SALIB_EXPORT SAVectorInterval : public SAVectorDatas<QwtIntervalSample>
{
public:
    SAVectorInterval();
    SAVectorInterval(const QString & name);
    SAVectorInterval(const QString& name,const QVector<QwtIntervalSample>& datas);

    virtual ~SAVectorInterval(){}
    virtual int getType() const   {return SA::VectorInterval;}
    virtual QString getTypeName() const{return QString("interval Vector");}

    virtual QVariant getAt (const std::initializer_list<size_t>& index) const;
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
    virtual void write(QDataStream & out) const;

};
Q_DECLARE_METATYPE(QwtIntervalSample)
SALIB_EXPORT QDataStream &operator<<(QDataStream & out, const QwtIntervalSample & item);
SALIB_EXPORT QDataStream &operator>>(QDataStream & in, QwtIntervalSample & item);
SALIB_EXPORT QDataStream &operator<<(QDataStream & out, const QwtInterval & item);
SALIB_EXPORT QDataStream &operator>>(QDataStream & in, QwtInterval & item);


#endif // SAVECTORINTERVAL_H
