#include "SASelectRegionDataEditor.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "qwt_plot_barchart.h"
#include "SAAbstractRegionSelectEditor.h"
#include "czyQtPointAlgorithm.h"
#include "SAFigureOptCommands.h"
#include <QHash>
#include "SALog.h"

class chart2d_base_info
{
public:
    enum RTTI
    {
        RTTI_PointInfo
    };

    chart2d_base_info():plotItem(nullptr)
    {

    }
    virtual ~chart2d_base_info(){}
    virtual int rtti() const = 0;

    const QVector<int>& indexs() const{return inRangIndexs;}
    QVector<int>& indexs() {return inRangIndexs;}
    void setIndexs(const QVector<int>& v){inRangIndexs = v;}

    QwtPlotItem* item(){return plotItem;}
    void setItem(QwtPlotItem* v){plotItem  =v;}
protected:
    QVector<int> inRangIndexs;
    QwtPlotItem* plotItem;
};

class chart2d_points_info : public chart2d_base_info
{
public:
    chart2d_points_info():chart2d_base_info(){

    }

    int rtti() const{return RTTI_PointInfo;}
    void setPoints(const QVector<QPointF>& p){
        inRangOriginData = p;
    }
    const QVector<QPointF>& points() const{
        return inRangOriginData;
    }
    QVector<QPointF>& points(){
        return inRangOriginData;
    }

protected:
    QVector<QPointF> inRangOriginData;
};

