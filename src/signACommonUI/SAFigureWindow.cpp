#include "SAFigureWindow.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QGridLayout>
//sa chart
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




void SAFigureWindow::initUI()
{

}



