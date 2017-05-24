#include "SAFigureWindow.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QGridLayout>
//sa chart
#include "DataFeatureItem.h"
#include "SAMultWidget.h"
#include "SAChart2D.h"
//sa lib
#include "SAData.h"
#include "SARandColorMaker.h"
#include "SAGlobalConfig.h"

#define GET_CHART_PTR \
SAChart2D* chart = current2DPlot();\
if(nullptr == chart)\
{\
    return nullptr;\
}

#define GET_CHART_PTR_RETURN_VOID \
SAChart2D* chart = current2DPlot();\
if(nullptr == chart)\
{\
    return;\
}

void SAFigureWindow::UI::setupUI(SAFigureWindow *par)
{
    centralwidget = new SAMultWidget(par);
    centralwidget->setObjectName(QStringLiteral("centralwidget"));
    par->setCentralWidget(centralwidget);
}

void SAFigureWindow::UI::retranslateUi(SAFigureWindow *par)
{
    par->setWindowTitle(QApplication::translate("SAFigureWindow", "Figure", 0));
}

//=============================================================================

SAFigureWindow::SAFigureWindow(QWidget *parent) :
    QMainWindow(parent)
    ,ui(new UI)
  ,m_lastPlotItem(nullptr)
{
    ui->setupUI(this);
    setAutoFillBackground(true);
    setBackgroundColor(QColor(255,255,255));
    static int s_figure_count=0;
    ++s_figure_count;
    setWindowTitle(QString("figure-%1").arg(s_figure_count));
    initUI();
}

SAFigureWindow::~SAFigureWindow()
{
    auto i = m_dataFeature.begin();
    auto end = m_dataFeature.end();
    for(;i!=end;++i)
    {
        AbstractDataFeatureItem* ptr = *i;
        if(nullptr != ptr)
        {
            delete ptr;
        }
    }
}

///
/// \brief 添加一个2Dchart
/// \return 返回2D绘图的指针
///
SAChart2D *SAFigureWindow::create2DPlot()
{
    try
    {
        SAChart2D* plot = new SAChart2D(this);
        ui->centralwidget->addWidget (plot,0,0,-1,-1,0);
        return plot;
    }
    catch(std::bad_alloc& b)
    {
        Q_UNUSED(b);
        QMessageBox::warning (this,tr("memory out"),tr("memory out"));
        return nullptr;
    }
    return nullptr;
}
///
/// \brief 添加一个2D sub chart
/// \param widget 要添加的窗体指针
/// \param fromRow 行
/// \param fromColumn 列
/// \param rowSpan 行的占用
/// \param columnSpan 列的占用
/// \param alignment 对齐方式
/// \return 返回绘图的Chart指针
///
SAChart2D *SAFigureWindow::create2DSubPlot(int fromRow, int fromColumn, int rowSpan, int columnSpan)
{
    try
    {
        SAChart2D* plot = new SAChart2D(this);
        ui->centralwidget->addWidget (plot,fromRow,fromColumn,rowSpan,columnSpan,0);
        return plot;
    }
    catch(std::bad_alloc& b)
    {
        Q_UNUSED(b);
        QMessageBox::warning (this,tr("memory out"),tr("memory out"));
        return nullptr;
    }
    return nullptr;
}
///
/// \brief 获取所有的图像列表
/// \return
///
QList<SAChart2D *> SAFigureWindow::get2DPlots() const
{
    QList<SAChart2D*> res;
    QGridLayout* grid = ui->centralwidget->getGridLayout();
    const int itemCount = grid->count();
    for(int i=0;i<itemCount;++i)
    {
        QLayoutItem* item = grid->itemAt(i);
        if(item)
        {
            QWidget* w = item->widget();
            if(w)
            {
                SAChart2D* chart = qobject_cast<SAChart2D*>(w);
                res.append(chart);
            }
        }

    }
    return res;
}
///
/// \brief 根据定位信息获取子绘图窗口
/// \param fromRow 行
/// \param fromColumn 列
/// \param rowSpan 行占位
/// \param columnSpan 列占位
/// \return 如果没有，返回nullptr
///
SAChart2D *SAFigureWindow::get2DPlot(int fromRow, int fromColumn) const
{
    QGridLayout* grid = ui->centralwidget->getGridLayout();
    QLayoutItem * item = grid->itemAtPosition(fromRow,fromColumn);
    if(item)
    {
        QWidget* w = item->widget();
        return qobject_cast<SAChart2D*>(w);
    }
    return nullptr;
}
///
/// \brief 当前的2d绘图指针
/// \return 当没有2d绘图时返回nullptr
///
SAChart2D *SAFigureWindow::current2DPlot() const
{
    return qobject_cast<SAChart2D*>(ui->centralwidget->currentWidget());
}
///
/// \brief 获取绘图结果中的特征属性信息
/// \return
///
QList<AbstractDataFeatureItem *> SAFigureWindow::getDataFeatureItemList() const
{
    return m_dataFeature;
}

