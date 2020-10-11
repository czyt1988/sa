#include "SAFigureWindow.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QAction>
#include <QMimeData>
#include <QPaintEvent>
#include <QCoreApplication>
#include <QScopedPointer>
#include <QChildEvent>
#include <QCursor>
#include <QPainter>
//sa chart
#include "SAChart2D.h"
#include "SAQwtSerialize.h"
//sa lib
#include "SAData.h"
#include "SARandColorMaker.h"
#include "SAValueManager.h"
#include "SAValueManagerMimeData.h"
//sa common ui
#include "SAFigureOptCommands.h"
#include "SAFigureContainer.h"
#include "SAFigureChartRubberbandEditOverlay.h"
#define GET_CHART_PTR			    \
    SAChart2D *chart = current2DPlot(); \
    if (nullptr == chart)		    \
    {				    \
        return (nullptr);	    \
    }

#define GET_CHART_PTR_RETURN_VOID	    \
    SAChart2D *chart = current2DPlot(); \
    if (nullptr == chart)		    \
    {				    \
        return;			    \
    }



class SAFigureWindowPrivate
{
    SA_IMPL_PUBLIC(SAFigureWindow)
public:
    SAFigureContainer *centralwidget;
    SAChart2D *currentPlot;
    SAFigureChartRubberbandEditOverlay *chartRubberbandEditor;      ///< 编辑模式
    QUndoStack redoUndoStack;
    QBrush backgroundBrush;                                         ///< 背景

    SAFigureWindowPrivate(SAFigureWindow *p) : q_ptr(p)
        , centralwidget(nullptr)
        , currentPlot(nullptr)
        , chartRubberbandEditor(nullptr)
    {
    }


    void setupUI()
    {
        centralwidget = new SAFigureContainer(q_ptr);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        q_ptr->setCentralWidget(centralwidget);
        q_ptr->setAcceptDrops(true);
        q_ptr->setBackgroundColor(QColor(255, 255, 255));
        q_ptr->setWindowIcon(QIcon(":/icon/icons/figureWindow.png"));
    }


    void retranslateUi()
    {
        q_ptr->setWindowTitle(QApplication::translate("SAFigureWindow", "Figure", 0));
    }
};

//=============================================================================

SAFigureWindow::SAFigureWindow(QWidget *parent) :
    SAMainWindow(parent)
    , d_ptr(new SAFigureWindowPrivate(this))
{
    askOnCloseEvent(false);
    d_ptr->setupUI();
    setFocusPolicy(Qt::ClickFocus);
    static int s_figure_count = 0;

    ++s_figure_count;
    setWindowTitle(QString("figure-%1").arg(s_figure_count));
    setMinimumWidth(100);
    setMinimumHeight(50);
}


SAFigureWindow::~SAFigureWindow()
{
    //qDebug() << "SAFigureWindow destroy";
}


QList<QWidget *> SAFigureWindow::getWidgets() const
{
    return (d_ptr->centralwidget->getWidgetList());
}


///
/// \brief 添加一个2Dchart
/// \return 返回2D绘图的指针
///
SAChart2D *SAFigureWindow::create2DPlot()
{
    return (create2DSubPlot(0.05f, 0.05f, 0.9f, 0.9f));
}


SAChart2D *SAFigureWindow::create2DSubPlot(float xPresent, float yPresent, float wPresent, float hPresent)
{
    try
    {
#if 0
        SAChart2D *chart = new SAChart2D(d_ptr->centralwidget);
        d_ptr->centralwidget->addWidget(chart, xPresent, yPresent, wPresent, hPresent);
        d_ptr->currentPlot = chart;
        emit chartAdded(chart);
        setFocusProxy(chart);
#else
        SAChart2D *chart = new SAChart2D;
        SAFigureCreateSubWidgetCommand *cmd = new SAFigureCreateSubWidgetCommand(this
            , chart
            , xPresent, yPresent, wPresent, hPresent
            , tr("create 2D chart")
            );
        chart->attachToFigure(this);
        appendCommand(cmd);
        d_ptr->currentPlot = chart;
        emit chartAdded(chart);
        setFocusProxy(chart);
#endif
        return (chart);
    }
    catch (const std::bad_alloc& b)
    {
        Q_UNUSED(b);
        QMessageBox::warning(this, tr("memory out"), tr("memory out"));
        return (nullptr);
    }
    return (nullptr);
}