class SASelectRegionDataEditorPrivate
{
    SA_IMPL_PUBLIC(SASelectRegionDataEditor)
    QList<chart2d_base_info*> m_curListInfo;//保存选中的曲线信息
public:
    bool m_isStart;//是否开始
    bool m_isStartKeyAction;///< 是否按键激活
    QPainterPath m_selectRegionOrigin;//保存选择的原始区域
    QPoint m_firstPressedScreenPoint;//第一次按下的点
    QPoint m_tmpPoint;//记录临时点
    bool m_isStartMouseAction;///<
    SASelectRegionDataEditorPrivate(SASelectRegionDataEditor* p)
        :q_ptr(p)
        ,m_isStart(false)
        ,m_isStartKeyAction(false)
        ,m_isStartMouseAction(false)
    {

    }
    ~SASelectRegionDataEditorPrivate()
    {
        clearInfo();
    }
    chart2d_base_info* info(int index)
    {
        return m_curListInfo[index];
    }
    int infoCount() const
    {
        return m_curListInfo.size();
    }
    void setInfo(chart2d_base_info* p,int i)
    {
        delete m_curListInfo[i];
        m_curListInfo[i] = p;
    }
    void appendInfo(chart2d_base_info* p)
    {
        m_curListInfo.append(p);
    }
    void clearInfo()
    {
        for(int i=0;i<m_curListInfo.size();++i)
        {
            chart2d_base_info* p = m_curListInfo[i];
            if(p)
            {
                delete p;
            }
        }
        m_curListInfo.clear();
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
///
/// \brief 记录所有可以移动的数据
///
void SASelectRegionDataEditor::updateRegionIndex()
{
    SAChart2D* chart = chart2D();
    d_ptr->m_selectRegionOrigin = chart->getSelectionRange();
    QList<QwtPlotItem*> plotItems = chart->getCurrentSelectItems();
    //QList<QwtPlotCurve*> cursList = chart2D()->getCurrentSelectPlotCurveItems();

    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return;
    }
    const int count = plotItems.size();
    //保存选中的索引号和数据
    d_ptr->clearInfo();
    for(int i=0;i<count;++i)
    {
        QwtPlotItem* item = plotItems[i];
        switch(item->rtti())
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {
            chart2d_points_info* ci = new chart2d_points_info();
            d_ptr->appendInfo(ci);
            ci->setItem(item);
            QwtPlotCurve* cur = static_cast<QwtPlotCurve*>(item);
            if(editor->getXAxis() != cur->xAxis() || editor->getYAxis() != cur->yAxis())
            {
                QPainterPath otPath = SAChart::transformPath(chart,d_ptr->m_selectRegionOrigin
                                                           ,editor->getXAxis(),editor->getYAxis()
                                                           ,cur->xAxis(),cur->yAxis());
                SAChart::getSeriesInSelectRangDataAndIndex(otPath,cur
                                                           ,ci->indexs()
                                                           ,ci->points());
            }
            else
            {
                SAChart::getSeriesInSelectRangDataAndIndex(d_ptr->m_selectRegionOrigin,cur
                                                           ,ci->indexs()
                                                           ,ci->points());
            }
            break;
        }
        case QwtPlotItem::Rtti_PlotBarChart:
        {
            chart2d_points_info* ci = new chart2d_points_info();
            d_ptr->appendInfo(ci);
            ci->setItem(item);
            QwtPlotBarChart* bar = static_cast<QwtPlotBarChart*>(item);
            if(editor->getXAxis() != bar->xAxis() || editor->getYAxis() != bar->yAxis())
            {
                QPainterPath otPath = SAChart::transformPath(chart,d_ptr->m_selectRegionOrigin
                                                           ,editor->getXAxis(),editor->getYAxis()
                                                           ,bar->xAxis(),bar->yAxis());
                SAChart::getSeriesInSelectRangDataAndIndex(otPath,bar
                                                           ,ci->indexs()
                                                           ,ci->points());
            }
            else
            {
                SAChart::getSeriesInSelectRangDataAndIndex(d_ptr->m_selectRegionOrigin,bar
                                                           ,ci->indexs()
                                                           ,ci->points());
            }
            break;
        }
        case QwtPlotItem::Rtti_PlotSpectroCurve:
        case QwtPlotItem::Rtti_PlotIntervalCurve:
        case QwtPlotItem::Rtti_PlotHistogram:
        case QwtPlotItem::Rtti_PlotSpectrogram:
        case QwtPlotItem::Rtti_PlotTradingCurve:
        case QwtPlotItem::Rtti_PlotMultiBarChart:
        default:
            break;
        }



    }
}
///
/// \brief SASelectRegionDataEditor::offsetRegionAndData
/// \param offset
///
void SASelectRegionDataEditor::offsetData(const QPointF &offset)
{
    bool isAutoReplot = chart2D()->autoReplot();
    chart2D()->setAutoReplot(false);
    int count = d_ptr->infoCount();
    for(int i=0;i<count;++i)
    {
        chart2d_base_info* info = d_ptr->info(i);
        QwtPlotItem* item = info->item();
        switch(item->rtti())
        {
        case QwtPlotItem::Rtti_PlotCurve:
        {
            QVector<QPointF> xyData;
            QwtPlotCurve*cur = static_cast<QwtPlotCurve*>(item);
            SAChart::getXYDatas(xyData,cur);
            std::for_each(info->indexs().begin(),info->indexs().end()
                          ,[&xyData,offset](int i){
                if(i >= 0 && i<xyData.size())
                {
                    xyData[i]+=offset;
                }
            });
            cur->setSamples(xyData);
            break;
        }
        case QwtPlotItem::Rtti_PlotBarChart:
        {
            QVector<QPointF> xyData;
            QwtPlotBarChart*bar = static_cast<QwtPlotBarChart*>(item);
            SAChart::getXYDatas(xyData,bar);
            std::for_each(info->indexs().begin(),info->indexs().end()
                          ,[&xyData,offset](int i){
                if(i >= 0 && i<xyData.size())
                {
                    xyData[i]+=offset;
                }
            });
            bar->setSamples(xyData);
            break;
        }
        case QwtPlotItem::Rtti_PlotSpectroCurve:
        case QwtPlotItem::Rtti_PlotIntervalCurve:
        case QwtPlotItem::Rtti_PlotHistogram:
        case QwtPlotItem::Rtti_PlotSpectrogram:
        case QwtPlotItem::Rtti_PlotTradingCurve:
        case QwtPlotItem::Rtti_PlotMultiBarChart:
        default:
            break;
        }
    }
    chart2D()->setAutoReplot(isAutoReplot);
}

