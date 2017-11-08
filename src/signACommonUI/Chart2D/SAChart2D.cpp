#include "SAChart2D.h"
#include "SAChart.h"
#include "SAAbstractDatas.h"
#include "SARandColorMaker.h"
#include "SAFigureGlobalConfig.h"
#include <memory>
#include "SAXYSeries.h"
#include "SABarSeries.h"
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include "SAValueManagerMimeData.h"
#include "SAValueManager.h"
#include "SATendencyChartDataSelectDialog.h"
#include "SALog.h"
#include "SARectRegionSelectEditor.h"
#include "SAEllipseRegionSelectEditor.h"
#include "SAPolygonRegionSelectEditor.h"
SAChart2D::SAChart2D(QWidget *parent):SA2DGraph(parent)
  ,m_chartSelectRigionEditor(nullptr)
{
    setAcceptDrops(true);
}

SAChart2D::~SAChart2D()
{
}
///
/// \brief 添加曲线
/// \param datas
/// \return
///
SAXYSeries *SAChart2D::addCurve(SAAbstractDatas *datas)
{
    std::unique_ptr<SAXYSeries> series(new SAXYSeries(datas->getName(),datas));
    if(series->dataSize() <= 0)
    {
        return nullptr;
    }
    series->setPen(SARandColorMaker::getCurveColor()
                ,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
    series->attach(this);
    emit plotCurveChanged(series.get());
    return series.release();
}
///
/// \brief SAChart2D::addCurve
/// \param datas
/// \param xStart
/// \param xDetal
/// \return
///
SAXYSeries *SAChart2D::addCurve(SAAbstractDatas *datas, double xStart, double xDetal)
{
    std::unique_ptr<SAXYSeries> series(new SAXYSeries(datas->getName()));
    series->setSamples(datas,xStart,xDetal);
    if(series->dataSize() <= 0)
    {
        return nullptr;
    }
    series->setPen(SARandColorMaker::getCurveColor()
                ,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
    series->attach(this);
    emit plotCurveChanged(series.get());
    return series.release();
}
///
/// \brief SAChart2D::addCurve
/// \param x
/// \param y
/// \param name
/// \return
///
SAXYSeries *SAChart2D::addCurve(SAAbstractDatas *x, SAAbstractDatas *y, const QString &name)
{
    std::unique_ptr<SAXYSeries> series(new SAXYSeries(name));
    series->setSamples(x,y);
    if(series->dataSize() <= 0)
    {
        return nullptr;
    }
    series->setPen(SARandColorMaker::getCurveColor()
                ,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
    series->attach(this);
    emit plotCurveChanged(series.get());
    return series.release();
}


void SAChart2D::addCurve(QwtPlotCurve *cur)
{
    cur->attach(this);
}
///
/// \brief SAChart2D::addBar
/// \param datas
/// \return
///
SABarSeries *SAChart2D::addBar(SAAbstractDatas *datas)
{
    std::unique_ptr<SABarSeries> series(new SABarSeries(datas,datas->getName()));
    if(series->dataSize() <= 0)
    {
        return nullptr;
    }
    QColor clr = SARandColorMaker::getCurveColor();
    series->setBrush(QBrush(clr));
    series->setPen(clr,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
    series->setStyle(QwtPlotHistogram::Columns);
    series->attach(this);
    emit plotCurveChanged(series.get());
    return series.release();
}

///
/// \brief 移除范围内数据
/// \param curves 需要移除的曲线列表
///
void SAChart2D::removeDataInRang(QList<QwtPlotCurve *> curves)
{
    setAutoReplot(false);
    QPainterPath region = getVisibleRegion();
    if(region.isEmpty())
    {
        return;
    }
    SAAbstractRegionSelectEditor* editor = getRegionSelectEditor();
    if(nullptr == editor)
    {
        return;
    }
    QHash<QPair<int,int>,QPainterPath> otherScaleMap;
    for(int i=0;i<curves.size();++i)
    {
        int xa = curves[i]->xAxis();
        int ya = curves[i]->yAxis();
        if(xa == editor->getXAxis() && ya == editor->getYAxis())
        {
            SAChart::removeDataInRang(region,curves[i]);
        }
        else
        {
            QPair<int,int> axiss=qMakePair(xa,ya);
            if(!otherScaleMap.contains(axiss))
            {
                otherScaleMap[axiss] = editor->transformToOtherAxis(xa,ya);

            }
            SAChart::removeDataInRang(otherScaleMap.value(axiss)
                                      ,curves[i]);
        }
    }
    setAutoReplot(true);
    replot();
}
///
/// \brief 开始选择模式
/// 选择模式可分为矩形，圆形等，具体见\sa SelectionMode
/// \param mode 选择模式
/// \see SelectionMode
///
void SAChart2D::enableSelection(SelectionMode mode,bool on)
{
    if(on)
    {
        if(NoneSelection == mode)
        {
            return;
        }
        stopSelectMode();
        m_selectMode = mode;
        switch(m_selectMode)
        {
        case RectSelection:startRectSelectMode();break;
        case EllipseSelection:startEllipseSelectMode();break;
        case PolygonSelection:startPolygonSelectMode();break;
        default:return;
        }
    }
    else
    {
        stopSelectMode();
    }
}
///
/// \brief 判断当前的选择模式
/// \param mode
/// \return
///
bool SAChart2D::isEnableSelection(SAChart2D::SelectionMode mode) const
{
    if(nullptr == m_chartSelectRigionEditor)
    {
        return false;
    }
    if(!m_chartSelectRigionEditor->isEnabled())
    {
        return false;
    }
    switch(mode)
    {
    case RectSelection:
        return m_chartSelectRigionEditor->rtti() == SAAbstractPlotEditor::RTTIRectRegionSelectEditor;
    case EllipseSelection:
        return m_chartSelectRigionEditor->rtti() == SAAbstractPlotEditor::RTTIEllipseRegionSelectEditor;
    case PolygonSelection:
        return m_chartSelectRigionEditor->rtti() == SAAbstractPlotEditor::RTTIPolygonRegionSelectEditor;
    default:
        break;
    }
    return false;
}
///
/// \brief 结束当前的选择模式
///
void SAChart2D::stopSelectMode()
{
    if(nullptr == m_chartSelectRigionEditor)
    {
        return;
    }
    m_chartSelectRigionEditor->setEnabled(false);
    m_selectMode = NoneSelection;
}
///
/// \brief 清除所有选区
///
void SAChart2D::clearAllSelectedRegion()
{
    if(nullptr == m_chartSelectRigionEditor)
    {
        return;
    }
    delete m_chartSelectRigionEditor;
    m_chartSelectRigionEditor = nullptr;
}
///
/// \brief 判断是否有选区
/// \return
///
bool SAChart2D::isRegionVisible() const
{
    if(nullptr == m_chartSelectRigionEditor)
    {
        return false;
    }
    return m_chartSelectRigionEditor->isRegionVisible();
}
///
/// \brief 获取当前正在显示的选择区域
/// \return
///
SAChart2D::SelectionMode SAChart2D::currentSelectRegionMode() const
{
    return m_selectMode;
}
///
/// \brief 获取矩形选择编辑器
/// \return 如果没有设置编辑器，返回nullptr
///
SAAbstractRegionSelectEditor *SAChart2D::getRegionSelectEditor()
{
    return m_chartSelectRigionEditor;
}

const SAAbstractRegionSelectEditor *SAChart2D::getRegionSelectEditor() const
{
    return m_chartSelectRigionEditor;
}
///
/// \brief 获取当前可见的选区的范围
/// \return
///
QPainterPath SAChart2D::getVisibleRegion() const
{
    const SAAbstractRegionSelectEditor* editor = getRegionSelectEditor();
    if(editor)
    {
        if(editor->isRegionVisible())
        {
            return editor->getSelectRegion();
        }
    }
    return QPainterPath();
}
///
/// \brief 开始矩形选框模式
///
void SAChart2D::startRectSelectMode()
{
    QPainterPath tmp;
    if(m_chartSelectRigionEditor)
    {
        tmp = m_chartSelectRigionEditor->getSelectRegion();
        delete m_chartSelectRigionEditor;
        m_chartSelectRigionEditor = nullptr;
    }
    if(nullptr == m_chartSelectRigionEditor)
    {
        m_chartSelectRigionEditor = new SARectRegionSelectEditor(this);
        if(!tmp.isEmpty())
        {
            m_chartSelectRigionEditor->setSelectRegion(tmp);
        }
    }
    m_chartSelectRigionEditor->setEnabled(true);
}
///
/// \brief 开始椭圆选框模式
///
void SAChart2D::startEllipseSelectMode()
{
    QPainterPath tmp;
    if(m_chartSelectRigionEditor)
    {
        tmp = m_chartSelectRigionEditor->getSelectRegion();
        delete m_chartSelectRigionEditor;
        m_chartSelectRigionEditor = nullptr;
    }
    if(nullptr == m_chartSelectRigionEditor)
    {
        m_chartSelectRigionEditor = new SAEllipseRegionSelectEditor(this);
        if(!tmp.isEmpty())
        {
            m_chartSelectRigionEditor->setSelectRegion(tmp);
        }
    }
    m_chartSelectRigionEditor->setEnabled(true);
}
///
/// \brief 开始多边形选框模式
///
void SAChart2D::startPolygonSelectMode()
{
    QPainterPath tmp;
    if(m_chartSelectRigionEditor)
    {
        tmp = m_chartSelectRigionEditor->getSelectRegion();
        delete m_chartSelectRigionEditor;
        m_chartSelectRigionEditor = nullptr;
    }
    if(nullptr == m_chartSelectRigionEditor)
    {
        m_chartSelectRigionEditor = new SAPolygonRegionSelectEditor(this);
        if(!tmp.isEmpty())
        {
            m_chartSelectRigionEditor->setSelectRegion(tmp);
        }
    }
    m_chartSelectRigionEditor->setEnabled(true);
}

///
/// \brief 向chart添加一组数据
/// \param datas
///
void SAChart2D::addDatas(const QList<SAAbstractDatas *> &datas)
{
    const int dataCount = datas.size();
    if(1 == dataCount)
    {
        if(SA::Dim1 == datas[0]->getDim())
        {
            SATendencyChartDataSelectDialog dlg(this);
            if(QDialog::Accepted != dlg.exec())
            {
                return;
            }
            if(dlg.isFollow())
            {
                QwtPlotCurve* cur = dlg.getSelFollowCurs();
                std::unique_ptr<SAXYSeries> series(new SAXYSeries(datas[0]->getName()));
                QVector<double> x,y;
                if(!SAAbstractDatas::converToDoubleVector(datas[0],y))
                {
                    return;
                }
                SAChart::getXDatas(x,cur);
                if(x.size() < y.size())
                {
                    y.resize(x.size());
                }
                else
                {
                    x.resize(y.size());
                }
                series->setSamples(x,y);
                series->setPen(SARandColorMaker::getCurveColor()
                            ,SAFigureGlobalConfig::getPlotCurWidth(series->dataSize()));
                series->attach(this);
                emit plotCurveChanged(series.release());
            }
            else if(dlg.isSelDef())
            {
                double startData,addedData;
                dlg.getSelDefData(startData,addedData);
                addCurve(datas[0],startData,addedData);
            }
            return;
        }
        else
        {
            addCurve(datas[0]);
        }
    }
    else if (2 == dataCount)
    {
        if(SA::Dim1 == datas[0]->getDim()
                &&SA::Dim1 == datas[1]->getDim())
        {
            addCurve(datas[0],datas[1]
                    ,QString("%1-%2")
                    .arg(datas[0]->getName())
                    .arg(datas[1]->getName()));
        }
    }
    else
    {
        std::for_each(datas.cbegin(),datas.cend(),[this](SAAbstractDatas* data){
            addCurve(data);
        });
    }
}

void SAChart2D::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void SAChart2D::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void SAChart2D::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
    {
        QList<int> ids;
        if(SAValueManagerMimeData::getValueIDsFromMimeData(event->mimeData(),ids))
        {
            QList<SAAbstractDatas*> datas = saValueManager->findDatas(ids);
            addDatas(datas);
        }
    }
}


