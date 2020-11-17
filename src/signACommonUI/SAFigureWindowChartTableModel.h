#ifndef SAFIGUREWINDOWCHARTTABLEMODEL_H
#define SAFIGUREWINDOWCHARTTABLEMODEL_H
#include "SACommonUIGlobal.h"
#include <QAbstractTableModel>
#include "SAFigureWindow.h"
SA_IMPL_FORWARD_DECL(SAFigureWindowChartTableModel)

/**
 * @brief 用于显示figure window下的chart
 */
class SA_COMMON_UI_EXPORT SAFigureWindowChartTableModel : public QAbstractTableModel
{
    SA_IMPL(SAFigureWindowChartTableModel)
public:
    SAFigureWindowChartTableModel(QObject *par = nullptr);
    ~SAFigureWindowChartTableModel();
    //设置figure
    void setFigure(SAFigureWindow *fig);

    //刷新
    void refresh();

public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};

#endif // SAFIGUREWINDOWCHARTTABLEMODEL_H