///
/// \brief 添加窗口到fig中，此操作不是命令模式，不支持redo/undo
/// \param w 窗体指针，如果父窗口不是fig，会改变为fig
/// \param xPresent
/// \param yPresent
/// \param wPresent
/// \param hPresent
///
void SAFigureWindow::_addWidget(QWidget *w, float xPresent, float yPresent, float wPresent, float hPresent)
{
    if (w->parentWidget() != d_ptr->centralwidget) {
        w->setParent(d_ptr->centralwidget);
    }
    d_ptr->centralwidget->addWidget(w, xPresent, yPresent, wPresent, hPresent);
}


///
/// \brief 获取所有的图像列表
/// \return
///
QList<SAChart2D *> SAFigureWindow::get2DPlots() const
{
    QList<SAChart2D *> res;
    QList<QWidget *> widgets = d_ptr->centralwidget->getWidgetList();

    for (auto i = widgets.begin(); i != widgets.end(); ++i)
    {
        SAChart2D *chart = qobject_cast<SAChart2D *>(*i);
        if (chart) {
            res.append(chart);
        }
    }
    return (res);
}


///
/// \brief 当前的2d绘图指针
/// \return 当没有2d绘图时返回nullptr
///
SAChart2D *SAFigureWindow::current2DPlot() const
{
    return (d_ptr->currentPlot);
}


void SAFigureWindow::clearAll()
{
    QList<SAChart2D *> charts = get2DPlots();

    while (!charts.isEmpty())
    {
        SAChart2D *p = charts.takeLast();
        emit chartWillRemove(p);
        delete p;
    }
}


///
/// \brief 设置窗体背景
/// \param brush
///
void SAFigureWindow::setBackgroundColor(const QBrush& brush)
{
    d_ptr->backgroundBrush = brush;
}


/**
 * @brief 设置窗体背景
 * @param clr
 */
void SAFigureWindow::setBackgroundColor(const QColor& clr)
{
    d_ptr->backgroundBrush.setStyle(Qt::SolidPattern);
    d_ptr->backgroundBrush.setColor(clr);
}


/**
 * @brief 获取背景颜色
 * @return
 */
const QBrush& SAFigureWindow::getBackgroundColor() const
{
    return (d_ptr->backgroundBrush);
}


///
/// \brief 获取窗口的位置
/// \param w
/// \return
///
QRectF SAFigureWindow::getWidgetPos(QWidget *w) const
{
    return (d_ptr->centralwidget->getWidgetPos(w));
}


///
/// \brief 设置当前的2dplot
/// \param p
///
bool SAFigureWindow::setCurrent2DPlot(SAChart2D *p)
{
    if (p == d_ptr->currentPlot) {
        return (false);
    }
    if (!d_ptr->centralwidget->isWidgetInContainer(p)) {
        return (false);
    }
    d_ptr->currentPlot = p;
    //setFocusProxy(p);
    //如果在进行子窗口编辑模式，此时需要重新设置编辑
    if (isSubWindowEditingMode()) {
        enableSubWindowEditMode(false);
        enableSubWindowEditMode(true);
    }
    emit currentWidgetChanged(p);

    return (true);
}


///
/// \brief 通过item查找对应的SAChart2D，如果没有返回nullptr
/// \param item
/// \return 如果没有返回nullptr
///
SAChart2D *SAFigureWindow::findChartFromItem(QwtPlotItem *item)
{
    QList<SAChart2D *> charts = get2DPlots();
    const int chartSize = charts.size();

    for (int i = 0; i < chartSize; ++i)
    {
        QwtPlotItemList items = charts[i]->itemList();
        if (items.contains(item)) {
            return (charts[i]);
        }
    }
    return (nullptr);
}


