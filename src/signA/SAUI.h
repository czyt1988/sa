#ifndef SAUI_H
#define SAUI_H
#include "SAUIInterface.h"


class MainWindow;
class QwtPlotCurve;

class SAUI : public SAUIInterface
{
    Q_OBJECT
public:
    SAUI(MainWindow* mainWnd);
    ~SAUI();
    friend class MainWindow;
public:
    //获取主界面指针
    virtual QWidget *getMainWindowPtr();
    //如果插件只导入到data import菜单下的条目可以调用此函数，如果需要插入一个QMenu可以使用addDataImportPluginAction
    virtual void addDataImportPluginAction(QAction *action);
    //添加导出数据插件的响应槽
    virtual QAction* addDataImportPluginMenu(QMenu *menu);
    //把菜单添加到分析功能的菜单中
    virtual QAction *addAnalysisPluginMenu(QMenu *menu);
#if SA_USE_RIBBON_UI
    ///
    /// \group Ribbon相关操作
    ///
    //把action加入 ribbon界面的Gallery
    virtual void addAnalysisActionsToRibbonGallery(const QString& name,const QList<QAction*>& actions);
#endif
public slots:
    /// \group 进度栏 消息窗口的相关操作
    /// \{
    //在ui界面显示普通信息
    void showNormalMessageInfo(const QString& info,int interval = -1);
    //在ui界面显示错误信息
    void showErrorMessageInfo(const QString& info,int interval = -1);
    //在ui界面显示警告信息
    void showWarningMessageInfo(const QString& info,int interval = -1);
    //在ui界面显示询问信息
    void showQuestionMessageInfo(const QString& info,int interval = -1);
    //显示消息
    void showMessageInfo(const QString& info,SA::MeaasgeType messageType);
    //接收子窗口发送过来的消息
    void showWidgetMessageInfo(const QString& info,QWidget *widget,SA::MeaasgeType messageType,int interval);
    //显示状态栏延时信息
    void showElapesdMessageInfo(const QString& info
                        ,SA::MeaasgeType type =  SA::NormalMessage,int interval = 4000);
    //隐藏状态栏的进度条
    void hideProgressStatusBar();
    //显示状态栏的进度条
    void showProgressStatusBar();
    //设置状态栏进度条的可见性
    void setProgressStatusBarVisible(bool isShow);
    //设置状态栏上的进度显示的进度状的百分比
    void setProgressStatusBarPresent(int present);
    //设置进度条状态栏上的文字
    void setProgressStatusBarText(const QString& text);
    //获取进度栏上的进度条指针
    QProgressBar* getProgressStatusBar();
    /// \}

public:
    ///
    /// \group 数据选择
    /// \{
    //获取选中的数据条目
    QList<SAAbstractDatas*> getSeletedDatas() const;
    //获取选中的数据条目
    SAAbstractDatas* getSeletedData() const;
    //获取选中的数据条目，如果没有选中，将弹出数据选择窗口让用户进行选择
    SAAbstractDatas* getSelectSingleData(bool isAutoSelect = false);
    /// \}

    /// 子窗口相关操作
    /// \group 子窗口相关操作
    /// \{
    //创建一个绘图窗口
    QMdiSubWindow* createFigureWindow(const QString& title = QString());
    //获取最后显示的绘图窗口的指针
    SAFigureWindow* getCurrentFigureWindow();
    //获取所有的figure
    QList<SAFigureWindow*> getFigureWindowList() const;
    //获取当前正在显示的Chart指针
    SAChart2D* getCurSubWindowChart();
    //获取当前正在显示的Chart指针,如果是一个subplot，返回多个指针
    QList<SAChart2D *> getCurSubWindowCharts();
    //用于子窗口激活时刷新“图表设置工具栏的选中状态”
    void updateChartSetToolBar(SAFigureWindow* w);
    //获取所有子窗口指针
    QList<QMdiSubWindow*> getSubWindowList() const;
    //记录最后获取焦点的窗口类型，此函数主要用于函数功能模块判断是对图进行操作还是对数据进行操作
    LastFocusType lastFocusWidgetType() const;
    /// \}

    /// \group 子窗口的相关控制
    /// \{
    //获取当前激活的子窗口
    QMdiSubWindow* getCurrentActiveSubWindow() const;
    //判断mdi中是否存在指定的子窗口
    bool isHaveSubWnd(QMdiSubWindow* wndToCheck) const;
    /// \}

    ///
    /// \group 几个dock窗口的控制
    /// \{
    //把主dock抬起，主dock包括绘图的窗口
    void raiseMainDock();
    //把信息窗口抬起
    void raiseMessageInfoDock();
    //让ValueViewerDock显示到最前面
    void raiseValueViewerDock();
    //让DataFeatureDock显示到最前面
    void raiseDataFeatureDock();
    //让windowList显示到最前面
    void raiseWindowListDock();
    //让valueManageDock显示到最前面
    void raiseValueManageDock();
    //让ChartLayoutDock显示到最前面
    void raiseChartLayoutDock();
    //让chartDataViewerDock显示到最前面
    void raiseChartDataViewerDock();
    //让ChartSettingDock显示到最前面
    void raiseChartSettingDock();
    /// \}

    ///
    /// \group 绘图窗口的控制
    /// \{
    //选择曲线
    virtual QList<QwtPlotItem*> selectPlotItems(SAChart2D* chart,const QSet<int>& filter);
    /// \}
protected:
    void onSelectDataChanged(SAAbstractDatas* dataPtr);
private:
    MainWindow* m_mainWnd;
};

#endif // SAUI_H
