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
///
/// \brief 用于序列化图
///
class SA_COMMON_UI_EXPORT SAChartSerializeHelp
{
public:
    SAChartSerializeHelp();
    //序列化SAChart2D -会保存item等额外信息
    static void serializeOut(QDataStream& out,const SAChart2D* chart);
    static void serializeIn(QDataStream& in,SAChart2D* chart);
private:
    static void serializeOutChartItem(QDataStream& out,const SAChart2D* chart);
    static void serializeInChartItem(QDataStream& in,SAChart2D* chart);

};

namespace sa {
    // SASeriesAndDataPtrMapper的序列化
    SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SASeriesAndDataPtrMapper* p);
    SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SASeriesAndDataPtrMapper* p);
    // SAXYSeries的序列化
    SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SAXYSeries* p);
    SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SAXYSeries* p);
    // SABarSeries的序列化
    SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SABarSeries* p);
    SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SABarSeries* p);
    // SAScatterSeries的序列化
    SA_COMMON_UI_EXPORT QDataStream& operator <<(QDataStream & out,const SAScatterSeries* p);
    SA_COMMON_UI_EXPORT QDataStream& operator >>(QDataStream & in,SAScatterSeries* p);
}


#endif // SACHARTSERIALIZEHELP_H