///
/// \brief 添加趋势线
/// \param datas 数据
/// \return 曲线指针
/// \note 需要保证数据能toPointFVector转换为点序列,否则返回空指针
///
QwtPlotCurve *SAFigureWindow::addCurve(SAAbstractDatas *datas)
{
    GET_CHART_PTR
    QVector<QPointF> serPoints;
    if(SAAbstractDatas::converToPointFVector(datas,serPoints))
    {
        QwtPlotCurve* cur =  addCurve(serPoints,datas->getName());
        rememberRelationShip(cur,{datas});
        m_lastPlotItem = cur;
        return cur;
    }
    QVector<double> serVectors;
    if(SAAbstractDatas::converToDoubleVector(datas,serVectors))
    {
        QwtPlotCurve* cur =  addCurve(serVectors,0,1,datas->getName());
        rememberRelationShip(cur,{datas});
        m_lastPlotItem = cur;
        return cur;
    }
    return nullptr;

}
///
/// \brief 添加趋势线
/// \param datas 趋势线的值
/// \param xStart 趋势开始的x点
/// \param xDetal 趋势每两个点的间距
/// \return 添加的趋势线指针
///
QwtPlotCurve *SAFigureWindow::addCurve(SAAbstractDatas *datas, double xStart, double xDetal)
{
    GET_CHART_PTR
    QVector<double> yd;
    if(!SAAbstractDatas::converToDoubleVector(datas,yd))
    {
        return nullptr;
    }
    if(0 == yd.size())
    {
        return nullptr;
    }
    QwtPlotCurve* cur =  addCurve(yd,xStart,xDetal,datas->getName());
    rememberRelationShip(cur,{datas});
    m_lastPlotItem = cur;
    return cur;
}

