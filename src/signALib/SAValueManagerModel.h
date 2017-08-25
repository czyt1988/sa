#ifndef SAVALUEMANAGERMODEL_H
#define SAVALUEMANAGERMODEL_H
#include <QAbstractItemModel>
#include "SALibGlobal.h"
#include "SAItem.h"
#include <QHash>
#include <functional>
#include <QSet>
class SAAbstractDatas;
class SALIB_EXPORT SAValueManagerModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    typedef std::function<QVariant(const QModelIndex &,SAAbstractDatas*)> FunBackgroundShowPtr;
    typedef std::function<bool(const QModelIndex&)> FunErgodicItemPtr;
    SAValueManagerModel(QObject * parent = nullptr);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

public:
    //把QModelIndex转为SAAbstractDatas
    SAAbstractDatas* castToDataPtr(const QModelIndex &index) const;
    static SAItem *toItemPtr(const QModelIndex &index);
    //根据数据类型获取对应的图标
    static QIcon getDataIcon(const SAAbstractDatas* data);
    //注册类型对应的图标
    static void registerDataTypeIcon(int type,const QIcon& ico);
    //添加背景颜色显示的回调函数
    void setFunBackgroundData(FunBackgroundShowPtr funBackgroundData);
    //迭代所有条目
    void ergodicAllItem( FunErgodicItemPtr pFun);
    //找出参数对应的条目index
    QModelIndexList datasToIndexs(const QSet<SAAbstractDatas*>& data);
private:
    //从index开始往下迭代
    bool ergodicItem(const QModelIndex &index,FunErgodicItemPtr pFun);
private slots:
    void onDataAdded(const QList<SAAbstractDatas*>& datas);
    void onDataDeleted(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
    void onDataClear();
    void onDataNameChanged(SAAbstractDatas* data,const QString& oldName);
private:
    void resetModel();
private:
    static QHash<int,QIcon> s_dataType2Icon; ///<保存数据类型对应的图标
    FunBackgroundShowPtr m_funBackgroundData;
};

#endif // SAVALUEMODEL_H
