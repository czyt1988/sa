#ifndef QCZYHASHTABLEMODEL_H
#define QCZYHASHTABLEMODEL_H
#include <QAbstractTableModel>
#include <czyQtApp.h>//QczyHashTable使用
class SAVariantHashTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SAVariantHashTableModel(QObject * parent=0);
    virtual ~SAVariantHashTableModel();
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const = 0;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void update();
public:
    typedef SA::SAHashTable<QVariant> Table;
    void setHashTableData(Table* prt);
    const Table* getHashTableData() const;
    Table* getHashTableData();
private:
    Table* m_data;
};

#endif // QCZYHASHTABLEMODEL_H
