#include "SAXYDataTracker.h"
#include <qwt_plot_item.h>
#include <qwt_plot_curve.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_dict.h>
#include <qwt_plot.h>
#include <qwt_painter.h>
#include <algorithm>
#include <numeric>
#include <math.h>
SAXYDataTracker::SAXYDataTracker(QWidget* canvas) :
    QwtPlotPicker( canvas )
{
    setTrackerMode( QwtPlotPicker::ActiveOnly );
    setRubberBand( UserRubberBand  );
    setStateMachine( new QwtPickerTrackerMachine() );
    connect (this,&QwtPicker::moved,this,&SAXYDataTracker::mouseMove);
    if(plot ())
        connect (plot(),&QwtPlot::itemAttached,this,&SAXYDataTracker::itemAttached );
}

QwtText SAXYDataTracker::trackerTextF(const QPointF& pos) const
{
    Q_UNUSED(pos);
    QwtText trackerText;
    if(!m_closePoint.isValid ())
        return trackerText;
    trackerText.setColor( Qt::black );
    QColor lineColor = m_closePoint.curve()->pen ().color ();
    QColor bkColor(lineColor);
    bkColor.setAlpha (30);
    trackerText.setBorderPen( m_closePoint.curve()->pen () );
    trackerText.setBackgroundBrush( bkColor );
    QPointF point = m_closePoint.getClosePoint ();
    QString info = QString("<font color=\"%1\">y:%2</font><br>")
                   .arg(lineColor.name ()).arg(point.y ())
                   +
                   QString("<font color=\"%1\">x:%2</font>")
                                      .arg(lineColor.name ()).arg(point.x ());
    trackerText.setText( info );
    trackerText.setBorderRadius (5);
    return trackerText;
}

QRect SAXYDataTracker::trackerRect(const QFont& font) const
{
    QRect r = QwtPlotPicker::trackerRect( font );
    r += QMargins(5,5,5,5);
    return r;
}

void SAXYDataTracker::drawRubberBand(QPainter* painter) const
{
    if ( !isActive() || rubberBand() == NoRubberBand ||
        rubberBandPen().style() == Qt::NoPen )
    {
        return;
    }
    if(!m_closePoint.isValid ())
        return;
    //获取鼠标的客户坐标位置
    const QPoint pos = trackerPosition ();
    if(pos.isNull ())
        return;
    const QPointF closePoint = m_closePoint.getClosePoint ();
    const QPoint cvp = transform (closePoint);
    QwtPainter::drawLine (painter,pos,cvp);
    QRect r(0,0,10,10);
    r.moveCenter (cvp);
    QwtPainter::drawRect (painter,r);
}
///
/// \brief 遍历所有数据找到最近点
/// \param pos 绘图坐标
/// \note 此算法是遍历所有数据，在数据量大时请谨慎
///
void SAXYDataTracker::calcClosestPoint(const QPoint& pos)
{
    const QwtPlotItemList curveItems =
        plot()->itemList( QwtPlotItem::Rtti_PlotCurve );
    if(curveItems.size () <= 0)
        return;
    //把屏幕坐标转换为图形的数值坐标
    //QPointF mousePoint = invTransform(pos);
    //记录最短的距离，默认初始化为double的最大值
    double distance = std::numeric_limits<double>::max ();
    //记录前一次最近点的曲线指针
    QwtPlotCurve * oldCur = m_closePoint.curve ();
    for ( int i = 0; i < curveItems.size(); ++i )
    {
        QwtPlotCurve * cur = static_cast<QwtPlotCurve *>( curveItems[i] );
        double dp;
        int index = cur->closestPoint (pos,&dp);
        if(-1 == index)
            continue;
        //QPointF p = cur->sample (index);
        if(dp < distance)
        {
            m_closePoint.setDistace(dp);//实际距离需要开方
            m_closePoint.setIndex(index);
            m_closePoint.setCurve(cur);
            distance = dp;
        }
    }
    //说明最近点的曲线更换了，标记线的颜色换为当前曲线的颜色
    if(m_closePoint.isValid () && oldCur!=m_closePoint.curve ())
    {
        QPen p(m_closePoint.curve ()->pen ());
        p.setWidth (1);
        setRubberBandPen (p);
    }
}

double SAXYDataTracker::distancePower(const QPointF& p1, const QPointF& p2)
{
    return pow(p1.x ()-p2.x (),2.0)+pow(p1.y ()-p2.y (),2.0);
}