QwtPlotCurve *SAFigureWindow::addCurve(SAAbstractDatas *x, SAAbstractDatas *y, const QString &name)
{
    GET_CHART_PTR
    QVector<double> xd,yd;
    if(!SAAbstractDatas::converToDoubleVector(x,xd))
        return nullptr;
    if(!SAAbstractDatas::converToDoubleVector(y,yd))
        return nullptr;

    QwtPlotCurve* cur = addCurve(xd,yd,name);
    rememberRelationShip(cur,{x,y} );
    m_lastPlotItem = cur;
    return cur;
}
///
/// \brief 绘制基本图形
/// \param points 点集
/// \param name 图形名字
/// \return 绘图曲线
///
QwtPlotCurve *SAFigureWindow::addCurve(const QVector<QPointF> &points,const QString &name)
{
    GET_CHART_PTR
    QwtPlotCurve* cur = chart->addCurve(points);
    cur->setPen(SARandColorMaker::getCurveColor()
                ,saConfig->getPlotCurWidth(points.size()));
    cur->setTitle(name);
    calcDataFeature(chart,cur);
    m_lastPlotItem = cur;
    return cur;
}
///
/// \brief 绘制基本图形
/// \param datas 数据（y）
/// \param xStart x初始
/// \param xDetal x增加
/// \param name 图形名字
/// \return
///
QwtPlotCurve *SAFigureWindow::addCurve(const QVector<double> &datas, double xStart, double xDetal,const QString &name)
{
    GET_CHART_PTR
    QVector<double> x;
    x.resize(datas.size());
    for(int i=0;i<datas.size();++i)
    {
        x[i] = xStart + xDetal * i;
    }
    QwtPlotCurve* cur = chart->addCurve(x,datas);
    cur->setPen(SARandColorMaker::getCurveColor()
                ,saConfig->getPlotCurWidth(datas.size()));
    cur->setTitle(name);
    calcDataFeature(chart,cur);
    m_lastPlotItem = cur;
    return cur;
}
///
/// \brief 绘制基本图形
/// \param x 数据（x）
/// \param y 数据（y）
/// \param name 图形名字
/// \return
///
QwtPlotCurve *SAFigureWindow::addCurve(const QVector<double> &x, const QVector<double> &y, const QString &name)
{
    GET_CHART_PTR
    int min = std::min(x.size(),y.size());
    QwtPlotCurve* cur = chart->addCurve(x,y);
    cur->setPen(SARandColorMaker::getCurveColor()
                ,saConfig->getPlotCurWidth(min));
    cur->setTitle(name);
    calcDataFeature(chart,cur);
    m_lastPlotItem = cur;
    return cur;
}
///
/// \brief 添加柱状图
/// \param datas 数据
/// \param style 柱状图的样式
/// \return 柱状图的指针
///
QwtPlotHistogram *SAFigureWindow::addBar(SAVectorInterval *datas, QwtPlotHistogram::HistogramStyle style)
{
    GET_CHART_PTR
    QwtPlotHistogram* hp = chart->addHistogram (datas->getValueDatas ());
    QColor clr = SARandColorMaker::getCurveColor();
    hp->setPen(clr,saConfig->getPlotCurWidth(datas->getSize()));
    clr.setAlpha(125);
    hp->setBrush(QBrush(clr));
    hp->setTitle(datas->getName ());
    hp->setStyle(style);
    calcDataFeature(chart,hp);
    rememberRelationShip(hp,QList<SAAbstractDatas*>()<<datas);
    m_lastPlotItem = hp;
    return hp;

}
///
/// \brief 添加一个虚拟的线，此线不会记录其
/// \param strName 曲线名
/// \param datas 数据
/// \return
///
QwtPlotCurve *SAFigureWindow::addVirtualCurve(const QString &strName, const QVector<QPointF> &datas)
{
    GET_CHART_PTR
    QwtPlotCurve* cur =  chart->addCurve(datas);
    cur->setPen(SARandColorMaker::getCurveColor()
                ,saConfig->getPlotCurWidth(datas.size()));
    cur->setTitle(strName);
    m_lastPlotItem = cur;
    return cur;
}
///
/// \brief 移除范围内数据
/// \param curves 需要移除的曲线列表
///
void SAFigureWindow::removeDataInRang(QList<QwtPlotCurve*> curves)
{
    GET_CHART_PTR_RETURN_VOID
    chart->setAutoReplot(false);
    QRectF rang = chart->getPlottingRegionRang();
    for(int i=0;i<curves.size();++i)
    {
        chart->removeDataInRang(rang,curves[i]);
    }
    chart->setAutoReplot(true);
}
///
/// \brief 设置窗体背景
/// \param brush
///
void SAFigureWindow::setBackgroundColor(const QBrush &brush)
{
    QPalette p = palette();
    p.setBrush(QPalette::Window,brush);
    setPalette(p);
}

