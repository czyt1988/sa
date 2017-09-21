#ifndef SACROSSTRACKER_H
#define SACROSSTRACKER_H
#include "SAChartGlobals.h"
#include <qwt_plot_picker.h>

class SA_CHART_EXPORT SACrossTracker : public QwtPlotPicker
{
public:
    SACrossTracker(QWidget *w);
    SACrossTracker(int xAxis, int yAxis,QWidget *w);
protected:
    virtual QwtText trackerTextF( const QPointF &pos ) const;

private:
    void init();
};

#endif // SACROSSTRACKER_H
