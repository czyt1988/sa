#ifndef SAVALUEMANAGERLISTMODEL_H
#define SAVALUEMANAGERLISTMODEL_H
#include "SALibGlobal.h"
#include "SAItem.h"
#include <QAbstractListModel>
#include <functional>
class SAAbstractDatas;
class SAValueManagerListModelPrivate;
///
/// \brief 变量的list model
///
class SALIB_EXPORT SAValueManagerListModel : public QAbstractListModel
{
    Q_OBJECT
    SA_IMPL(SAValueManagerListModel)
public:
    typedef std::function<bool(const SAAbstractDatas*,int)> FunPtrFilter;
    explicit SAValueManagerListModel(QObject *parent = 0);
    ~SAValueManagerListModel();
    //
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //设置过滤器，过滤器会按照规则过滤不显示的条目
    void setFilter(FunPtrFilter filterFun);
private slots:
    void onDataChanged(const QList<SAAbstractDatas*>& datas);
    void onDataClear();
private:
    void updateData();
public:
    //把QModelIndex转为SAAbstractDatas
    SAAbstractDatas* castToDataPtr(const QModelIndex &index) const;
};
// 只保留1维数据
bool SALIB_EXPORT filterSAValueManagerListModel1Dim(const SAAbstractDatas*p,int index);

#endif // SAVALUEMANAGERLISTMODEL_H
