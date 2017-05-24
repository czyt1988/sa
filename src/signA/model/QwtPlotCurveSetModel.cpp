#include "QwtPlotCurveSetModel.h"
#define COLUMNCOUNT 4
//序列名称,颜色
QwtPlotCurveSetModel::QwtPlotCurveSetModel(QwtPlot* qwt,QObject* parent):QAbstractTableModel(parent)
  ,m_plot(qwt)
{
}

int QwtPlotCurveSetModel::rowCount(const QModelIndex& parent) const
{
    return m_plot->itemList(QwtPlotItem::Rtti_PlotCurve).size();
}

int QwtPlotCurveSetModel::columnCount(const QModelIndex& parent) const
{
    return COLUMNCOUNT;
}
