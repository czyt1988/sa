#include "SAPlotDataModel.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAFigureTableCommands.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_intervalcurve.h"
#include "qwt_plot_barchart.h"
#include "qwt_plot_multi_barchart.h"
#include "qwt_plot_spectrocurve.h"
#include "qwt_plot_tradingcurve.h"


class SAPlotDataModelPrivate
{
    SA_IMPL_PUBLIC(SAPlotDataModel)
public:
    SAFigureWindow* m_fig;
    SAPlotDataModelPrivate(SAPlotDataModel* d):q_ptr(d)
    {

    }
};

class AutoResetSAPlotDataModel
{
public:
    AutoResetSAPlotDataModel(SAPlotDataModel* model):m_model(model)
    {
        m_model->beginResetModel();
    }
    ~AutoResetSAPlotDataModel()
    {
        m_model->endResetModel();
    }
private:
    SAPlotDataModel* m_model;
};


SAPlotDataModel::SAPlotDataModel(QObject *p):QwtPlotItemDataModel(p)
  ,d_ptr(new SAPlotDataModelPrivate(this))
{

}

SAPlotDataModel::~SAPlotDataModel()
{

}

int SAPlotDataModel::rowCount(const QModelIndex &parent) const
{
    return QwtPlotItemDataModel::rowCount(parent) + 1;
}

int SAPlotDataModel::columnCount(const QModelIndex &parent) const
{
    return QwtPlotItemDataModel::columnCount(parent) + 1;
}

bool SAPlotDataModel::setPlotItemData(int row, int col, QwtPlotItem *item, const QVariant &var)
{
    if(row < 0 || col < 0 || !var.isValid() || nullptr == item)
        return false;
    bool isOK = false;
    double d = var.toDouble(&isOK);
    if(!isOK)
        return false;
    SAChart2D * chart = d_ptr->m_fig->findChartFromItem(item);
    if(nullptr == chart)
        return false;

    AutoResetSAPlotDataModel autoResetModel(this);
    Q_UNUSED(autoResetModel);

    //new SAFiguresDataCommandEditSeries(chart,item,d,row,col,tr("set curve data"));

    QScopedPointer<SAFigureTableEditSeriesCommand> cmd;
    cmd.reset(new SAFigureTableEditSeriesCommand(chart,item,d,row,col,tr("set curve data")));
    if(cmd->isValid())
    {
        bool isChangedSize = cmd->isSizeChanged();
        chart->appendCommand(cmd.take());
        if(isChangedSize)
        {
            beginResetModel();
            updateRowCount();
            endResetModel();
        }
        return true;
    }
    return false;
}


SAFigureWindow *SAPlotDataModel::getFigure() const
{
    return d_ptr->m_fig;
}

void SAPlotDataModel::setFigure(SAFigureWindow *fig)
{
    d_ptr->m_fig = fig;
}

void SAPlotDataModel::updateModel()
{
    beginResetModel();
    updateRowCount();
    updateColumnCount();
    updateItemColor();
    endResetModel();
}

void SAPlotDataModel::updateRow()
{
    beginResetModel();
    updateRowCount();
    endResetModel();
}


