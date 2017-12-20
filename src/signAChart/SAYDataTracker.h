#ifndef SAYDATATRACKER_H
#define SAYDATATRACKER_H
#include "SAChartGlobals.h"
#include <qwt_plot_picker.h>
class QwtPlotItem;
class QwtPlotCurve;
class QwtPlotBarChart;
class SA_CHART_EXPORT SAYDataTracker: public QwtPlotPicker
{
public:
    SAYDataTracker( QWidget * );

protected:
    virtual QwtText trackerTextF( const QPointF & ) const;
    virtual QRect trackerRect( const QFont & ) const;
    //内部实现使用了dynamic_cast
    virtual QString itemInfoAt(const QwtPlotItem* item,const QPointF &pos) const;
private:
    QString curveInfoAt( const QwtPlotCurve *, const QPointF & ) const;
    QLineF curveLineAt( const QwtPlotCurve *, double x ) const;
    QString barInfoAt( const QwtPlotBarChart *, const QPointF & ) const;
    double barValueAt( const QwtPlotBarChart *bar, double x ) const;
};

#endif // SAYDATATRACKER_H
