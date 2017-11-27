#include "SASelectRegionDataEditor.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "SAAbstractRegionSelectEditor.h"
#include "czyQtPointAlgorithm.h"
#include "SAFigureOptCommands.h"
#include <QHash>
#include "SALog.h"
struct curve_info
{
    QwtPlotCurve* curve;
    QVector<int> inRangIndexs;
    QVector<QPointF> inRangOriginData;
};

class SASelectRegionDataEditorPrivate
{
    SA_IMPL_PUBLIC(SASelectRegionDataEditor)
public:
    bool m_isStart;//是否开始
    QPainterPath m_selectRegionOrigin;//保存选择的原始区域
    QList<curve_info> m_curListInfo;//保存选中的曲线信息
    QPoint m_firstPressedScreenPoint;//第一次按下的点
    QPoint m_tmpPoint;//记录临时点
    SASelectRegionDataEditorPrivate(SASelectRegionDataEditor* p)
        :q_ptr(p)
    {

    }
};

SASelectRegionDataEditor::SASelectRegionDataEditor(SAChart2D *parent)
    :SAAbstractPlotEditor(parent)
    ,d_ptr(new SASelectRegionDataEditorPrivate(this))
{
    connect(parent,&SAChart2D::currentSelectItemsChanged
            ,this,&SASelectRegionDataEditor::onCurrentSelectItemsChanged);
    updateRegionIndex();
    setEnabled(true);
}

SASelectRegionDataEditor::~SASelectRegionDataEditor()
{

}

int SASelectRegionDataEditor::rtti() const
{
    return RTTISelectDataRegionEditor;
}

const SAChart2D *SASelectRegionDataEditor::chart2D() const
{
    return qobject_cast<const SAChart2D*>(plot());
}

SAChart2D *SASelectRegionDataEditor::chart2D()
{
    return qobject_cast<SAChart2D*>(plot());
}



void SASelectRegionDataEditor::onCurrentSelectItemsChanged(const QList<QwtPlotItem *> &items)
{
    Q_UNUSED(items);
    updateRegionIndex();
}

void SASelectRegionDataEditor::updateRegionIndex()
{
    d_ptr->m_selectRegionOrigin = chart2D()->getSelectionRange();
    QList<QwtPlotCurve*> cursList = chart2D()->getCurrentSelectPlotCurveItems();
    const int count = cursList.size();
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return;
    }
    //保存选中的索引号和数据
    d_ptr->m_curListInfo.clear();
    for(int i=0;i<count;++i)
    {
        curve_info ci;
        d_ptr->m_curListInfo.append(ci);
        d_ptr->m_curListInfo[i].curve = cursList[i];
        SAChart::getCurveInSelectRangDataAndIndex(d_ptr->m_selectRegionOrigin
                                           ,d_ptr->m_curListInfo[i].curve
                                           ,d_ptr->m_curListInfo[i].inRangIndexs
                                           ,d_ptr->m_curListInfo[i].inRangOriginData
                                                  ,editor->getXAxis()
                                                  ,editor->getYAxis());
    }
}

bool SASelectRegionDataEditor::mousePressEvent(const QMouseEvent *e)
{
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    plot()->setCursor(QCursor(Qt::SizeAllCursor));
    d_ptr->m_isStart = true;
    d_ptr->m_firstPressedScreenPoint = e->pos();
    d_ptr->m_tmpPoint = e->pos();
    d_ptr->m_selectRegionOrigin = chart2D()->getSelectionRange();
    return true;
}

bool SASelectRegionDataEditor::mouseMovedEvent(const QMouseEvent *e)
{
    if(!d_ptr->m_isStart)
    {
        return false;
    }
    QPoint screenPoint = e->pos();
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    chart2D()->setAutoReplot(false);
    QPointF currentPoint = editor->invTransform(screenPoint);
    QPointF originPoint = editor->invTransform(d_ptr->m_tmpPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);

    //选区进行移动
    QPainterPath p = chart2D()->getSelectionRange();
    p.translate(offset);
    chart2D()->setSelectionRange(p);
    //数据进行移动
    for(int i=0;i<d_ptr->m_curListInfo.size();++i)
    {
        const curve_info& info = d_ptr->m_curListInfo[i];
        int xaxis = info.curve->xAxis();
        int yaxis = info.curve->yAxis();
#if 0
        double cx = plot()->canvasMap(xaxis).invTransform(screenPoint.x());
        double cy = plot()->canvasMap(yaxis).invTransform(screenPoint.y());
        double ox = plot()->canvasMap(xaxis).invTransform(d_ptr->m_tmpPoint.x());
        double oy = plot()->canvasMap(yaxis).invTransform(d_ptr->m_tmpPoint.y());
#else
        double cx = plot()->invTransform(xaxis,screenPoint.x());
        double cy = plot()->invTransform(yaxis,screenPoint.y());
        double ox = plot()->invTransform(xaxis,d_ptr->m_tmpPoint.x());
        double oy = plot()->invTransform(yaxis,d_ptr->m_tmpPoint.y());
#endif
        offset.rx() = cx-ox;
        offset.ry() = cy-oy;
        QVector<QPointF> xyData;
        SAChart::getXYDatas(xyData,info.curve);
        for(int j=0;j<info.inRangIndexs.size();++j)
        {
            czy::pointOffseted(xyData[info.inRangIndexs[j]],offset.x(),offset.y());
        }
        info.curve->setSamples(xyData);
    }
    d_ptr->m_tmpPoint = screenPoint;
    chart2D()->setAutoReplot(true);
    plot()->replot();
    return true;
}