///
/// \brief 是否开始子窗口编辑模式
/// \param enable
/// \param ptr 通过此参数可以指定自定义的编辑器，若为nullptr，将使用默认的编辑器，此指针的管理权将移交SAFigureWindow
///
void SAFigureWindow::enableSubWindowEditMode(bool enable, SAFigureChartRubberbandEditOverlay *ptr)
{
    if (enable) {
        if (nullptr == d_ptr->chartRubberbandEditor) {
            if (ptr) {
                d_ptr->chartRubberbandEditor = ptr;
                d_ptr->chartRubberbandEditor->show();
            }else {
                d_ptr->chartRubberbandEditor = new SAFigureChartRubberbandEditOverlay(this);
                d_ptr->chartRubberbandEditor->show();
            }
        }else {
            if (d_ptr->chartRubberbandEditor->isHidden()) {
                d_ptr->chartRubberbandEditor->show();
            }
        }
    }else {
        if (d_ptr->chartRubberbandEditor) {
            delete d_ptr->chartRubberbandEditor;
            d_ptr->chartRubberbandEditor = nullptr;
        }
    }
}


///
/// \brief 获取子窗口编辑器指针，若没有此编辑器，返回nullptr
///
/// 此指针的管理权在SAFigureWindow上，不要在外部对此指针进行释放
/// \return
///
SAFigureChartRubberbandEditOverlay *SAFigureWindow::subWindowEditModeOverlayWidget() const
{
    return (d_ptr->chartRubberbandEditor);
}


/**
 * @brief SAFigureWindow::isSubWindowEditingMode
 * @return
 */
bool SAFigureWindow::isSubWindowEditingMode() const
{
    SAFigureChartRubberbandEditOverlay *b = subWindowEditModeOverlayWidget();

    if (b) {
        return (b->isVisible());
    }
    return (false);
}


///
/// \brief 使用支持redo/undo模式的改变子窗口大小
/// \param p 子窗口
/// \param rect 子窗口相对figure的大小
///
void SAFigureWindow::resizeWidget(QWidget *p, const QRect& newRect, const QRect& oldRect)
{
    SAFigureSubChartResizeCommand *resizeCmd;

    if (oldRect.isNull()) {
        resizeCmd = new SAFigureSubChartResizeCommand(this, p, oldRect, newRect, tr("resize"));
    }else {
        resizeCmd = new SAFigureSubChartResizeCommand(this, p, newRect, tr("resize"));
    }
    appendCommand(resizeCmd);
}


void SAFigureWindow::appendCommand(SAFigureOptCommand *cmd)
{
    d_ptr->redoUndoStack.push(cmd);
}


///
/// \brief 删除所有的命令
///
void SAFigureWindow::clearUndoCommand()
{
    d_ptr->redoUndoStack.clear();
}


///
/// \brief 判断命令是否是空的
/// \return
///
bool SAFigureWindow::isUndoCommandClean() const
{
    return (d_ptr->redoUndoStack.isClean());
}


///
/// \brief 返回当前光标下的widget
/// \return 如果当前没有返回nullptr
///
QWidget *SAFigureWindow::cursorWidget() const
{
    QPoint p = d_ptr->centralwidget->mapFromGlobal(QCursor::pos());

    return (d_ptr->centralwidget->childAt(p));
}


///
/// \brief 返回在当前光标下的2D图
/// \return 如果当前没有返回nullptr
///
SAChart2D *SAFigureWindow::cursor2DChart() const
{
    //return qobject_cast<SAChart2D *>(cursorWidget());
    QList<SAChart2D *> charts = get2DPlots();

    for (int i = 0; i < charts.size(); ++i)
    {
        if (charts[i]->underMouse()) {
            return (charts[i]);
        }
    }
    return (nullptr);
}