void SASelectRegionDataEditor::offsetRegion(const QPointF &offset)
{
    bool isAutoReplot = chart2D()->autoReplot();
    chart2D()->setAutoReplot(false);
    //选区进行移动
    QPainterPath p = chart2D()->getSelectionRange();
    p.translate(offset);
    chart2D()->setSelectionRange(p);
    chart2D()->setAutoReplot(isAutoReplot);
}

bool SASelectRegionDataEditor::completeEdit(const QPoint& screenPoint)
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
    d_ptr->m_isStartKeyAction = false;
    bool isAutoReplot = chart2D()->autoReplot();
    chart2D()->setAutoReplot(false);
    //选区进行移动
    QPointF currentPoint = editor->invTransform(screenPoint);
    QPointF originPoint = editor->invTransform(d_ptr->m_firstPressedScreenPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);
    SAFigureOptCommand *removeDataAndRegion = new SAFigureOptCommand(chart2D(),tr("move region datas")); // 创建一个命令集
    QPainterPath p = d_ptr->m_selectRegionOrigin.translated(offset);
    new SAFigureChartSelectionRegionAddCommand(chart2D()
                                                ,d_ptr->m_selectRegionOrigin
                                                ,p
                                                ,tr("move region")
                                                ,removeDataAndRegion);
    //
    //数据进行移动
    int count = d_ptr->infoCount();
    for(int i=0;i<count;++i)
    {
        chart2d_base_info* info = d_ptr->info(i);
        QwtPlotItem* item = info->item();
        int xaxis = item->xAxis();
        int yaxis = item->yAxis();
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
        QPointF plotOffset(cx-ox,cy-oy);
        switch(info->rtti())
        {
        case chart2d_base_info::RTTI_PointInfo:
        {
            chart2d_points_info* pointInfo = static_cast<chart2d_points_info*>(info);
            SAFigureMoveSerisePointDataInfo* reunInfo = new SAFigureMoveSerisePointDataInfo(item);
            reunInfo->setIndexs(pointInfo->indexs());
            reunInfo->setOriginPoints(pointInfo->points());
            QVector<QPointF> newData = pointInfo->points();
            std::for_each(newData.begin(),newData.end(),[plotOffset](QPointF& point){point+=plotOffset;});
            reunInfo->setNewPoints(newData);
            new SAFigureMovePointDataInIndexsCommand(chart2D(),reunInfo,"move datas",removeDataAndRegion);
            //把数据更新
            pointInfo->setPoints(newData);
            break;
        }
        default:
            break;
        }
    }
    chart2D()->appendCommand(removeDataAndRegion);
    chart2D()->setAutoReplot(isAutoReplot);
    return true;
}

void SASelectRegionDataEditor::startEdit(const QPoint &screenPoint)
{
    plot()->setCursor(QCursor(Qt::SizeAllCursor));
    d_ptr->m_isStart = true;
    d_ptr->m_firstPressedScreenPoint = screenPoint;
    d_ptr->m_tmpPoint = screenPoint;
    d_ptr->m_selectRegionOrigin = chart2D()->getSelectionRange();
}

void SASelectRegionDataEditor::moveEdit(const QPoint &toScreenPoint)
{
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return;
    }
    bool isAutoReplot = chart2D()->autoReplot();
    chart2D()->setAutoReplot(false);
    QPointF currentPoint = editor->invTransform(toScreenPoint);
    QPointF originPoint = editor->invTransform(d_ptr->m_tmpPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);
    //选区进行移动
    offsetRegion(offset);
    //数据进行移动
    int count = d_ptr->infoCount();
    for(int i=0;i<count;++i)
    {
        chart2d_base_info* info = d_ptr->info(i);
        QwtPlotItem* item = info->item();
        int xaxis = item->xAxis();
        int yaxis = item->yAxis();
#if 0
        double cx = plot()->canvasMap(xaxis).invTransform(screenPoint.x());
        double cy = plot()->canvasMap(yaxis).invTransform(screenPoint.y());
        double ox = plot()->canvasMap(xaxis).invTransform(d_ptr->m_tmpPoint.x());
        double oy = plot()->canvasMap(yaxis).invTransform(d_ptr->m_tmpPoint.y());
#else
        double cx = plot()->invTransform(xaxis,toScreenPoint.x());
        double cy = plot()->invTransform(yaxis,toScreenPoint.y());
        double ox = plot()->invTransform(xaxis,d_ptr->m_tmpPoint.x());
        double oy = plot()->invTransform(yaxis,d_ptr->m_tmpPoint.y());
#endif
        offset.rx() = cx-ox;
        offset.ry() = cy-oy;
        offsetData(offset);
    }
    d_ptr->m_tmpPoint = toScreenPoint;
    chart2D()->setAutoReplot(isAutoReplot);
}



