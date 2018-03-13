#ifndef SAPLOTDATAMODEL_H
#define SAPLOTDATAMODEL_H
#include "SACommonUIGlobal.h"
#include "QwtPlotItemDataModel.h"
class SAFigureWindow;
class SAChart2D;
class QwtPlotItem;
///
/// \brief QwtPlotItemDataModel的扩展，主要为了支持redo/undo
///
class SA_COMMON_UI_EXPORT SAPlotDataModel : public QwtPlotItemDataModel
{
    Q_OBJECT
public:
    SAPlotDataModel(QObject *p = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    //设置数据 row col 要对应item的维度
    virtual bool setItemData(int row,int col,QwtPlotItem* item,const QVariant& var);

    SAFigureWindow *getFigure() const;
    void setFigure(SAFigureWindow *fig);
private:
    //通过item找到对应的chart
    SAChart2D* getChartFromItem(QwtPlotItem* item);
private:
    SAFigureWindow* m_fig;
};

#endif // SAPLOTDATAMODEL_H