void SAFigureWindow::setBackgroundColor(const QColor &clr)
{
    QPalette p = palette();
    p.setColor(QPalette::Window,clr);
    setPalette(p);
}
///
/// \brief 添加曲线
/// \param pC
///
void SAFigureWindow::addCurve(QwtPlotCurve *pC)
{
    GET_CHART_PTR_RETURN_VOID
    chart->addCurve(pC);
    m_realPlotItem.append(pC);
    QColor c = SARandColorMaker::getCurveColor();
    pC->setBrush(QBrush(c));
    calcDataFeature(chart,pC);
    m_lastPlotItem = pC;
}



///
/// \brief 记录曲线对应的数据
/// \param cur 曲线
/// \param datas 数据
///
void SAFigureWindow::rememberRelationShip(QwtPlotItem *cur, const QList<SAAbstractDatas *> &datas)
{
    m_relationShip[cur] = datas;
}
///
/// \brief 计算bar特征数据
/// \param plot 顶层指针
/// \param bar 数据线条的指针
/// \param emitSignal 计算完成之后是否发送信号，默认为true，
///
AbstractDataFeatureItem *SAFigureWindow::calcBarDataFeature(QwtPlotHistogram *bar)
{
    //TODO
    return nullptr;
}
///
/// \brief 计算并显示特征参数
///
/// 用户可以通过重载此函数来实现自己的配置。此函数默认会计算常见的统计参数
/// \param cur 曲线
/// \return AbstractDataFeatureItem指针
///
AbstractDataFeatureItem* SAFigureWindow::calcCureDataFeature(QwtPlotCurve *cur)
{
    CurveDataFeatureItem* curItem = new CurveDataFeatureItem(cur);
    return curItem;
}


void SAFigureWindow::initUI()
{

}
///
/// \brief 计算曲线的特征数据
/// 计算完成后会发射信号haveBeingCalcDataFeature
/// \param plot 曲线对应的窗体指针
/// \param item 曲线的指针
/// \see haveBeingCalcDataFeature
///
void SAFigureWindow::calcDataFeature(QWidget *plot,QwtPlotItem *item)
{
    //先查找已经创建的条目中是否包含此项
    DataFeaturePlotInfoItem* plotItem = findTopItem(plot);
    if(nullptr == plotItem)
    {//没有此项说明是新建的绘图，创建一个item
        plotItem = new DataFeaturePlotInfoItem();
        plotItem->setChartWidget(plot);
        m_dataFeature.append(plotItem);
    }
    if(item->rtti() == QwtPlotItem::Rtti_PlotCurve)
    {
        //如果对于的是曲线图
        AbstractDataFeatureItem* childItem = calcCureDataFeature(static_cast<QwtPlotCurve*>(item));
        plotItem->appendRow(childItem);
    }
    else if(item->rtti() == QwtPlotItem::Rtti_PlotHistogram)
    {
        //如果对于的是棒图
        AbstractDataFeatureItem* childItem = calcBarDataFeature(static_cast<QwtPlotHistogram*>(item));
        plotItem->appendRow(childItem);
    }
    emit haveBeingCalcDataFeature(plot,item);
}
///
/// \brief 根据plot指针寻找顶层item
/// \param plot 绘图窗体
/// \return 如果没有对应的返回nullptr
///
DataFeaturePlotInfoItem *SAFigureWindow::findTopItem(QWidget *plot) const
{
    auto i = m_dataFeature.cbegin();
    auto end = m_dataFeature.cend();
    for(;i!=end;++i)
    {
        AbstractDataFeatureItem * top = const_cast<AbstractDataFeatureItem *>(*i);
        if(nullptr == top)
        {
            continue;
        }
        if(top->rtti() == AbstractDataFeatureItem::TopPlotItem)
        {
            DataFeaturePlotInfoItem* plotItem = static_cast<DataFeaturePlotInfoItem*>(top);
            if(nullptr == plotItem)
            {
                continue;
            }
            if(plotItem->chartWidget() == plot)
            {
                return plotItem;
            }
        }
    }
    return nullptr;
}


///
/// \brief SAFigureWindow::RealPlotItem::append
/// \param item
///
void SAFigureWindow::RealPlotItem::append(QwtPlotItem *item)
{
    m_plotItem.append(item);
}
