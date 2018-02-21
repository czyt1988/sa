#ifndef SADATATABLEMODEL_H
#define SADATATABLEMODEL_H
#include <QObject>
#include <QAbstractTableModel>
#include <QColor>
#include <QVector>
#include <QList>
#include "SALibGlobal.h"
#include <QHash>
#include <QMultiHash>
class SAAbstractDatas;

class SALIB_EXPORT SADataTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SADataTableModel(QObject *parent = 0);
    void setSADataPtr(SAAbstractDatas* data);
    void setSADataPtrs(const QList<SAAbstractDatas*>& datas);
    void appendSADataPtr(SAAbstractDatas* data);
    void appendSADataPtrs(QList<SAAbstractDatas*> datas);
    void getSADataPtrs(QList<SAAbstractDatas*>& data) const;

    void update();
    void saveToCsv(const QString& path);

    void removeDatas(const QList<SAAbstractDatas*>& datas);
    bool isEmpty() const;
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    void reCalcRowAndColumnCount();

    void toCsvCellString(QString& str);
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
