#include "SASelectRegionDataEditor.h"
#include "SAChart2D.h"
#include "SAChart.h"
#include "qwt_plot_barchart.h"
#include "SAAbstractRegionSelectEditor.h"
#include "czyQtPointAlgorithm.h"
#include "SAFigureOptCommands.h"
#include <QHash>
#include "SALog.h"
#include <memory>
#include "SALog.h"
class chart2d_base_info
{
public:
    enum RTTI
    {
        RTTI_SeriesStoreInfo
    };

    chart2d_base_info():plotItem(nullptr)
    {

    }
    virtual ~chart2d_base_info(){}
    virtual int rtti() const = 0;
    //偏移数据，输入偏移的屏幕坐标点，会根据上次的偏移进行偏移，返回实际绘图坐标系的偏移量，偏移量的坐标系为当前QwtPlotItem的坐标系
    virtual QPointF offsetData(const QPointF &screenPoint) = 0;
    //重置偏移基准
    virtual void resetOffsetBase(const QPointF &screenPoint) = 0;
    //接受更改，接受更改应该向sachart2d写入命令
    virtual SAFigureOptCommand* accept(QUndoCommand* parent) = 0;
    QwtPlotItem* item(){return plotItem;}
    void setItem(QwtPlotItem* v){plotItem  =v;}
protected:

    QwtPlotItem* plotItem;
};

template<typename T,typename PlotItemType,typename OffsetFun,typename FpSetSeriesSampleFun>
class chart2d_series_store_info : public chart2d_base_info
{
public:
    chart2d_series_store_info(QwtPlotItem* item,OffsetFun offsetFun,FpSetSeriesSampleFun fpSetSeries):chart2d_base_info()
      ,m_fpOffsetFun(offsetFun)
      ,m_fpSetSample(fpSetSeries)
    {
        m_plotItem = static_cast<PlotItemType*>(item);
        setItem(item);
    }

    int rtti() const{return RTTI_SeriesStoreInfo;}
    void setDatas(const QVector<T>& p){m_datas = p;}
    const QVector<T>& datas() const{return m_datas;}
    QVector<T>& datas(){return m_datas;}
    const QVector<int>& indexs() const{return m_indexs;}
    QVector<int>& indexs() {return m_indexs;}
    void setIndexs(const QVector<int>& v){m_indexs = v;}
    int size() const{return qMin(m_datas.size(),m_indexs.size());}
    template<typename CheckFun>
    void init(const QPainterPath& otPath,int xaxis,int yaxis,CheckFun fun)
    {
        m_indexs.clear();
        m_datas.clear();
        if(xaxis != item()->xAxis() || yaxis != item()->yAxis())
        {
            QPainterPath trPath = SAChart::transformPath(item()->plot(),otPath,xaxis,yaxis
                                                       ,item()->xAxis(),item()->yAxis());

            for(size_t i=0;i<m_plotItem->dataSize();++i)
            {
                if(fun(trPath,m_plotItem->sample(i)))
                {
                    m_indexs.append((int)i);
                    m_datas.append(m_plotItem->sample(i));
                }
            }
            m_newDatas = m_datas;
        }
        else
        {
            for(size_t i=0;i<m_plotItem->dataSize();++i)
            {
                if(fun(otPath,m_plotItem->sample(i)))
                {
                    m_indexs.append((int)i);
                    m_datas.append(m_plotItem->sample(i));
                }
            }
            m_newDatas = m_datas;
        }
    }
    virtual QPointF offsetData(const QPointF &screenPoint)
    {
        int xaxis = item()->xAxis();
        int yaxis = item()->yAxis();
        QwtPlot* plot = item()->plot();
        if(nullptr == plot)
        {
            saError("item dose not bind plot");
            return QPointF();
        }
        m_currentOffset = SAChart::screenPointToPlotPoint(plot,screenPoint,xaxis,yaxis);
        if(m_firstOffset.isNull())
        {
            m_firstOffset.reset(new QPointF(m_currentOffset));
        }
        if(m_lastOffset == m_currentOffset)
        {
            return QPointF();
        }
        QPointF plotOffset = m_currentOffset - m_lastOffset;
        m_lastOffset = m_currentOffset;
        for(int i=0;i<m_newDatas.size()&&i<m_indexs.size();++i)
        {
            m_fpOffsetFun(m_newDatas[i],plotOffset.x(),plotOffset.y());
        }
        replacePlotData();
        return plotOffset;
    }
    virtual void resetOffsetBase(const QPointF &screenPoint)
    {
        m_firstOffset.reset();
        m_lastOffset = SAChart::screenPointToPlotPoint(item()->plot(),screenPoint,item()->xAxis(),item()->yAxis());
        m_currentOffset = m_lastOffset;
    }
    void replacePlotData()
    {
        QVector<T> newDatas;
        newDatas.reserve(m_plotItem->dataSize());
        SAChart::getSeriesData(newDatas,m_plotItem);
        for(int i=0;i<m_indexs.size()&&i<m_newDatas.size();++i)
        {
            if(m_indexs[i] < newDatas.size())
            {
                newDatas[m_indexs[i]] = m_newDatas[i];
            }
        }
        m_fpSetSample(item(),newDatas);
    }
    virtual SAFigureOptCommand* accept(QUndoCommand* parent)
    {
        SAChart2D* chart = qobject_cast<SAChart2D*>(item()->plot());
        if(nullptr == chart)
            return nullptr;
        SAFigureReplaceDatasCommand<T>* cmd
                = new SAFigureReplaceDatasCommand<T>(
                    chart
                    ,item()
                    ,m_indexs
                    ,m_datas
                    ,m_newDatas
                    ,QObject::tr("move range datas")
                    ,parent
                    );
        return cmd;
    }


protected:
    PlotItemType* m_plotItem;
    QVector<T> m_datas;
    QVector<int> m_indexs;
    QVector<T> m_newDatas;
    QScopedPointer<QPointF> m_firstOffset;
    QPointF m_lastOffset;
    QPointF m_currentOffset;
    OffsetFun m_fpOffsetFun;
    FpSetSeriesSampleFun m_fpSetSample;///< fun(QwtPlotItem* item,cosnt QVector<T>& series);
};

