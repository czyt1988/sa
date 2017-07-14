#ifndef SAPLOTZOOMER_H
#define SAPLOTZOOMER_H

#include "qwt_plot_zoomer.h"
class SAPlotZoomer : public QwtPlotZoomer
{
public:
    explicit SAPlotZoomer(QWidget *w, bool doReplot = true);
    explicit SAPlotZoomer( int xAxis, int yAxis,
                            QWidget *w, bool doReplot = true );

};

#endif // SAPLOTZOOMER_H
