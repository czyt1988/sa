#ifndef SAQWTSERIALIZE_H
#define SAQWTSERIALIZE_H
#include "SAChartGlobals.h"
#include <QDataStream>
#include "qwt_text.h"
#include "qwt_samples.h"
class QwtPlotItem;
class QwtSymbol;
class QwtPlotCurve;
class QwtPlotCanvas;
class QFrame;
class QwtScaleWidget;
class QwtPlot;
class QwtColorMap;
class QwtScaleDraw;
namespace sa {
    ///< QFrame的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QFrame* f);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QFrame* f);

    ///< QwtText的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtText& t);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtText& t);
    ///< QwtSymbol的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtSymbol* t);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtSymbol* t);
    ///< QwtPlotItem指针的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtPlotItem* item);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtPlotItem* item);
    /// QwtScaleWidget指针的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtScaleWidget* w);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtScaleWidget* w);
    /// QwtScaleDraw指针的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtScaleDraw* w);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtScaleDraw* w);
    /// QwtPlotCurve指针的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtPlotCurve* item);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtPlotCurve* item);
    ///< QwtPlotCanvas的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtPlotCanvas* c);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtPlotCanvas* c);
    ///< QwtColorMap的序列化
//    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtColorMap* c);
//    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtColorMap* c);
    ///< QwtPlot的序列化
    SA_CHART_EXPORT QDataStream& operator <<(QDataStream & out,const QwtPlot* chart);
    SA_CHART_EXPORT QDataStream& operator >>(QDataStream & in,QwtPlot* chart);
    ///<QwtIntervalSample的序列化
    SA_CHART_EXPORT QDataStream &operator<<(QDataStream & out, const QwtIntervalSample & item);
    SA_CHART_EXPORT QDataStream &operator>>(QDataStream & in, QwtIntervalSample & item);
    ///<QwtInterval的序列化
    SA_CHART_EXPORT QDataStream &operator<<(QDataStream & out, const QwtInterval & item);
    SA_CHART_EXPORT QDataStream &operator>>(QDataStream & in, QwtInterval & item);
}

#endif // SAQWTSERIALIZE_H
