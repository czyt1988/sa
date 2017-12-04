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
    //item最近点，如果有新的item，继承此类并重写此函数即可
    virtual int itemClosedPoint(const QwtPlotItem* item, const QPoint &pos,QPointF* itemPoint, double *dist);
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
        QwtPlotItem * item() const{return this->m_item;}
        void setItem(QwtPlotItem * item);
        bool isValid() const;
        QPointF getClosePoint() const;
        void setClosePoint(const QPointF& p);
        int index() const{return this->m_index;}
        void setIndex(int i){this->m_index = i;}
        double distace() const{return this->m_distace;}
        void setDistace(double d){this->m_distace = d;}
        void setInvalid();
    private:
        QwtPlotItem *m_item;
        int m_index;
        double m_distace;
        QPointF m_point;
    };
    closePoint m_closePoint;
private slots:
    //捕获鼠标移动的槽
    void mouseMove(const QPoint &pos);
public slots:
    void itemAttached(QwtPlotItem* plotItem,bool on);
private:
    QPen m_pen;
};

#endif // SAXYDATATRACKER_H
