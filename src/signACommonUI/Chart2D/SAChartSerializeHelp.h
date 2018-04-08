#ifndef SACHARTSERIALIZEHELP_H
#define SACHARTSERIALIZEHELP_H
#include <QDataStream>
#include "SACommonUIGlobal.h"
#include "SAChart2D.h"
#include "qwt_plot_item.h"
#include "qwt_text.h"
class SASeriesAndDataPtrMapper;
class SAXYSeries;
class QwtPlotCanvas;
class QFrame;
class QwtScaleWidget;
class SAScatterSeries;


// SAChart2D的序列化
SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SAChart2D* chart);
SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SAChart2D* chart);

// SAXYSeries的序列化
SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SAXYSeries* p);
SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SAXYSeries* p);
// SABarSeries的序列化
SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SABarSeries* p);
SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SABarSeries* p);
// SAScatterSeries的序列化
SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SAScatterSeries* p);
SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SAScatterSeries* p);


#endif // SACHARTSERIALIZEHELP_H
