#ifndef QTDMCHANNELTABLEMODEL_H
#define QTDMCHANNELTABLEMODEL_H
#include <QObject>
#include <QAbstractTableModel>
#include <QList>
#include "SALibGlobal.h"
class QTDMChannel;
class SALIB_EXPORT QTDMChannelTableModel : public QAbstractTableModel
{
public:
    QTDMChannelTableModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    void update()
    {
        beginResetModel();
        endResetModel();
    }
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setChannels(const QList<QTDMChannel*>& channels);
private:
    QList<QTDMChannel*> m_datas;
    int m_rowCount;
};

#endif // QTDMCHANNELTABLEMODEL_H