//void SAFigureWindow::dragEnterEvent(QDragEnterEvent *event)
//{
//    qDebug() << "SAFigureWindow dragEnterEvent mimeData:"<<event->mimeData()->formats();
//    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
//    {
//        //event->setDropAction(Qt::MoveAction);
//        event->acceptProposedAction();
//        qDebug() << "SAFigureWindow dragEnterEvent acceptProposedAction";
//        return;
//    }
////    else
////    {
////        qDebug() << "SAFigureWindow dragEnterEvent ignore";
////        event->ignore();
////    }
//    QMainWindow::dragEnterEvent(event);
//}

//void SAFigureWindow::dragMoveEvent(QDragMoveEvent *event)
//{
//    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
//    {
////        event->setDropAction(Qt::MoveAction);
////        event->accept();
//        event->acceptProposedAction();
//        qDebug() << "SAFigureWindow dragMoveEvent acceptProposedAction";
//        return;
//    }
//    QMainWindow::dragMoveEvent(event);
//}

//void SAFigureWindow::dropEvent(QDropEvent *event)
//{
//    if(event->mimeData()->hasFormat(SAValueManagerMimeData::valueIDMimeType()))
//    {
//        qDebug() << "SAFigureWindow::dropEvent";
//        QList<int> ids;
//        if(SAValueManagerMimeData::getValueIDsFromMimeData(event->mimeData(),ids))
//        {
//            QList<SAAbstractDatas*> datas = saValueManager->findDatas(ids);
//            if(SAChart2D * c = current2DPlot())
//            {
//                for(int i=0;i<datas.size();++i)
//                {
//                    c->addCurve(datas[i]);
//                }
//            }
//        }
//    }
//}

///
/// \brief redo
///
void SAFigureWindow::redo()
{
    d_ptr->redoUndoStack.redo();
}


///
/// \brief undo
///
void SAFigureWindow::undo()
{
    d_ptr->redoUndoStack.undo();
}


///
/// \brief 清空回退栈
///
void SAFigureWindow::clearRedoUndoStack()
{
    d_ptr->redoUndoStack.clear();
}


/**
 * @brief SAFigureWindow::paintEvent
 * @param e
 */
void SAFigureWindow::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    p.setBrush(d_ptr->backgroundBrush);
    p.fillRect(0, 0, width(), height(), d_ptr->backgroundBrush);
    SAMainWindow::paintEvent(e);
}


#if 0
void SAFigureWindow::keyPressEvent(QKeyEvent *e)
{
    qDebug() << e->type() << e->key() << " " << e->modifiers();
    if (Qt::ControlModifier& e->modifiers()) {
        if (Qt::Key_Z == e->key()) {
            undo();
        }else if (Qt::Key_Y == e->key()) {
            redo();
        }
    }
    QMainWindow::keyPressEvent(e);
}


#endif



QDataStream& operator <<(QDataStream& out, const SAFigureWindow *p)
{
    const int magicStart = 0x1314abc;

    out	<< magicStart
        << p->saveGeometry()
        << p->saveState()
    ;
    QList<SAChart2D *> charts = p->get2DPlots();
    QList<QRectF> pos;

    for (int i = 0; i < charts.size(); ++i)
    {
        pos.append(p->getWidgetPos(charts[i]));
    }
    out << pos;
    for (int i = 0; i < charts.size(); ++i)
    {
        out << charts[i];
    }
    return (out);
}


QDataStream& operator >>(QDataStream& in, SAFigureWindow *p)
{
    const int magicStart = 0x1314abc;
    int tmp;

    in >> tmp;
    if (tmp != magicStart) {
        throw sa::SABadSerializeExpection();
        return (in);
    }
    QByteArray geometryData, stateData;

    in >> geometryData
    >> stateData
    ;
    p->restoreGeometry(geometryData);
    p->restoreState(stateData);
    QList<QRectF> pos;

    in >> pos;
    try
    {
        for (int i = 0; i < pos.size(); ++i)
        {
            const QRectF& r = pos[i];
            QScopedPointer<SAChart2D> chart(p->create2DSubPlot(r.x(), r.y(), r.width(), r.height()));
            in >> chart.data();
            chart->show();
            chart.take();
        }
    }
    catch (const sa::SABadSerializeExpection& exp)
    {
        throw exp;
    }
    return (in);
}
