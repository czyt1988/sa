#include "SASelectRegionEditor.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAAbstractRegionSelectEditor.h"
#include "czyQtPointAlgorithm.h"
#include <QHash>
#include "SALog.h"
struct curve_info
{
    QwtPlotCurve* curve;
    QVector<int> inRangIndexs;
    QVector<QPointF> inRangOriginData;
};

class SASelectRegionEditorPrivate
{
    SA_IMPL_PUBLIC(SASelectRegionEditor)
public:
    bool m_haveMutiAxis;//此标记此时含有多种坐标组合
    bool m_isStart;//是否开始
    QPainterPath m_selectRegion;//保存选择区域
    QPainterPath m_selectRegionOrigin;//保存选择的原始区域
    QHash<QPair<int,int>,QPainterPath> m_otherAxisSelectRegion;//其他坐标轴的选区
    QHash<QPair<int,int>,QPainterPath> m_otherAxisSelectRegionOrigin;//其他坐标轴的选区
    QList<curve_info> m_curListInfo;//保存选中的曲线信息
    QPoint m_firstPressedScreenPoint;//第一次按下的点
    QPoint m_tmpPoint;//记录临时点
    SASelectRegionEditorPrivate(SASelectRegionEditor* p)
        :q_ptr(p)
        ,m_haveMutiAxis(false)
        ,m_isStart(false)
    {

    }
};


SASelectRegionEditor::SASelectRegionEditor(SAChart2D *parent)
    :SAAbstractPlotEditor(parent)
    ,d_ptr(new SASelectRegionEditorPrivate(this))
{

    connect(parent,&SAChart2D::selectionRegionChanged
            ,this,&SASelectRegionEditor::onSelectionRegionChanged);
    connect(parent,&SAChart2D::currentSelectItemsChanged
            ,this,&SASelectRegionEditor::onCurrentSelectItemsChanged);
    updateRegionIndex();
    setEnabled(true);
}

SASelectRegionEditor::~SASelectRegionEditor()
{

}



int SASelectRegionEditor::rtti() const
{
    return RTTISelectRegionEditor;
}

const SAChart2D *SASelectRegionEditor::chart2D() const
{
    return qobject_cast<const SAChart2D*>(plot());
}

SAChart2D *SASelectRegionEditor::chart2D()
{
    return qobject_cast<SAChart2D*>(plot());
}

void SASelectRegionEditor::onSelectionRegionChanged(const QPainterPath &shape)
{
    Q_UNUSED(shape);
    updateRegionIndex();
}

void SASelectRegionEditor::onCurrentSelectItemsChanged(const QList<QwtPlotItem *> &items)
{
    Q_UNUSED(items);
    updateRegionIndex();
}
///
/// \brief 记录选中区域的索引和原始数值
///
void SASelectRegionEditor::updateRegionIndex()
{
    d_ptr->m_selectRegion = chart2D()->getSelectionRange();
    d_ptr->m_selectRegionOrigin = d_ptr->m_selectRegion;
    QList<QwtPlotCurve*> cursList = chart2D()->getCurrentSelectPlotCurveItems();
    const int count = cursList.size();
    //保存选中的索引号和数据
    d_ptr->m_curListInfo.clear();
    d_ptr->m_haveMutiAxis = false;
    for(int i=0;i<count;++i)
    {
        curve_info ci;
        d_ptr->m_curListInfo.append(ci);
        d_ptr->m_curListInfo[i].curve = cursList[i];
        if(chart2D()->isSelectionRangeAxisMatch(d_ptr->m_curListInfo[i].curve))
        {
            SAChart::getCurveInSelectRangDataAndIndex(d_ptr->m_selectRegion
                                               ,d_ptr->m_curListInfo[i].curve
                                               ,d_ptr->m_curListInfo[i].inRangIndexs
                                               ,d_ptr->m_curListInfo[i].inRangOriginData);
        }
        else
        {
            d_ptr->m_haveMutiAxis = true;
            //获取别的坐标系
            QPair<int,int> otherAxisPair = qMakePair(cursList[i]->xAxis(),cursList[i]->yAxis());
            if(!d_ptr->m_otherAxisSelectRegion.contains(otherAxisPair))
            {
                d_ptr->m_otherAxisSelectRegion[otherAxisPair]
                        = chart2D()->getSelectionRange(otherAxisPair.first,otherAxisPair.second);
                d_ptr->m_otherAxisSelectRegionOrigin[otherAxisPair] = d_ptr->m_otherAxisSelectRegion[otherAxisPair];
            }
            SAChart::getCurveInSelectRangDataAndIndex(d_ptr->m_otherAxisSelectRegion[otherAxisPair]
                                               ,d_ptr->m_curListInfo[i].curve
                                               ,d_ptr->m_curListInfo[i].inRangIndexs
                                               ,d_ptr->m_curListInfo[i].inRangOriginData);
        }
    }
}