class SASelectRegionDataEditorPrivate
{
    SA_IMPL_PUBLIC(SASelectRegionDataEditor)
    QList<chart2d_base_info*> m_curListInfo;//保存选中的曲线信息
public:
    bool m_isStart;//是否开始
    bool m_isStartMouseAction;///< 是否鼠标激活
    bool m_isStartKeyAction;///< 是否按键激活
    QPainterPath m_selectRegionOrigin;//保存选择的原始区域
    QPoint m_firstPressedScreenPoint;//第一次按下的点
    QPoint m_tmpPoint;//记录临时点
    SASelectRegionDataEditorPrivate(SASelectRegionDataEditor* p)
        :q_ptr(p)
        ,m_isStart(false)
        ,m_isStartMouseAction(false)
        ,m_isStartKeyAction(false)
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
    //说明上次还说处于键盘移动状态
    qDebug() <<"SASelectRegionDataEditor destroy";
    completeKeyActionEdit();
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

void SASelectRegionDataEditor::setEnabled(bool on)
{
    //说明上次还说处于键盘移动状态
    completeKeyActionEdit();
    SAAbstractPlotEditor::setEnabled(on);
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
            typedef chart2d_series_store_info
                    <QPointF
                    ,QwtPlotCurve
                    ,decltype(&SAChart::offsetPointSample)
                    ,decltype(&SAChart::setPlotCurveSample)
                    > SerInfo;
            QScopedPointer<SerInfo> ser(new SerInfo(item,&SAChart::offsetPointSample,&SAChart::setPlotCurveSample));
            ser->init(d_ptr->m_selectRegionOrigin,editor->getXAxis(),editor->getYAxis(),&SAChart::isPointInRange);
            if(ser->size() > 0)
            {
                d_ptr->appendInfo(ser.take());
            }
            break;
        }
        case QwtPlotItem::Rtti_PlotBarChart:
        {
            typedef chart2d_series_store_info
                    <QPointF
                    ,QwtPlotBarChart
                    ,decltype(&SAChart::offsetPointSample)
                    ,decltype(&SAChart::setPlotBarChartSample)
                    > SerInfo;
            QScopedPointer<SerInfo> ser(new SerInfo(item,&SAChart::offsetPointSample,&SAChart::setPlotBarChartSample));
            ser->init(d_ptr->m_selectRegionOrigin,editor->getXAxis(),editor->getYAxis(),&SAChart::isPointInRange);
            if(ser->size() > 0)
            {
                d_ptr->appendInfo(ser.take());
            }
            break;
        }
        case QwtPlotItem::Rtti_PlotSpectroCurve:
        {
            typedef chart2d_series_store_info
                    <QwtPoint3D
                    ,QwtPlotSpectroCurve
                    ,decltype(&SAChart::offsetSpectroCurveSample)
                    ,decltype(&SAChart::setPlotSpectroCurveSample)
                    > SerInfo;
            QScopedPointer<SerInfo> ser(new SerInfo(item,&SAChart::offsetSpectroCurveSample,&SAChart::setPlotSpectroCurveSample));
            ser->init(d_ptr->m_selectRegionOrigin,editor->getXAxis(),editor->getYAxis(),&SAChart::isSpectroCurveSampleInRange);
            if(ser->size() > 0)
            {
                d_ptr->appendInfo(ser.take());
            }
            break;
        }
        case QwtPlotItem::Rtti_PlotIntervalCurve:
        {
            typedef chart2d_series_store_info
                    <QwtIntervalSample
                    ,QwtPlotIntervalCurve
                    ,decltype(&SAChart::offsetIntervalCurveSample)
                    ,decltype(&SAChart::setPlotIntervalCurveSample)
                    > SerInfo;
            QScopedPointer<SerInfo> ser(new SerInfo(item,&SAChart::offsetIntervalCurveSample,&SAChart::setPlotIntervalCurveSample));
            ser->init(d_ptr->m_selectRegionOrigin,editor->getXAxis(),editor->getYAxis(),&SAChart::isIntervalCurveSampleInRange);
            if(ser->size() > 0)
            {
                d_ptr->appendInfo(ser.take());
            }
            break;
        }
        case QwtPlotItem::Rtti_PlotHistogram:
        {
            typedef chart2d_series_store_info
                    <QwtIntervalSample
                    ,QwtPlotHistogram
                    ,decltype(&SAChart::offsetHistogramSample)
                    ,decltype(&SAChart::setPlotHistogramSample)
                    > SerInfo;
            QScopedPointer<SerInfo> ser(new SerInfo(item,&SAChart::offsetHistogramSample,&SAChart::setPlotHistogramSample));
            ser->init(d_ptr->m_selectRegionOrigin,editor->getXAxis(),editor->getYAxis(),&SAChart::isHistogramSampleInRange);
            if(ser->size() > 0)
            {
                d_ptr->appendInfo(ser.take());
            }
            break;
        }
        case QwtPlotItem::Rtti_PlotTradingCurve:
        {
            typedef chart2d_series_store_info
                    <QwtOHLCSample
                    ,QwtPlotTradingCurve
                    ,decltype(&SAChart::offsetTradingCurveSample)
                    ,decltype(&SAChart::setPlotTradingCurveSample)
                    > SerInfo;
            QScopedPointer<SerInfo> ser(new SerInfo(item,&SAChart::offsetTradingCurveSample,&SAChart::setPlotTradingCurveSample));
            ser->init(d_ptr->m_selectRegionOrigin,editor->getXAxis(),editor->getYAxis(),&SAChart::isTradingCurveSampleInRange);
            if(ser->size() > 0)
            {
                d_ptr->appendInfo(ser.take());
            }
            break;
        }
        case QwtPlotItem::Rtti_PlotMultiBarChart:
        {
            typedef chart2d_series_store_info
                    <QwtSetSample
                    ,QwtPlotMultiBarChart
                    ,decltype(&SAChart::offsetMultiBarChartSample)
                    ,decltype(&SAChart::setPlotMultiBarChartSample)
                    > SerInfo;
            QScopedPointer<SerInfo> ser(new SerInfo(item,&SAChart::offsetMultiBarChartSample,&SAChart::setPlotMultiBarChartSample));
            ser->init(d_ptr->m_selectRegionOrigin,editor->getXAxis(),editor->getYAxis(),&SAChart::isMultiBarChartSampleInRange);
            if(ser->size() > 0)
            {
                d_ptr->appendInfo(ser.take());
            }
            break;
        }
        default:
            break;
        }



    }
}


