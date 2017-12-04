#include "SAXYDataTracker.h"
#include <qwt_plot_item.h>
#include <qwt_plot_curve.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_dict.h>
#include <qwt_plot.h>
#include <qwt_painter.h>
#include <qwt_plot_barchart.h>
#include <algorithm>
#include <numeric>
#include <math.h>
#include "SAChart.h"

SAXYDataTracker::SAXYDataTracker(QWidget* canvas) :
    QwtPlotPicker( canvas )
{
    setTrackerMode( QwtPlotPicker::ActiveOnly );
    setRubberBand( UserRubberBand  );
    setStateMachine( new QwtPickerTrackerMachine() );
    connect (this,&QwtPicker::moved,this,&SAXYDataTracker::mouseMove);
    m_pen.setWidth(1);
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
    QColor lineColor = SAChart::getItemColor(m_closePoint.item());
    QColor bkColor(lineColor);
    bkColor.setAlpha (30);
    //trackerText.setBorderPen( m_closePoint.item()->pen () );
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
    painter->setPen(m_pen);
    const QPointF closePoint = m_closePoint.getClosePoint ();
    const QPoint cvp = transform (closePoint);
    QwtPainter::drawLine (painter,pos,cvp);
    QRect r(0,0,10,10);
    r.moveCenter (cvp);
    QwtPainter::drawRect (painter,r);
}

int SAXYDataTracker::itemClosedPoint(const QwtPlotItem *item, const QPoint &pos, QPointF *itemPoint, double *dist)
{
    int index = -1;
    QPointF point;
    switch(item->rtti())
    {
    case QwtPlotItem::Rtti_PlotCurve:
    {
        const QwtPlotCurve * cur = static_cast<const QwtPlotCurve*>(item);
        index = cur->closestPoint (pos,dist);
        if(-1 != index)
        {
            point = cur->sample(index);
        }
        break;
    }
    case QwtPlotItem::Rtti_PlotBarChart:
    {
        const QwtPlotBarChart * bar = static_cast<const QwtPlotBarChart*>(item);
        index = SAChart::closestPoint(bar,pos,dist);
        if(-1 != index)
        {
            point = bar->sample(index);
        }
        break;
    }
    default:
        break;
    }
    if(itemPoint)
    {
        *itemPoint = point;
    }
    return index;
}
///
/// \brief 遍历所有数据找到最近点
/// \param pos 绘图坐标
/// \note 此算法是遍历所有数据，在数据量大时请谨慎
///
void SAXYDataTracker::calcClosestPoint(const QPoint& pos)
{
    const QwtPlotItemList curveItems = plot()->itemList();
    if(curveItems.size () <= 0)
        return;
    //把屏幕坐标转换为图形的数值坐标
    //QPointF mousePoint = invTransform(pos);
    //记录最短的距离，默认初始化为double的最大值
    double distance = std::numeric_limits<double>::max ();
    //记录前一次最近点的曲线指针

    QPointF point;
    QwtPlotItem * oldItem = m_closePoint.item ();
    for ( int i = 0; i < curveItems.size(); ++i )
    {
        double dp;
        int index = -1;
        QwtPlotItem * item = curveItems[i];
        index = itemClosedPoint(item,pos,&point,&dp);
        if(-1 == index)
            continue;
        //QPointF p = cur->sample (index);
        if(dp < distance)
        {
            m_closePoint.setDistace(dp);//实际距离需要开方
            m_closePoint.setIndex(index);
            m_closePoint.setItem(item);
            m_closePoint.setClosePoint(point);
            distance = dp;
        }
    }
    //说明最近点的曲线更换了，标记线的颜色换为当前曲线的颜色
    if(m_closePoint.isValid () && oldItem!=m_closePoint.item ())
    {
        m_pen.setColor(SAChart::getItemColor(m_closePoint.item()));
        setRubberBandPen (m_pen);
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
        if(plotItem == m_closePoint.item())
            m_closePoint.setInvalid ();
    }
}

SAXYDataTracker::closePoint::closePoint()
  :m_item(nullptr)
  ,m_index(-1)
  ,m_distace(std::numeric_limits<double>::max ())
{

}

void SAXYDataTracker::closePoint::setItem(QwtPlotItem *item)
{
    this->m_item = item;
}

bool SAXYDataTracker::closePoint::isValid() const
{
    return ((this->item() != nullptr) && (this->index() >= 0));
}

QPointF SAXYDataTracker::closePoint::getClosePoint() const
{
    if(isValid ())
        return m_point;
    return QPointF();
}

void SAXYDataTracker::closePoint::setClosePoint(const QPointF &p)
{
    m_point = p;
}

void SAXYDataTracker::closePoint::setInvalid()
{
    setItem (nullptr);
    setIndex (-1);
    setDistace (std::numeric_limits<double>::max ());
    m_point = QPointF();
}
