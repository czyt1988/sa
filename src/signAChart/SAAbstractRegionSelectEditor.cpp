#include "SAAbstractRegionSelectEditor.h"


SAAbstractRegionSelectEditor::SAAbstractRegionSelectEditor(QwtPlot *parent)
    :SAAbstractPlotEditor(parent)
    ,m_selectionMode(AdditionalSelection)//SingleSelection
    ,m_xAxis(QwtPlot::xBottom)
    ,m_yAxis(QwtPlot::yLeft)
{

}

SAAbstractRegionSelectEditor::~SAAbstractRegionSelectEditor()
{

}

SAAbstractRegionSelectEditor::SelectionMode SAAbstractRegionSelectEditor::getSelectionMode() const
{
    return m_selectionMode;
}

void SAAbstractRegionSelectEditor::setSelectionMode(const SelectionMode &selectionMode)
{
    m_selectionMode = selectionMode;
}

///
/// \brief 获取绑定的x轴
/// \return
///
int SAAbstractRegionSelectEditor::getXAxis() const
{
    return m_xAxis;
}

///
/// \brief 获取绑定的y轴
/// \return
///
int SAAbstractRegionSelectEditor::getYAxis() const
{
    return m_yAxis;
}
///
/// \brief 设置关联的坐标轴
/// \note 默认是xbottom，yLeft
/// \param xAxis
/// \param yAxis
///
void SAAbstractRegionSelectEditor::setAxis(int xAxis, int yAxis)
{
    m_xAxis = xAxis;
    m_yAxis = yAxis;
}

///
/// \brief 屏幕坐标转换为数据坐标
/// \param pos
/// \return
///
QPointF SAAbstractRegionSelectEditor::invTransform(const QPointF &pos ) const
{
    QwtScaleMap xMap = plot()->canvasMap( getXAxis() );
    QwtScaleMap yMap = plot()->canvasMap( getYAxis() );

    return QPointF(
        xMap.invTransform( pos.x() ),
        yMap.invTransform( pos.y() )
                );
}
///
/// \brief 数据坐标转换为屏幕坐标
/// \param pos
/// \return
///
QPointF SAAbstractRegionSelectEditor::transform(const QPointF &pos) const
{
    QwtScaleMap xMap = plot()->canvasMap( getXAxis() );
    QwtScaleMap yMap = plot()->canvasMap( getYAxis() );
    return QwtScaleMap::transform(xMap,yMap,pos);
}

///
/// \brief 把当前区域转换为其它轴系
/// \param axisX
/// \param axisY
/// \return
///
QPainterPath SAAbstractRegionSelectEditor::transformToOtherAxis(int axisX, int axisY)
{
    QPainterPath shape = getSelectRegion();
    QwtScaleMap xMap = plot()->canvasMap( axisX );
    QwtScaleMap yMap = plot()->canvasMap( axisY );
    const int eleCount = shape.elementCount();
    for(int i=0;i<eleCount;++i)
    {
        const QPainterPath::Element &el = shape.elementAt( i );
        QPointF tmp = transform(QPointF(el.x,el.y));
        tmp = QwtScaleMap::invTransform(xMap,yMap,tmp);
        shape.setElementPositionAt( i, tmp.x(), tmp.y() );
    }
    return shape;
}
