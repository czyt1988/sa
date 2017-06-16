#include "SAFigureWindow.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QGridLayout>
//sa chart
#include "DataFeatureItem.h"
#include "SAChart2D.h"
//sa lib
#include "SAData.h"
#include "SARandColorMaker.h"
#include "SAFigureGlobalConfig.h"
//sa common ui
#include "SAFigureContainer.h"
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
    centralwidget = new SAFigureContainer(par);
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
  ,m_currentPlot(nullptr)
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
    return create2DSubPlot(0.05,0.05,0.9,0.9);
}

SAChart2D *SAFigureWindow::create2DSubPlot(float xPresent, float yPresent, float wPresent, float hPresent)
{
    try
    {
        SAChart2D* plot = new SAChart2D(ui->centralwidget);
        ui->centralwidget->addWidget (plot,xPresent,yPresent,wPresent,hPresent);
        m_currentPlot = plot;
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
    QList<QWidget *> widgets = ui->centralwidget->getWidgetList();
    for(auto i=widgets.begin();i!=widgets.end();++i)
    {
        SAChart2D* chart = qobject_cast<SAChart2D*>(*i);
        if(chart)
        {
            res.append(chart);
        }
    }
    return res;
}

///
/// \brief 当前的2d绘图指针
/// \return 当没有2d绘图时返回nullptr
///
SAChart2D *SAFigureWindow::current2DPlot() const
{
    return m_currentPlot;
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
