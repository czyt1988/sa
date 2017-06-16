#include "SASeriesAndDataPtrMapper.h"

SASeriesAndDataPtrMapper::SASeriesAndDataPtrMapper()
{

}

void SASeriesAndDataPtrMapper::insertData(SAAbstractDatas *d)
{
    m_linkDatas.insert(d);
}

void SASeriesAndDataPtrMapper::removeLinkDataPtr(const SAAbstractDatas *ptr)
{
    m_linkDatas.remove(const_cast<SAAbstractDatas *>(ptr));
}

bool SASeriesAndDataPtrMapper::isContainData(SAAbstractDatas *d)
{
    return m_linkDatas.contains(d);
}

const QSet<SAAbstractDatas *> &SASeriesAndDataPtrMapper::linkDatas() const
{
    return m_linkDatas;
}

QSet<SAAbstractDatas *> &SASeriesAndDataPtrMapper::linkDatas()
{
    return m_linkDatas;
}

void SASeriesAndDataPtrMapper::clearDataPtrLink()
{
    m_linkDatas.clear();
}
