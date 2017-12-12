#ifndef SACHARTSERIALIZEHELP_H
#define SACHARTSERIALIZEHELP_H
#include <QDataStream>
#include "SACommonUIGlobal.h"
#include "SAChart2D.h"
#include "qwt_plot_item.h"
#include "qwt_text.h"
class QwtPlotCurve;
class QwtPlotCanvas;
class QFrame;
class QwtScaleWidget;

///
/// \brief 用于序列化图
///
class SA_COMMON_UI_EXPORT SAChartSerializeHelp
{
public:
    SAChartSerializeHelp();
    //序列化QwtPlotItem
    static void serializeOut(QDataStream& out,const QwtPlotItem* item);
    static void serializeIn(QDataStream& in,QwtPlotItem* item);
    //序列化SAChart2D -会保存item等额外信息
    static void serializeOut(QDataStream& out,const SAChart2D* chart);
    static void serializeIn(QDataStream& in,SAChart2D* chart);

};

#endif // SACHARTSERIALIZEHELP_H
