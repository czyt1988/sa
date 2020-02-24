#ifndef QCSVFILEMODEL_H
#define QCSVFILEMODEL_H
#include <QAbstractTableModel>
#include <QVector>
#include <QList>
#include <algorithm>
#include <vector>

#include <czyQtApp.h>//QTable使用
#include <SAVariantHashTableModel.h>
class SACsvFileTableModel : public SAVariantHashTableModel
{
public:
    SACsvFileTableModel(const QString& fileName, QObject * parent = 0);
    bool setFile(const QString& fileName);
    void splitCsvLine(const QString &lineStr,size_t rows);

    QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const; 
    static bool getCzyHashTable(const QString& fileName,SA::SAHashTable<QVariant>& data);
private:
    static int advquoted(const QString& s, QString& fld, int i);
    static int advplain(const QString& s, QString& fld, int i);
    SAVariantHashTableModel::Table m_t;
    bool m_enableEdit;
};
#endif // QCSVFILEMODEL_H
