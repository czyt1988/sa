#include "SAMimeData.h"
#include <SAGlobals.h>
SAAbstractMimeData::SAAbstractMimeData()
{
}


void SAAbstractMimeData::setIndexList(const QModelIndexList& list)
{
    m_indexList = list;
}





SAValueTreeMimeData::SAValueTreeMimeData()
{
    m_mimeTypes<<SA_MIME_ValueManagerItem;
}

SAValueTreeMimeData::~SAValueTreeMimeData()
{

}

QStringList SAValueTreeMimeData::getMimeTypes() const
{
    return m_mimeTypes;
}

QString SAValueTreeMimeData::getMimeType(int index) const
{
    return m_mimeTypes.at (index);
}

size_t SAValueTreeMimeData::getTypesCount() const
{
    return m_mimeTypes.size ();
}
///
/// \brief 设置数据指针
/// \param ptrs
///
void SAValueTreeMimeData::setDataPtr(const QList<SAAbstractDatas *> &ptrs)
{
    m_dataPtrs = ptrs;
}
///
/// \brief 获取数据指针
/// \return
///
QList<SAAbstractDatas *> SAValueTreeMimeData::getDataPtrs() const
{
    return m_dataPtrs;
}


