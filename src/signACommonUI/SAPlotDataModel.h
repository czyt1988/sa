#ifndef SAPLOTDATAMODEL_H
#define SAPLOTDATAMODEL_H
#include "SACommonUIGlobal.h"
#include "QwtPlotItemDataModel.h"
class SAPlotDataModelPrivate;
class SAFigureWindow;
class SAChart2D;
class QwtPlotItem;
///
/// \brief QwtPlotItemDataModel的扩展，主要为了支持redo/undo
///
class SA_COMMON_UI_EXPORT SAPlotDataModel : public QwtPlotItemDataModel
{
    Q_OBJECT
    SA_IMPL(SAPlotDataModel)
public:
    SAPlotDataModel(QObject *p = 0);
    ~SAPlotDataModel();
    SAFigureWindow *getFigure() const;
    void setFigure(SAFigureWindow *fig);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    //设置数据 row col 要对应item的维度
    virtual bool setItemData(int row,int col,QwtPlotItem* item,const QVariant& var);

    //更新整个model
    void updateModel();
    //更新行
    void updateRow();
private:
    friend class AutoResetSAPlotDataModel;
};

#endif // SAPLOTDATAMODEL_H
