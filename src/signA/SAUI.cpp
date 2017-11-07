#include "SAUI.h"
#include <QDebug>
#include <QMenu>
#include <QMdiSubWindow>
#include <QMainWindow>
#include "SAFigureWindow.h"
#include "mainwindow.h"
#include "CurveSelectDialog.h" //图线选择窗口

SAUI::SAUI(MainWindow* mainWnd)
    :m_mainWnd(mainWnd)
{

}

SAUI::~SAUI()
{

}

QList<SAAbstractDatas *> SAUI::getSeletedDatas() const
{
    return m_mainWnd->getSeletedDatas();
}

SAAbstractDatas *SAUI::getSeletedData() const
{
    return m_mainWnd->getSeletedData();
}

SAAbstractDatas *SAUI::getSelectSingleData(bool isAutoSelect)
{
    return m_mainWnd->getSelectSingleData(isAutoSelect);
}

QWidget *SAUI::getMainWindowPtr()
{
    return m_mainWnd;
}
///
/// \brief 如果插件只导入到data import菜单下的条目可以调用此函数，如果需要插入一个QMenu可以使用addDataImportPluginAction
/// \param action
///
void SAUI::addDataImportPluginAction(QAction *action)
{
    m_mainWnd->addDataImportPluginAction(action);
}
///
/// \brief 添加导入数据插件菜单
/// \param menu
/// \return
///
QAction *SAUI::addDataImportPluginMenu(QMenu *menu)
{
    return m_mainWnd->addDataImportPluginMenu(menu);
}
///
/// \brief 把菜单添加到分析功能的菜单中
/// \param menu
/// \return
///
QAction *SAUI::addAnalysisPluginMenu(QMenu *menu)
{
    return m_mainWnd->addAnalysisPluginMenu(menu);
}
///
/// \brief SAUI::addAnalysisActionsToRibbonGallery
/// \param name
/// \param actions
///
void SAUI::addAnalysisActionsToRibbonGallery(const QString &name, const QList<QAction *> &actions)
{
    m_mainWnd->addAnalysisActionsToRibbonGallery(name,actions);
}

void SAUI::showNormalMessageInfo(const QString &info, int interval)
{
    m_mainWnd->showNormalMessageInfo(info,interval);
}

void SAUI::showErrorMessageInfo(const QString &info, int interval)
{
    m_mainWnd->showErrorMessageInfo(info,interval);
}

void SAUI::showWarningMessageInfo(const QString &info, int interval)
{
    m_mainWnd->showWarningMessageInfo(info,interval);
}

void SAUI::showQuestionMessageInfo(const QString &info, int interval)
{
    m_mainWnd->showQuestionMessageInfo(info,interval);
}

void SAUI::showMessageInfo(const QString &info, SA::MeaasgeType messageType)
{
    m_mainWnd->showMessageInfo(info,messageType);
}

void SAUI::showWidgetMessageInfo(const QString &info, QWidget *widget, SA::MeaasgeType messageType, int interval)
{
    m_mainWnd->showWidgetMessageInfo(info,widget,messageType,interval);
}

void SAUI::showElapesdMessageInfo(const QString &info, SA::MeaasgeType type, int interval)
{
    m_mainWnd->showElapesdMessageInfo(info,type,interval);
}

void SAUI::hideProgressStatusBar()
{
    m_mainWnd->hideProgressStatusBar();
}

void SAUI::showProgressStatusBar()
{
    m_mainWnd->showProgressStatusBar();
}

void SAUI::setProgressStatusBarVisible(bool isShow)
{
    m_mainWnd->setProgressStatusBarVisible(isShow);
}

void SAUI::setProgressStatusBarPresent(int present)
{
    m_mainWnd->setProgressStatusBarPresent(present);
}

void SAUI::setProgressStatusBarText(const QString &text)
{
    m_mainWnd->setProgressStatusBarText(text);
}

QProgressBar *SAUI::getProgressStatusBar()
{
    return m_mainWnd->getProgressStatusBar();
}

QMdiSubWindow *SAUI::createFigureWindow(const QString &title)
{
    return m_mainWnd->createFigureWindow(title);
}

SAFigureWindow *SAUI::getCurrentFigureWindow()
{
    return m_mainWnd->getCurrentFigureWindow();
}

QList<SAFigureWindow *> SAUI::getFigureWindowList() const
{
    return m_mainWnd->getFigureWindowList();
}

SAChart2D *SAUI::getCurSubWindowChart()
{
    return m_mainWnd->getCurSubWindowChart();
}

QList<SAChart2D *> SAUI::getCurSubWindowCharts()
{
    return m_mainWnd->getCurSubWindowCharts();
}

void SAUI::updateChartSetToolBar(SAFigureWindow *w)
{
    m_mainWnd->updateChartSetToolBar(w);
}

QList<QMdiSubWindow *> SAUI::getSubWindowList() const
{
    return m_mainWnd->getSubWindowList();
}

QMdiSubWindow *SAUI::getCurrentActiveSubWindow() const
{
    return m_mainWnd->getCurrentActiveSubWindow();
}

bool SAUI::isHaveSubWnd(QMdiSubWindow *wndToCheck) const
{
    return m_mainWnd->isHaveSubWnd(wndToCheck);
}

void SAUI::raiseMainDock()
{
    m_mainWnd->raiseMainDock();
}

void SAUI::raiseMessageInfoDock()
{
    m_mainWnd->raiseMessageInfoDock();
}

QList<QwtPlotCurve *> SAUI::selectCurves(SAChart2D* chart)
{
    QList<QwtPlotCurve *> res;
    if(nullptr == chart)
    {
        return res;
    }
    QList<QwtPlotCurve*> curves = CurveSelectDialog::getSelCurve(chart,getMainWindowPtr ());
    return curves;
}

void SAUI::onSelectDataChanged(SAAbstractDatas *dataPtr)
{
    emit selectDataChanged(dataPtr);
}
