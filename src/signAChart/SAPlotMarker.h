#ifndef SAPLOTMARKER_H
#define SAPLOTMARKER_H
#include "SAChartGlobals.h"

#include <qwt_plot_marker.h>
///
/// \brief marker接口
///
class SA_CHART_EXPORT SAAbstractPlotMarker : public QwtPlotMarker
{
public:
    enum{
        Rtti_SAAbstractMarker = SARttiMarker_LowerBoundary
    ,Rtti_SAPointMarker///< SAPointMarker标记
    ,Rtti_SAYValueMarker///< SAYValueMarker
    ,Rtti_SAXValueMarker///< SAXValueMarker
    };
    enum{
        PointMarker=0 ///< 标记为点标记
        ,YValueMarker ///< y值标记，水平线
    };
    SAAbstractPlotMarker(const QString &title=QString::null);
    SAAbstractPlotMarker(const QwtText &title);
    virtual ~SAAbstractPlotMarker();
    virtual int rtti () const
    {
        return Rtti_SAAbstractMarker;
    }
    virtual int markerType() const = 0;
};
///
/// \brief 点标注marker
///
class SA_CHART_EXPORT SAPointMarker : public SAAbstractPlotMarker
{
public:
    SAPointMarker(const QString &title=QString::null);
    SAPointMarker(const QwtText &title);
    SAPointMarker(const QPointF &point);
    virtual ~SAPointMarker();
    virtual int rtti () const
    {
        return SAAbstractPlotMarker::Rtti_SAPointMarker;
    }
    virtual int markerType() const
    {
        return SAAbstractPlotMarker::PointMarker;
    }
};
///
/// \brief Y值标记
///
class SA_CHART_EXPORT SAYValueMarker : public SAAbstractPlotMarker
{
public:
    SAYValueMarker(const QString &title=QString::null);
    SAYValueMarker(const QwtText &title);
    SAYValueMarker(const double &value);
    virtual ~SAYValueMarker();
    virtual int rtti () const
    {
        return SAAbstractPlotMarker::Rtti_SAYValueMarker;
    }
    virtual int markerType() const
    {
        return SAAbstractPlotMarker::YValueMarker;
    }
};

#endif // SAPLOTMARKER_H
