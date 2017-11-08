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
    enum RTTI
    {
        RTTIRectRegionSelectEditor ///<用于给图标添加矩形选框
        ,RTTIEllipseRegionSelectEditor ///<用于给图标添加椭圆选框
        ,RTTIPolygonRegionSelectEditor ///<用于给图标添加多边形选框

        ,RTTIUserDefine
    };

    const QwtPlot *plot() const;
    QwtPlot *plot();
    //是否启用编辑器
    virtual void setEnabled( bool on );
    virtual bool isEnabled() const;
    virtual int rtti() const = 0;
private:
    bool m_isEnable;///< 是否生效
};

#endif // SAABSTRACTPLOTEDITOR_H
