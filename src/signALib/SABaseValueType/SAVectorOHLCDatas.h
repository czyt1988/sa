#ifndef SAVECTOROHLCDATAS_H
#define SAVECTOROHLCDATAS_H
#define SA_QWT

#include "SAVectorDatas.h"

#ifdef SA_QWT
#include "qwt_samples.h"
#endif
///
/// \brief 内置格式对QwtOHLCSample的封装
/// 此数据实际是一个二维数据，n*5
///
class SALIB_EXPORT SAVectorOHLCDatas : public SAVectorDatas<QwtOHLCSample>
{
public:
    SAVectorOHLCDatas();
    SAVectorOHLCDatas(const QString & name);
    SAVectorOHLCDatas(const QString& name,const QVector<QwtOHLCSample>& datas);
    virtual ~SAVectorOHLCDatas();
    virtual int getType() const   {return SA::VectorOHLC;}
    virtual QString getTypeName() const{return QString("OHLC Vector");}
    virtual QVariant getAt (const std::initializer_list<size_t>& index) const;
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
    virtual void write(QDataStream & out) const;
    virtual int getDim() const;
    virtual int getSize(int dim=SA::Dim1) const;
    virtual bool setAt(const QVariant &val, const std::initializer_list<size_t> &index);
};
Q_DECLARE_METATYPE(QwtOHLCSample)
SALIB_EXPORT QDataStream &operator<<(QDataStream & out, const QwtOHLCSample & item);
SALIB_EXPORT QDataStream &operator>>(QDataStream & in, QwtOHLCSample & item);


#endif // SAVECTOROHLCDATAS_H
