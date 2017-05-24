#ifndef QWTPLOTCURVESETMODEL_H
#define QWTPLOTCURVESETMODEL_H
#include <QAbstractTableModel>
#include <QList>
#include <algorithm>
#include <vector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
class QwtPlotCurveSetModel : public QAbstractTableModel
{
public:
    QwtPlotCurveSetModel(QwtPlot* qwt,QObject *parent = 0);
    void update()
    {
        beginResetModel();
        endResetModel();
    }
    int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QwtPlot* m_plot;
};

#endif // QWTPLOTCURVESETMODEL_H