void SASelectRegionDataEditor::offsetRegion(const QPointF &offset)
{
    //选区进行移动
    QPainterPath p = chart2D()->getSelectionRange();
    p.translate(offset);
    chart2D()->setSelectionRange(p);
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
    if(d_ptr->m_firstPressedScreenPoint == screenPoint)
    {
        d_ptr->m_tmpPoint = screenPoint;
        return true;
    }
    bool isAutoReplot = chart2D()->autoReplot();
    chart2D()->setAutoReplot(false);
    SAFigureOptCommand *topCmd = new SAFigureOptCommand(chart2D(),tr("move region datas")); // 创建一个命令集
    //选区进行移动
    QPointF currentPoint = editor->invTransform(screenPoint);
    QPointF originPoint = editor->invTransform(d_ptr->m_firstPressedScreenPoint);
    QPointF offset = czy::calcOffset(currentPoint,originPoint);
    QPainterPath p = d_ptr->m_selectRegionOrigin.translated(offset);
    new SAFigureChartSelectionRegionAddCommand(chart2D()
                                                ,d_ptr->m_selectRegionOrigin
                                                ,p
                                                ,tr("move region")
                                                ,topCmd);
    //
    //数据进行移动
    int count = d_ptr->infoCount();
    for(int i=0;i<count;++i)
    {
        chart2d_base_info* info = d_ptr->info(i);
        info->offsetData(screenPoint);
        info->accept(topCmd);
    }
    chart2D()->appendCommand(topCmd);
    chart2D()->setAutoReplot(isAutoReplot);
    d_ptr->m_firstPressedScreenPoint = screenPoint;
    d_ptr->m_tmpPoint = screenPoint;
    return true;
}

