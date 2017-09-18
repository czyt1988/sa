#ifndef SAABSTRACTPLOTEDITOR_H
#define SAABSTRACTPLOTEDITOR_H
#include "SAChartGlobals.h"
#include <QObject>
#include "qwt_plot.h"
///
/// \brief 绘图编辑器
///
class SA_CHART_EXPORT SAAbstractPlotEditor : public QObject
{
    Q_OBJECT
public:
    SAAbstractPlotEditor(QwtPlot* parent);
    virtual ~SAAbstractPlotEditor();
    const QwtPlot *plot() const;
    QwtPlot *plot();
    //是否启用编辑器
    virtual void setEnabled( bool on ) = 0;
    virtual bool isEnabled() const = 0;
};

#endif // SAABSTRACTPLOTEDITOR_H
