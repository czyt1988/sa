#include "SAPlotDataModel.h"
#include "SAFigureWindow.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAFigureOptCommands.h"
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

bool SAPlotDataModel::setItemData(int row, int col, QwtPlotItem *item, const QVariant &var)
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

    QScopedPointer<SAFiguresEditSeriesCommand> cmd;
    cmd.reset(new SAFiguresEditSeriesCommand(chart,item,d,row,col,tr("set curve data")));
    if(cmd->isValid())
    {
        bool isChangedSize = cmd->isSizeChanged();
        chart->appendCommand(cmd.take());
        if(isChangedSize)
        {
            updateColumnCount();
        }
        return true;
    }



    /*
    int rtti = item->rtti();
    switch(rtti)
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        if(col > 1)
            return false;
        QwtPlotCurve* p = static_cast<QwtPlotCurve*>(item);

        if(row < (int)p->dataSize())
        {
            QPointF newData = p->sample(row);
            if(0 == col)
            {
                newData.setX(d);
            }
            else
            {
                newData.setY(d);
            }
            SAFigureReplaceXYSeriesDataInIndexsCommand* cmd =
                    new SAFigureReplaceXYSeriesDataInIndexsCommand(chart
                                                                   ,p
                                                                   ,tr("set curve %1 data").arg(p->title().text())
                                                                   ,{row}
                                                                   ,{newData}
                                                                   );
            chart->appendCommand(cmd);
            return true;
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        if(col > 1)
            return false;
        QwtPlotBarChart* p = static_cast<QwtPlotBarChart*>(item);

        if(row < (int)p->dataSize())
        {
            QPointF newData = p->sample(row);
            if(0 == col)
            {
                newData.setX(d);
            }
            else
            {
                newData.setY(d);
            }
            SAFigureReplaceXYSeriesDataInIndexsCommand* cmd =
                    new SAFigureReplaceXYSeriesDataInIndexsCommand(chart
                                                                   ,p
                                                                   ,tr("set bar %1 data").arg(p->title().text())
                                                                   ,{row}
                                                                   ,{newData}
                                                                   );
            chart->appendCommand(cmd);
            return true;
        }


        return false;
    }
    case QwtPlotItem::Rtti_PlotSpectroCurve:
    {
        if(col > 2)
            return false;
        QwtPlotSpectroCurve* p = static_cast<QwtPlotSpectroCurve*>(item);
        if(row < (int)p->dataSize())
        {
            QwtPoint3D newData = p->sample(row);
            switch(col)
            {
            case 0:
                newData.setX(d);break;
            case 1:
                newData.setY(d);break;
            case 2:
                newData.setZ(d);break;
            default:
                return false;
            }
            SAFigureReplaceXYZSeriesDataInIndexsCommand* cmd =
                    new SAFigureReplaceXYZSeriesDataInIndexsCommand(chart
                                                                   ,p
                                                                   ,tr("set spectro curve %1 data").arg(p->title().text())
                                                                   ,{row}
                                                                   ,{newData}
                                                                   );
            chart->appendCommand(cmd);
            return true;
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotIntervalCurve:
    {
        if(col > 2)
            return false;
        QwtPlotIntervalCurve* p = static_cast<QwtPlotIntervalCurve*>(item);
        if(row < (int)p->dataSize())
        {
            QwtIntervalSample newData = p->sample(row);
            switch(col)
            {
            case 0:newData.value = d;break;
            case 1:newData.interval.setMinValue(d);break;
            case 2:newData.interval.setMaxValue(d);break;
            default:
                return false;
            }
            SAFigureReplaceIntervalSeriesDataInIndexsCommand* cmd =
                    new SAFigureReplaceIntervalSeriesDataInIndexsCommand(chart
                                                                   ,p
                                                                   ,tr("set interval curve %1 data").arg(p->title().text())
                                                                   ,{row}
                                                                   ,{newData}
                                                                   );
            chart->appendCommand(cmd);
            return true;
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotHistogram:
    {
        if(col > 2)
            return false;
        QwtPlotHistogram* p = static_cast<QwtPlotHistogram*>(item);
        if(row < (int)p->dataSize())
        {
            QwtIntervalSample newData = p->sample(row);
            switch(col)
            {
            case 0:newData.interval.setMinValue(d);break;
            case 1:newData.interval.setMaxValue(d);break;
            case 2:newData.value = d;break;
            default:
                return false;
            }
            SAFigureReplaceIntervalSeriesDataInIndexsCommand* cmd =
                    new SAFigureReplaceIntervalSeriesDataInIndexsCommand(chart
                                                                   ,p
                                                                   ,tr("set histogram %1 data").arg(p->title().text())
                                                                   ,{row}
                                                                   ,{newData}
                                                                   );
            chart->appendCommand(cmd);
            return true;
        }
        return false;
    }
    case QwtPlotItem::Rtti_PlotTradingCurve:
    {
        QwtPlotTradingCurve* p = static_cast<QwtPlotTradingCurve*>(item);
        if(col > 4)
            return false;
        if(row < (int)p->dataSize())
        {
            QwtOHLCSample newData = p->sample(row);
            switch(col)
            {
            case 0:
                newData.time = d;break;
            case 1:
                newData.open = d;break;
            case 2:
                newData.high = d;break;
            case 3:
                newData.low = d;break;
            case 4:
                newData.close = d;break;
            default:
                return false;
            }
            SAFigureReplaceOHLCSeriesDataInIndexsCommand* cmd =
                    new SAFigureReplaceOHLCSeriesDataInIndexsCommand(chart
                                                                   ,p
                                                                   ,tr("set trading curve %1 data").arg(p->title().text())
                                                                   ,{row}
                                                                   ,{newData}
                                                                   );
            chart->appendCommand(cmd);
            return true;
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotMultiBarChart:
    {
        QwtPlotMultiBarChart* p = static_cast<QwtPlotMultiBarChart*>(item);
        QVector<QwtSetSample> samples;
        SAChart::getSeriesData<QwtSetSample>(samples,p);
        if(row < (int)p->dataSize())
        {
            QwtSetSample newData = p->sample(row);
            if(0 == col)
            {
                newData.value = d;
            }
            else
            {
                if(col-1 < newData.set.size())
                {
                    newData.set[col-1] = d;
                }
                else
                {
                    return false;
                }
            }
            SAFigureReplaceMultiBarSeriesDataInIndexsCommand* cmd =
                    new SAFigureReplaceMultiBarSeriesDataInIndexsCommand(chart
                                                                   ,p
                                                                   ,tr("set multi bar %1 data").arg(p->title().text())
                                                                   ,{row}
                                                                   ,{newData}
                                                                   );
            chart->appendCommand(cmd);
            return true;
        }
        return false;
    }
    default:
        break;
    }
    */

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


