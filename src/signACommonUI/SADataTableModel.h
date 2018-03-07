#ifndef SADATATABLEMODEL_H
#define SADATATABLEMODEL_H
#include <QObject>
#include <QAbstractTableModel>
#include <QColor>
#include <QVector>
#include <QList>
#include "SACommonUIGlobal.h"
#include <QHash>
#include <QMultiHash>
#include <functional>
class SAAbstractDatas;

class SA_COMMON_UI_EXPORT SADataTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    typedef std::function<bool(int,int,const QVariant&)> FUN_SET_DATA;
    SADataTableModel(QObject *parent = 0);
    //设置数据
    void setSADataPtr(SAAbstractDatas* data);
    void setSADataPtrs(const QList<SAAbstractDatas*>& datas);
    //插入数据
    void appendSADataPtr(SAAbstractDatas* data);
    void appendSADataPtrs(QList<SAAbstractDatas*> datas);
    //获取数据
    const QList<SAAbstractDatas*>& getSADataPtrs() const;
    //更新模型
    void update();
    //清空
    void clear();
    //移除显示的数据
    void removeDatas(const QList<SAAbstractDatas*>& datas);
    //判断是否为空
    bool isEmpty() const;
    //根据列号获取对应的数据
    SAAbstractDatas* columnToData(int c);
    //根据数据获取对应的列范围
    void dataColumnRange(SAAbstractDatas* p,int& start,int &end);
    ///
    /// \brief setData时的函数指针
    /// bool(int,int,const QVariant&)形式
    ///
    FUN_SET_DATA onSetDataFun;
    //判断当前行列是否在数据范围里，而不是在空白处
    bool isInDataRange(int row , int col) const;
public:
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    //数据区的行号 区别于rowCount是显示区的
    int dataRowCount() const;
    //数据区的列号 区别于columnCount是显示区的
    int dataColumnCount() const;
private:
    void reCalcRowAndColumnCount();
private:
    QVariant dataToDim0(int row,int col, SAAbstractDatas* d) const;
    QVariant dataToDim1(int row,int col, SAAbstractDatas* d) const;
    QVariant dataToDim2(int row,int col, SAAbstractDatas* d) const;

    QVariant horizontalHeaderToDim0(int section, SAAbstractDatas* d) const;
    QVariant horizontalHeaderToDim1(int section, SAAbstractDatas* d) const;
    QVariant horizontalHeaderToDim2(int section, SAAbstractDatas* d) const;
private:
    int m_rowCount;
    int m_columnCount;
    int m_columnShowMin;
    int m_rowShowMin;
    QList<SAAbstractDatas*> m_datas;
    QHash<int,SAAbstractDatas*> m_col2Ptr;
    QMultiHash<SAAbstractDatas*,int> m_ptr2Col;///记录指针对应的列表的所有列号，此用来加快删除速度

    QHash<SAAbstractDatas*,QHash<int,int> >m_ptr2ColMap;///< 记录指针对应的列号映射表
};

#endif // SADATATABLEMODEL_H