bool SASelectRegionEditor::eventFilter(QObject *object, QEvent *event)
{
    QwtPlot *plot = qobject_cast<QwtPlot *>( parent() );
    if ( plot && (object == plot->canvas()) )
    {
        switch( event->type() )
        {
            case QEvent::MouseButtonPress:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                if(mouseEvent)
                {
                    return mousePressEvent( mouseEvent);
                }
                break;
            }
            case QEvent::MouseMove:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast< QMouseEvent* >( event );
                if(mouseEvent)
                {
                    return mouseMovedEvent( mouseEvent );
                }
                break;
            }
            case QEvent::MouseButtonRelease:
            {
                const QMouseEvent* mouseEvent =
                        dynamic_cast<QMouseEvent* >( event );
                if(mouseEvent)
                {
                    return mouseReleasedEvent( mouseEvent );
                }
                break;
            }
            case QEvent::KeyPress:
            {
                const QKeyEvent* keyEvent =
                    dynamic_cast<QKeyEvent* >( event );
                if(keyEvent)
                {
                    return keyPressEvent(keyEvent);
                }
                break;
            }
            case QEvent::KeyRelease:
            {
                const QKeyEvent* keyEvent =
                    dynamic_cast<QKeyEvent* >( event );
                if(keyEvent)
                {
                    return keyReleaseEvent(keyEvent);
                }
                break;
            }
            default:
                break;
        }
        return false;
    }
    return QObject::eventFilter( object, event );
}

bool SASelectRegionEditor::mousePressEvent(const QMouseEvent *e)
{
    d_ptr->m_isStart = true;
    d_ptr->m_firstPressedScreenPoint = e->pos();
    d_ptr->m_tmpPoint = e->pos();
    return false;
}

bool SASelectRegionEditor::mouseMovedEvent(const QMouseEvent *e)
{
    if(!d_ptr->m_isStart)
    {
        return true;
    }
    if(d_ptr->m_haveMutiAxis)
    {

    }
    else
    {
        QPoint screenPoint = e->pos();
        SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
        if(!editor)
        {
            return true;
        }
        chart2D()->setAutoReplot(false);
        QPointF currentPoint = editor->invTransform(screenPoint);
        QPointF originPoint = editor->invTransform(d_ptr->m_tmpPoint);
        QPointF offset = czy::calcOffset(currentPoint,originPoint);
        d_ptr->m_tmpPoint = screenPoint;
        qDebug() << "currentPoint:"<<currentPoint<<" originPoint"<<originPoint<<" offset:"<<offset;
        //选区进行移动
        d_ptr->m_selectRegion = d_ptr->m_selectRegionOrigin.translated(offset);
        chart2D()->setSelectionRange(d_ptr->m_selectRegion);
        //选中数据进行移动
//        const int count = d_ptr->m_curListInfo.size();
//        for(int i=0;i<count;++i)
//        {
//            const curve_info& curinfo = d_ptr->m_curListInfo[i];
//            const int indexCount = curinfo.inRangIndexs.size();
//            QVector<QPointF> xyData( curinfo.curve->dataSize() );
//            SAChart::getXYDatas(xyData,curinfo.curve);
//            for(int j = 0;j<indexCount;++j)
//            {
//                int index = curinfo.inRangIndexs[j];
//                const QPointF& p = curinfo.inRangOriginData[j];
//                xyData[index] = p + offset;
//            }
//            curinfo.curve->setSamples(xyData);
//        }
        QwtPlotCanvas *plotCanvas =
                qobject_cast<QwtPlotCanvas *>( plot()->canvas() );
        chart2D()->setAutoReplot(true);
        plotCanvas->setPaintAttribute( QwtPlotCanvas::ImmediatePaint, true );
        plot()->replot();
        plotCanvas->setPaintAttribute( QwtPlotCanvas::ImmediatePaint, false );
        return false;
    }
    return true;
}

bool SASelectRegionEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    if(!d_ptr->m_isStart)
    {
        return true;
    }
    d_ptr->m_isStart = false;
    return false;
}

bool SASelectRegionEditor::keyPressEvent(const QKeyEvent *e)
{
    return true;
}

bool SASelectRegionEditor::keyReleaseEvent(const QKeyEvent *e)
{
    return true;
}
