#include "SAValueManagerMimeData.h"
#include <QDataStream>
SAValueManagerMimeData::SAValueManagerMimeData():QMimeData()
{

}
///
/// \brief 变量数据id的mime类型
/// \return
///
QString SAValueManagerMimeData::valueIDMimeType()
{
    return QStringLiteral("SA.SAValueManagerValueID");
}

///
/// \brief 设置变量id
/// \param valueIDs
///
void SAValueManagerMimeData::setValueIDs(const QList<int>& valueIDs)
{
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << valueIDs;
    setData(valueIDMimeType(),itemData);
}
///
/// \brief 获取变量id 调用getValueIDsFromMimeData
/// \return
/// \see getValueIDsFromMimeData
///
QList<int> SAValueManagerMimeData::getValueIDs() const
{
     QList<int> ids;
     getValueIDsFromMimeData(this,ids);
     return ids;
}
///
/// \brief 获取变量id
/// \param mimeData
/// \param ids
/// \return
///
bool SAValueManagerMimeData::getValueIDsFromMimeData(const QMimeData *mimeData, QList<int> &ids)
{
    QByteArray buffer = mimeData->data(valueIDMimeType());
    if(buffer.size() <= 0)
    {
        return false;
    }
    QDataStream dataStream(&buffer, QIODevice::ReadOnly);
    dataStream >> ids;
    return true;
}
