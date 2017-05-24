#ifndef SADATAREFERENCE_H
#define SADATAREFERENCE_H
#include <QVariant>
#include "SAAbstractDatas.h"

///
/// \brief 数据连接，不含有专门数据，包含数据的连接
///
class SALIB_EXPORT SADataReference : public SAAbstractDatas
{
public:
    SADataReference(SAAbstractDatas* origion);
    SADataReference(SAAbstractDatas* origion,const QString & text);

    virtual ~SADataReference();

    virtual int getType() const{return SA::DataLink;}
    virtual QString getTypeName() const;
    virtual int getSize(int dim=SA::Dim1) const;
    virtual int getDim() const;
    virtual QVariant getAt(const std::initializer_list<size_t>& index) const;
    virtual QString displayAt(const std::initializer_list<size_t>& index) const;
    void disLink();
    SAAbstractDatas* getLinkData() const{return m_linkData;}
    virtual void write(QDataStream & out) const;
private:
    SAAbstractDatas* m_linkData;
};

#endif // SADATAREFERENCE_H
