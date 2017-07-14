#include "SAPlotZoomer.h"

SAPlotZoomer::SAPlotZoomer(QWidget *w, bool doReplot)
    :QwtPlotZoomer(w,doReplot)
{

}

SAPlotZoomer::SAPlotZoomer(int xAxis, int yAxis, QWidget *w, bool doReplot)
    :QwtPlotZoomer(xAxis,yAxis,w,doReplot)
{

}