bool SASelectRegionDataEditor::mousePressEvent(const QMouseEvent *e)
{
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    if(d_ptr->m_isStartKeyAction)
    {
        completeEdit(d_ptr->m_tmpPoint);

    }
    d_ptr->m_isStartMouseAction = true;
    startEdit(e->pos());
    return true;
}

bool SASelectRegionDataEditor::mouseMovedEvent(const QMouseEvent *e)
{
    if(!d_ptr->m_isStart || d_ptr->m_isStartKeyAction)
    {
        return false;
    }
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    moveEdit(e->pos());
    plot()->replot();
    return true;
}

bool SASelectRegionDataEditor::mouseReleasedEvent(const QMouseEvent *e)
{
    d_ptr->m_isStartMouseAction = false;
    return completeEdit(e->pos());
}

bool SASelectRegionDataEditor::keyPressEvent(const QKeyEvent *e)
{
    //响应方向键和回车键
    if(d_ptr->m_isStartMouseAction)
    {
        return false;
    }
    if(Qt::ControlModifier == e->modifiers())
    {
        //Ctrl 按下，进行精细移动
        switch(e->key())
        {
        case Qt::Key_Up:
        case Qt::Key_Right:
        case Qt::Key_Left:
        case Qt::Key_Down:
        {
            if(!d_ptr->m_isStartKeyAction)
            {
                //说明是第一次进行按键操作
                QPoint virtualScreenPoint = chart2D()->rect().center();
                startEdit(virtualScreenPoint);
                d_ptr->m_isStartKeyAction = true;
            }
            break;
        }
        default:
            return false;
        }

        switch(e->key())
        {
        case Qt::Key_Up:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() += 1;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Right:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.rx() += 1;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Left:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.rx() -= 1;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Down:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() -= 1;
            moveEdit(toPoint);
            break;
        }
        default:
            return false;
        }
        plot()->replot();
        return true;
    }
    else
    {
        //Ctrl没按下，进行粗移动
        switch(e->key())
        {
        case Qt::Key_Up:
        case Qt::Key_Right:
        case Qt::Key_Left:
        case Qt::Key_Down:
        {
            if(!d_ptr->m_isStartKeyAction)
            {
                //说明是第一次进行按键操作
                QPoint virtualScreenPoint = chart2D()->rect().center();
                startEdit(virtualScreenPoint);
                d_ptr->m_isStartKeyAction = true;
            }
            break;
        }
        //case Qt::Key
        default:
            return false;
        }
        int dw = plot()->width()/20;
        if(dw<=1)
        {
            dw = 1;
        }
        int dh = plot()->height()/20;
        if(dh<=1)
        {
            dh = 1;
        }

        switch(e->key())
        {
        case Qt::Key_Up:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() -= dh;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Right:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.rx() += dw;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Left:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.rx() -= dw;
            moveEdit(toPoint);
            break;
        }
        case Qt::Key_Down:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() += dh;
            moveEdit(toPoint);
            break;
        }
        default:
            return false;
        }
        plot()->replot();
        return true;
    }
    return false;
}

bool SASelectRegionDataEditor::keyReleaseEvent(const QKeyEvent *e)
{
    return false;
}
