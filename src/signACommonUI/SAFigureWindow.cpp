#include "SAFigureWindow.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QAction>
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

class SAFigureWindowPrivate
{
    SA_IMPL_PUBLIC(SAFigureWindow)
public:
    SAFigureContainer *centralwidget;
    SAChart2D *currentPlot;
    SAFigureWindowPrivate(SAFigureWindow* p):q_ptr(p)
      ,centralwidget(nullptr)
      ,currentPlot(nullptr)
    {

    }

    void setupUI()
    {
        centralwidget = new SAFigureContainer(q_ptr);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        q_ptr->setCentralWidget(centralwidget);
        q_ptr->setAcceptDrops(true);
        q_ptr->setAutoFillBackground(true);
        q_ptr->setBackgroundColor(QColor(255,255,255));
    }

    void retranslateUi()
    {
        q_ptr->setWindowTitle(QApplication::translate("SAFigureWindow", "Figure", 0));
    }
};

//=============================================================================

SAFigureWindow::SAFigureWindow(QWidget *parent) :
    QMainWindow(parent)
    ,d_ptr(new SAFigureWindowPrivate(this))
{
    d_ptr->setupUI();
    static int s_figure_count=0;
    ++s_figure_count;
    setWindowTitle(QString("figure-%1").arg(s_figure_count));
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
        SAChart2D* plot = new SAChart2D(d_ptr->centralwidget);
        d_ptr->centralwidget->addWidget (plot,xPresent,yPresent,wPresent,hPresent);
        d_ptr->currentPlot = plot;
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
    QList<QWidget *> widgets = d_ptr->centralwidget->getWidgetList();
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
    return d_ptr->currentPlot;
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

void SAFigureWindow::redo()
{
    SAChart2D * c = current2DPlot();
    if(c)
    {
        qDebug() << "void SAFigureWindow::redo()";
        c->redo();
    }
}

void SAFigureWindow::undo()
{
    SAChart2D * c = current2DPlot();
    if(c)
    {
        qDebug() << "void SAFigureWindow::undo()";
        c->undo();
    }
}
#if 0
void SAFigureWindow::keyPressEvent(QKeyEvent *e)
{
    qDebug() << e->type() << e->key() << " " << e->modifiers();
    if(Qt::ControlModifier & e->modifiers())
    {
        if(Qt::Key_Z == e->key())
        {
            undo();
        }
        else if(Qt::Key_Y == e->key())
        {
            redo();
        }
    }
    QMainWindow::keyPressEvent(e);
}
#endif







