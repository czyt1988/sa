#ifndef SASERIES
#define SASERIES

#ifdef USE_QWT
    #include <qwt_plot_curve.h>
    #include <qwt_plot_histogram.h>
//    typedef QwtPlotItem QwtPlotItem;
//    typedef QwtPlotSeriesItem SAPlotSeriesItem;
//    typedef QwtPlotCurve QwtPlotCurve;
//    typedef QwtPlotHistogram QwtPlotHistogram;
#elif USE_QT_CHART

    #include <QXYSeries>
    #include <QBarSeries>

    typedef QObject SAPlotItem;
    typedef QXYSeries SAXYSeries;
    typedef QBarSeries SABarSeries;
#endif


#endif // SAABSTRACTSERIES