void SAXYDataTracker::mouseMove(const QPoint& pos)
{
    calcClosestPoint(pos);
}

void SAXYDataTracker::itemAttached(QwtPlotItem* plotItem, bool on)
{
    if(!on)
    {
        if(QwtPlotItem::Rtti_PlotCurve == plotItem->rtti ())
        {
            QwtPlotCurve * cur = static_cast<QwtPlotCurve *>( plotItem);
            if(cur == m_closePoint.curve())
                m_closePoint.setInvalid ();
        }
    }
}

///
/// 获取x点对应的curve的间隔线此时x正好位于这两点之间
/// \param curve
/// \param x
/// \return
///
QLineF SAXYDataTracker::curveLineAtX(const QwtPlotCurve*curve, double x) const
{
    QLineF line;
    if ( curve->dataSize() <= 2 )
        return line;
    int index = curveLineIndexAtX(curve,x);
    if ( index == -1 &&
        x == curve->sample( curve->dataSize() - 1 ).x() )
    {
        // the last sample is excluded from qwtUpperSampleIndex
        index = curve->dataSize() - 1;
    }

    if ( index > 0 )
    {
        line.setP1( curve->sample( index - 1 ) );
        line.setP2( curve->sample( index ) );
    }
    return line;
}
///
/// \brief 获取x点对应的curve的上边界索引
/// \param curve 曲线指针
/// \param x 搜索数值
/// \return 索引，如果没有，返回-1
/// \note 曲线需保证x值是有序曲线
///
int SAXYDataTracker::curveLineIndexAtX(const QwtPlotCurve* curve, double x) const
{
    int index=-1;

    if ( curve->dataSize() >= 2 )
    {
        const QRectF br = curve->boundingRect();
        if ( br.isValid() && x >= br.left() && x <= br.right() )
        {
            index = qwtUpperSampleIndex<QPointF>(
                            *curve->data(), x
                            ,[](const double x, const QPointF &pos)->bool{
                                return ( x < pos.x() );
                            } );
        }
    }
    return index;
}
///
/// \brief 找到对应curve下pos位置竖直距离最近的点
/// \param curve 曲线指针
/// \param pos 绘图坐标
/// \return 返回最近的曲线上点，若没有，返回QPointF()
///
QPointF SAXYDataTracker::closePointX(const QwtPlotCurve* curve, const QPointF& pos) const
{
    const QLineF line = curveLineAtX(curve,pos.x ());
    if ( line.isNull() )
        return QPointF();
    //如果pos位于靠近开始点，那么pos.x() - line.p1().x() ) / line.dx()为小于0.5返回开始点，反之亦然
    return  line.pointAt(
        ( pos.x() - line.p1().x() ) / line.dx() );
}
///
/// \brief 找到图线中pos位置竖直距离最近的点
/// \param pos 绘图坐标
/// \return 返回最近的曲线上点，若没有，返回QPointF()
///
QPointF SAXYDataTracker::closePointX(const QPointF& pos) const
{
    const QwtPlotItemList curveItems =
        plot()->itemList( QwtPlotItem::Rtti_PlotCurve );
    QPointF closePoint = QPointF();
    double lastLength = std::numeric_limits<double>::max ();
    QLineF lp;
    lp.setP1 (pos);
    double lpLength = 0;
    for ( int i = 0; i < curveItems.size(); i++ )
    {
        const QwtPlotCurve * cur = static_cast<const QwtPlotCurve *>( curveItems[i] );
        QPointF p = closePointX(cur,pos);
        if(p.isNull ())
            continue;
        lp.setP2 (p);
        lpLength=lp.length ();
        if(lpLength<lastLength)
        {
            closePoint = p;
            lastLength = lpLength;
        }
    }
    return closePoint;
}

SAXYDataTracker::closePoint::closePoint()
  :m_curve(nullptr)
  ,m_index(-1)
  ,m_distace(std::numeric_limits<double>::max ())
{

}

void SAXYDataTracker::closePoint::setCurve(QwtPlotCurve* cur)
{
    this->m_curve = cur;
}

bool SAXYDataTracker::closePoint::isValid() const
{
    return ((this->curve() != nullptr) && (this->index() >= 0));
}

QPointF SAXYDataTracker::closePoint::getClosePoint() const
{
    if(isValid ())
        return this->curve()->sample (this->index());
    return QPointF();
}

void SAXYDataTracker::closePoint::setInvalid()
{
    setCurve (nullptr);
    setIndex (-1);
    setDistace (std::numeric_limits<double>::max ());
}
