#ifndef SAPLOTPICKER_H
#define SAPLOTPICKER_H
#include "qwt_plot_picker.h"
#include "SAChartGlobals.h"

class SA_CHART_EXPORT SAPlotPicker : public QwtPlotPicker
{
public:
    explicit SAPlotPicker( QWidget *canvas );
    explicit SAPlotPicker( int xAxis, int yAxis, QWidget * );
    explicit SAPlotPicker( int xAxis, int yAxis,
                           RubberBand rubberBand, DisplayMode trackerMode, QWidget * );

};

#endif // SAPLOTPICKER_H
