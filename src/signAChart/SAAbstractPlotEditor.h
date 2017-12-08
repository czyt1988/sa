#ifndef SAABSTRACTPLOTEDITOR_H
#define SAABSTRACTPLOTEDITOR_H
#include "SAChartGlobals.h"
#include <QObject>
#include "qwt_plot.h"
///
/// \brief 绘图编辑器
/// 此编辑器会屏蔽长按空格的keyPressEvent和keyReleaseEvent
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
    //设置是否使用空格，如果这个editor使用了空格，需要设置为true，否则会被屏蔽空格的输入,默认为false
    void setUseSpaceKey(bool b);
    bool isUseSpaceKey() const;
protected:
    virtual bool eventFilter(QObject *object, QEvent *event);
    virtual bool mousePressEvent(const QMouseEvent *e);
    virtual bool mouseMovedEvent( const QMouseEvent *e);
    virtual bool mouseReleasedEvent( const QMouseEvent *e);
    virtual bool keyPressEvent(const QKeyEvent *e);
    virtual bool keyReleaseEvent(const QKeyEvent *e);
private:
    bool m_isEnable;///< 是否生效
    bool m_isSpacePressed;///< 空格是否长按
    bool m_isCanUseSpaceKey;///< 标记是否可以使用空格
};

#endif // SAABSTRACTPLOTEDITOR_H
