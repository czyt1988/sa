#ifndef SAVALUEMANAGERMIMEDATA_H
#define SAVALUEMANAGERMIMEDATA_H
#include <QMimeData>
#include "SALibGlobal.h"
///
/// \brief 用于拖曳的选中的变量管理的数据id
///
class SALIB_EXPORT SAValueManagerMimeData : public QMimeData
{
public:
    SAValueManagerMimeData();
    //变量数据id的mime类型
    static QString valueIDMimeType();
    //设置变量id
    void setValueIDs(const QList<int>& valueID);
    //获取变量id 调用getValueIDsFromMimeData
    QList<int> getValueIDs() const;
    //获取变量id
    static bool getValueIDsFromMimeData(const QMimeData* data,QList<int> &ids);
};

#endif // SAVALUEMANAGERMIMEDATA_H
