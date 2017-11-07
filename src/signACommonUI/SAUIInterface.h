#ifndef SAUIINTERFACE_H
#define SAUIINTERFACE_H
//Qt
#include <QObject>
#include <QVariantList>
#include <QHash>

#include "SACommonUIGlobal.h"
#include "SAGlobals.h"
class QWidget;
class QAction;
class QMenu;
class QMdiSubWindow;
class QProgressBar;
//qwt
class QwtPlotCurve;
//SA
class MainWindow;
//SA Lib
class SAAbstractDatas;
//SA Chart
class SAChart2D;
//SA Common UI
class SAFigureWindow;
class SA_COMMON_UI_EXPORT SAUIInterface : public QObject
{
    Q_OBJECT
public:
    SAUIInterface();
    virtual ~SAUIInterface();
public:
    //获取主界面指针
    virtual QWidget *getMainWindowPtr() = 0;
    //如果插件只导入到data import菜单下的条目可以调用此函数，如果需要插入一个QMenu可以使用addDataImportPluginAction
    virtual void addDataImportPluginAction(QAction *action) = 0;
    //添加导出数据插件的响应槽
    virtual QAction* addDataImportPluginMenu(QMenu *menu) = 0;
    //把菜单添加到分析功能的菜单中
    virtual QAction *addAnalysisPluginMenu(QMenu *menu) = 0;
#if SA_USE_RIBBON_UI
    ///
    /// \group Ribbon相关操作
    ///
    virtual void addAnalysisActionsToRibbonGallery(const QString& name,const QList<QAction*>& actions) = 0;
#endif
public slots:

    /// \group 进度栏 消息窗口的相关操作
    /// \{
    //在ui界面显示普通信息
    virtual void showNormalMessageInfo(const QString& info,int interval = -1) = 0;
    //在ui界面显示错误信息
    virtual void showErrorMessageInfo(const QString& info,int interval = -1) = 0;
    //在ui界面显示警告信息
    virtual void showWarningMessageInfo(const QString& info,int interval = -1) = 0;
    //在ui界面显示询问信息
    virtual void showQuestionMessageInfo(const QString& info,int interval = -1) = 0;
    //显示消息
    virtual void showMessageInfo(const QString& info,SA::MeaasgeType messageType) = 0;
    //接收子窗口发送过来的消息
    virtual void showWidgetMessageInfo(const QString& info,QWidget *widget,SA::MeaasgeType messageType,int interval) = 0;
    //显示状态栏延时信息
    virtual void showElapesdMessageInfo(const QString& info,SA::MeaasgeType type =  SA::NormalMessage,int interval = 4000) = 0;

    //隐藏状态栏的进度条
    virtual void hideProgressStatusBar() = 0;
    //显示状态栏的进度条
    virtual void showProgressStatusBar() = 0;
    //设置状态栏进度条的可见性
    virtual void setProgressStatusBarVisible(bool isShow) = 0;
    //设置状态栏上的进度显示的进度状的百分比
    virtual void setProgressStatusBarPresent(int present) = 0;
    //设置进度条状态栏上的文字
    virtual void setProgressStatusBarText(const QString& text) = 0;
    //获取进度栏上的进度条指针
    virtual QProgressBar* getProgressStatusBar() = 0;
     /// \}
public:
    ///
    /// \group 数据选择
    /// \{
    //获取选中的数据条目
    virtual QList<SAAbstractDatas*> getSeletedDatas() const = 0;
    //获取选中的数据条目
    virtual SAAbstractDatas* getSeletedData() const = 0;
    //获取选中的数据条目，如果没有选中，将弹出数据选择窗口让用户进行选择
    virtual SAAbstractDatas* getSelectSingleData(bool isAutoSelect = false) = 0;
    /// \}

    ///
    /// \group 子窗口相关操作
    /// \{
    //创建一个绘图窗口
    virtual QMdiSubWindow* createFigureWindow(const QString& title = QString()) = 0;
    //获取最后显示的绘图窗口的指针
    virtual SAFigureWindow* getCurrentFigureWindow() = 0;
    //获取所有的figure
    virtual QList<SAFigureWindow*> getFigureWindowList() const = 0;
    //获取当前正在显示的Chart指针
    virtual SAChart2D* getCurSubWindowChart() = 0;
    //获取当前正在显示的Chart指针,如果是一个subplot，返回多个指针
    virtual QList<SAChart2D *> getCurSubWindowCharts() = 0;
    //用于子窗口激活时刷新“图表设置工具栏的选中状态”
    virtual void updateChartSetToolBar(SAFigureWindow* w) = 0;
    //获取所有子窗口指针
    virtual QList<QMdiSubWindow*> getSubWindowList() const = 0;
    //从subwindow指针中查找是否含有SAFigureWindow
    static SAFigureWindow* getFigureWidgetFromMdiSubWindow(QMdiSubWindow* sub);
    //获取当前激活的子窗口
    virtual QMdiSubWindow* getCurrentActiveSubWindow() const = 0;
    //判断mdi中是否存在指定的子窗口
    virtual bool isHaveSubWnd(QMdiSubWindow* wndToCheck) const = 0;
    /// \}

    ///
    /// \group 几个dock窗口的控制
    /// \{
    //把主dock抬起，主dock包括绘图的窗口
    virtual void raiseMainDock() = 0;
    //把信息窗口抬起
    virtual void raiseMessageInfoDock() = 0;
    /// \}

    ///
    /// \group 绘图窗口的控制
    /// \{
    //选择曲线
    virtual QList<QwtPlotCurve*> selectCurves(SAChart2D* chart) = 0;
    /// \}
    ///


signals:
    ///
    /// \brief 在主界面变量管理树选择发生改变后会发射此信号
    /// \param dataPtr 选中的数据指针，也可以通过 \a getSelectDatas 获取多选状态下的指针
    ///
    void selectDataChanged(SAAbstractDatas* dataPtr);
    ///
    /// \brief 子窗口激活
    /// \param arg1
    ///
    void mdiAreaSubWindowActivated(QMdiSubWindow *arg1);

};

#endif // SAUIINTERFACE_H