void SASelectRegionDataEditor::startEdit(const QPoint &screenPoint)
{
    plot()->setCursor(QCursor(Qt::SizeAllCursor));
    d_ptr->m_isStart = true;
    d_ptr->m_firstPressedScreenPoint = screenPoint;
    d_ptr->m_tmpPoint = screenPoint;
    d_ptr->m_selectRegionOrigin = chart2D()->getSelectionRange();
    int count = d_ptr->infoCount();
    for(int i=0;i<count;++i)
    {
        chart2d_base_info* info = d_ptr->info(i);
        info->resetOffsetBase(screenPoint);
    }
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
    QPointF offset = currentPoint - originPoint;
    //选区进行移动
    offsetRegion(offset);
    //数据进行移动
    int count = d_ptr->infoCount();
    for(int i=0;i<count;++i)
    {
        chart2d_base_info* info = d_ptr->info(i);
        info->offsetData(toScreenPoint);
    }
    d_ptr->m_tmpPoint = toScreenPoint;
    chart2D()->setAutoReplot(isAutoReplot);
}

void SASelectRegionDataEditor::completeKeyActionEdit()
{
    if(d_ptr->m_isStartKeyAction)
    {
        completeEdit(d_ptr->m_tmpPoint);
    }
}



bool SASelectRegionDataEditor::mousePressEvent(const QMouseEvent *e)
{
    SAAbstractRegionSelectEditor* editor = chart2D()->getRegionSelectEditor();
    if(!editor)
    {
        return false;
    }
    completeKeyActionEdit();//如果按键激活过，把激活的记录保存，如果保存了，此函数不做任何处理
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
    qDebug() << "key press";
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
        case Qt::Key_Enter://按下回车键确认键盘的移动
            completeKeyActionEdit();
            return true;
        default:
            return false;
        }

        switch(e->key())
        {
        case Qt::Key_Up:
        {
            QPoint toPoint = d_ptr->m_tmpPoint;
            toPoint.ry() -= 1;
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
            toPoint.ry() += 1;
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






