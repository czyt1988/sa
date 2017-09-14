#ifndef SAPLOTZOOMER_H
#define SAPLOTZOOMER_H
#include "SAChartGlobals.h"
#include "qwt_plot_zoomer.h"
class SA_CHART_EXPORT SAPlotZoomer : public QwtPlotZoomer
{
public:
    explicit SAPlotZoomer(QWidget *w, bool doReplot = true);
    explicit SAPlotZoomer( int xAxis, int yAxis,
                            QWidget *w, bool doReplot = true );

};

#endif // SAPLOTZOOMER_H