bool SASelectRegionDataEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    if(!d_ptr->m_isStart)
    {
        plot()->unsetCursor();
        return false;
    }
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        plot()->unsetCursor();
        return false;
    }
    plot()->unsetCursor();
    d_ptr->m_isStart = false;
    chart2D()->setAutoReplot(false);
    QPoint screenPoint = e->pos();
    QPointF currentPoint = editor->invTransform(screenPoint);
    QPointF originPoint = editor->invTransform(d_ptr->m_firstPressedScreenPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);
    //
    SAFigureOptCommand *removeDataAndRegion = new SAFigureOptCommand(chart2D(),tr("move region datas")); // 创建一个命令集
    //选区进行移动
    QPainterPath p = d_ptr->m_selectRegionOrigin.translated(offset);
    SAFigureChartSelectionRegionAddCommand* regionMove = new SAFigureChartSelectionRegionAddCommand(chart2D()
                                                                                                    ,d_ptr->m_selectRegionOrigin
                                                                                                    ,p
                                                                                                    ,tr("move region")
                                                                                                    ,removeDataAndRegion);
    //数据进行移动
    QList<SAFigureMoveCurveDataInIndexsCommandCurveInfo > cmdInfoList;
    SAFigureMoveCurveDataInIndexsCommandCurveInfo cmdInfo;
    for(int i=0;i<d_ptr->m_curListInfo.size();++i)
    {
        curve_info& info = d_ptr->m_curListInfo[i];
        int xaxis = info.curve->xAxis();
        int yaxis = info.curve->yAxis();
#if 0
        double cx = plot()->canvasMap(xaxis).invTransform(screenPoint.x());
        double cy = plot()->canvasMap(yaxis).invTransform(screenPoint.y());
        double ox = plot()->canvasMap(xaxis).invTransform(d_ptr->m_firstPressedScreenPoint.x());
        double oy = plot()->canvasMap(yaxis).invTransform(d_ptr->m_firstPressedScreenPoint.y());
#else
        double cx = plot()->invTransform(xaxis,screenPoint.x());
        double cy = plot()->invTransform(yaxis,screenPoint.y());
        double ox = plot()->invTransform(xaxis,d_ptr->m_firstPressedScreenPoint.x());
        double oy = plot()->invTransform(yaxis,d_ptr->m_firstPressedScreenPoint.y());
#endif
        QPointF offset(cx-ox,cy-oy);

        cmdInfo.curve = info.curve;
        cmdInfo.inRangIndexs = info.inRangIndexs;
        cmdInfo.inRangOriginData = info.inRangOriginData;
        QVector<QPointF> newData = cmdInfo.inRangOriginData;
        std::for_each(newData.begin(),newData.end(),czy::OffsetPoints<QPointF>(offset));
        cmdInfo.inRangNewData = newData;
        cmdInfoList.append(cmdInfo);
        info.inRangOriginData = newData;//保存的原始值进行更新
    }

    SAFigureMoveCurveDataInIndexsCommand* regionDataMove = new SAFigureMoveCurveDataInIndexsCommand(chart2D()
                                                                                                    ,cmdInfoList
                                                                                                    ,tr("move region datas")
                                                                                                    ,removeDataAndRegion);


    chart2D()->appendCommand(removeDataAndRegion);
    chart2D()->setAutoReplot(true);
    plot()->replot();
    return true;
}

bool SASelectRegionDataEditor::keyPressEvent(const QKeyEvent *e)
{
    return false;
}

bool SASelectRegionDataEditor::keyReleaseEvent(const QKeyEvent *e)
{
    return false;
}
