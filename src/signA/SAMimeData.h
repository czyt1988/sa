#ifndef SAABSTRACTMIMEDATA_H
#define SAABSTRACTMIMEDATA_H
#include <QMimeData>
#include <QModelIndexList>
#include "SAAbstractDatas.h"

#ifndef SA_MIME_TYPE_VALUE_TREE
#define SA_MIME_TYPE_VALUE_TREE 1
#endif
class SAAbstractMimeData : public QMimeData
{
public:
    SAAbstractMimeData();
    virtual int getType() const = 0;
    virtual QStringList getMimeTypes() const = 0;
    virtual QString getMimeType(int index) const = 0;
    virtual size_t getTypesCount() const = 0;

    void setIndexList(const QModelIndexList& list);
    QModelIndexList getIndexList() const{return m_indexList;}
private:
    QModelIndexList m_indexList;
};

class SAValueTreeMimeData : public SAAbstractMimeData
{
public:
    SAValueTreeMimeData();
    virtual ~SAValueTreeMimeData();
    virtual int getType() const{return SA_MIME_TYPE_VALUE_TREE;}
    virtual QStringList getMimeTypes() const;
    virtual QString getMimeType(int index) const;
    virtual size_t getTypesCount() const;

    //设置数据指针
    void setDataPtr(const QList<SAAbstractDatas*>& ptrs);
    //获取数据指针
    QList<SAAbstractDatas*> getDataPtrs() const;
private:
    QStringList m_mimeTypes;
    QList<SAAbstractDatas*> m_dataPtrs;
};

#endif // SAABSTRACTMIMEDATA_H
