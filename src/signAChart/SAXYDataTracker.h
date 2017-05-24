#ifndef SAXYDATATRACKER_H
#define SAXYDATATRACKER_H
#include "SAChartGlobals.h"
#include <qwt_plot_picker.h>
class QwtPlotCurve;
class QwtPlotItem;
class SA_CHART_EXPORT SAXYDataTracker: public QwtPlotPicker
{
public:
    SAXYDataTracker(QWidget * canvas);
protected:
    virtual QwtText trackerTextF(const QPointF & pos) const;
    virtual QRect trackerRect(const QFont & font) const;
    virtual void drawRubberBand (QPainter *painter) const;
    void calcClosestPoint(const QPoint& pos);
    static double distancePower(const QPointF& p1,const QPointF& p2);
private:
    ///
    /// \brief 记录最近点的信息
    ///
    class closePoint
    {
    public:
        closePoint();
        QwtPlotCurve * curve() const{return this->m_curve;}
        void setCurve(QwtPlotCurve * cur);
        bool isValid() const;
        QPointF getClosePoint() const;
        int index() const{return this->m_index;}
        void setIndex(int i){this->m_index = i;}
        double distace() const{return this->m_distace;}
        void setDistace(double d){this->m_distace = d;}
        void setInvalid();
    private:
        QwtPlotCurve *m_curve;
        int m_index;
        double m_distace;
    };
    closePoint m_closePoint;
private slots:
    //捕获鼠标移动的槽
    void mouseMove(const QPoint &pos);
public slots:
    void itemAttached(QwtPlotItem* plotItem,bool on);
private:
    //根据刻度坐标查找水平距离最近的曲线点
    QLineF curveLineAtX( const QwtPlotCurve *curve, double x ) const;
    int curveLineIndexAtX( const QwtPlotCurve *curve, double x ) const;
    QPointF closePointX(const QwtPlotCurve *curve, const QPointF & pos) const;
    QPointF closePointX(const QPointF & pos) const;
};

#endif // SAXYDATATRACKER_H
