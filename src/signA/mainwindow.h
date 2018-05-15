#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "../global/SAGlobals.h"
#ifdef SA_USE_RIBBON_UI
#include "SARibbonMainWindow.h"

#define _CFG_LAYOUT_SELECT_CHANG_QSS 0

#include <QScopedPointer>
class MainWindowPrivate;
#else
namespace Ui {
class MainWindow;
}
#endif
#include <memory>
#include <QMainWindow>
//#include "SAChartSupport.h"
#include <QMdiSubWindow>
#include <QMap>
#include "SAUI.h"

#include <QScopedPointer>
#include <QStandardItemModel>
#include <QIcon>
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QSettings>
#include "SAResourDefine.h"
#include <SAMdiSubWindow.h>

#include "progressStateWidget.h"


#include <SAMdiSubWindowManager.h>

#include "SADrawDelegate.h"

#include <SAInformationStatusWidget.h>
#include <tuple>
#include "SAPluginManager.h"
#include "SAUIInterface.h"
#include "SAAbstractRegionSelectEditor.h"
#include "SAAddLineChartSetDialog.h"


class QProgressBar;
class QActionGroup;


class SAInformationStatusWidget;
class ValueDataViewer;
class SAPlotLayerModel;
class DataFeatureTreeModel;
class SATabValueViewerWidget;
class SAValueManagerModel;
class SAFigureWindow;
class SAChart2D;
class QwtPlotItem;

class SAAbstractDataImportInterface;
class SARectRegionSelectEditor;
///
/// \brief The MainWindow class
/// \todo 更改project model
///
#ifdef SA_USE_RIBBON_UI
class MainWindow : public SARibbonMainWindow
#else
class MainWindow : public QMainWindow
#endif
{
    Q_OBJECT
    friend class SAUI;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void init();
    void initUI();
    void initMenu();
    void initPlugin();
    void initTheme();
    //初始化UIReflation
    void initUIReflection();
    //初始化其它进程
    void initProcess();

public:
    static QIcon getIconByWndType(SA::SubWndType type);
    //-----------------------状态栏响应-------------------------
    //状态栏 - 显示进度状态
    //显示消息，如果interval大于0，将会在状态栏上也显示，显示的时间为interval(ms)的值




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
    /// 子窗口相关操作
    /// \group 子窗口相关操作
    /// \{
    //创建一个绘图窗口
    QMdiSubWindow* createFigureWindow(const QString& title = QString());
    QMdiSubWindow* createFigureWindow(SAFigureWindow* fig,const QString& title = QString());
    //判断mdi中是否存在指定的子窗口
    bool isHaveSubWnd(QMdiSubWindow* wndToCheck) const;
    //判断是否存在绘图窗口
    bool isHaveFigureWindow() const;
    //获取当前激活的子窗口
    QMdiSubWindow* getCurrentActiveSubWindow() const;
    //获取最后显示的绘图窗口的指针
    SAFigureWindow* getCurrentFigureWindow() const;
    //获取所有的figure
    QList<SAFigureWindow*> getFigureWindowList() const;
    //获取当前正在显示的Chart指针
    SAChart2D* getCurSubWindowChart() const;
    //获取当前正在显示的Chart指针,如果是一个subplot，返回多个指针
    QList<SAChart2D *> getCurSubWindowCharts() const;
    //用于子窗口激活时刷新“图表设置工具栏的选中状态”
    void updateChartSetToolBar(SAFigureWindow* w=nullptr);
    //获取所有子窗口指针
    QList<QMdiSubWindow*> getSubWindowList() const;
    //从subwindow指针中查找是否含有SAFigureWindow
    static SAFigureWindow* getFigureWidgetFromMdiSubWindow(QMdiSubWindow* sub);
    //记录最后获取焦点的窗口类型，此函数主要用于函数功能模块判断是对图进行操作还是对数据进行操作
    SAUIInterface::LastFocusType lastFocusWidgetType() const;
    //设置多文档激活的窗口和QMdiArea::setActiveSubWindow一样
    void setActiveSubWindow(QMdiSubWindow *window);
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
    //让windowListDock显示到最前面
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
    /// \group 插件控制
    /// \{
    void addDataImportPluginAction(QAction *action);
    //添加导出数据插件的响应槽
    QAction* addDataImportPluginMenu(QMenu *menu);
    //把菜单添加到分析功能的菜单中
    QAction *addAnalysisPluginMenu(QMenu *menu);
#ifdef SA_USE_RIBBON_UI
    //把action加入 ribbon界面的Gallery
    void addAnalysisActionsToRibbonGallery(const QString &name, const QList<QAction *> &actions);
#endif
    /// \}

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


signals:
	/// 
	/// \brief 准备清理项目
	///
	void startCleanProject();
	/// 
	/// \brief 已经清理项目
	///
	void cleanedProject();
    ///
    /// \brief 子窗口创建的信号
    /// \param wnd
    ///
    void subWindowHaveCreated(SAMdiSubWindow* wnd);
    ///
    /// \brief 在主界面变量管理树选择发生改变后会发射此信号
    /// \param dataPtr 选中的数据指针，也可以通过 \a getSelectDatas 获取多选状态下的指针
    ///
    void selectDataChanged(SAAbstractDatas* dataPtr);
private slots:

    //焦点变换触发的槽
    void onFocusChanged(QWidget *old, QWidget *now);

    ///
    /// \group ValueManager tree view signal/slots（变量管理相关）
    /// \{
    ///
    // ValueManager tree clicked
    void onTreeViewValueManagerClicked(const QModelIndex &index);
    // ValueManager tree double clicked
    void onTreeViewValueManagerDoubleClicked(const QModelIndex &index);
    // ValueManager tree right clicked menu
    void onTreeViewValueManagerCustomContextMenuRequested(const QPoint &pos);
    //变量管理treeView-菜单-重名变量
    void onActionRenameValueTriggered();
    /// \}

    ///
    /// \group file menu slots（文件菜单相关）
    /// \{
    ///
    //打开
    void onActionOpenTriggered();
    //打开项目
    void onActionOpenProjectTriggered();
    //保存
    void onActionSaveTriggered();
    //另存为
    void onActionSaveAsTriggered();
    //清除项目
    void onActionClearProjectTriggered();
    /// \}

    ///
    /// \group mdi subwindow slots（多文档界面相关）
    /// \{
    ///
    // mdi sub window active
    void onMdiAreaSubWindowActivated(QMdiSubWindow *arg1);
    // mdi sub window closed
    void onSubWindowClosed(QMdiSubWindow *arg1);
    /// \}



    ///
    /// \group dataManager menu slots(变量操作)
    /// \{
    ///
    //变量管理treeView-菜单-当前选择的数据在当前的tab里浏览
    void onActionViewValueInCurrentTabTriggered();
    //变量管理treeView-菜单-当前选择的数据在当前的tab里追加浏览
    void onActionViewValueAppendInCurrentTabTriggered();
    //变量管理treeView-菜单-当前选择的数据在新标签页里浏览
    void onActionViewValueInNewTabTriggered();
    //变量删除
    void onActionDeleteValueTriggered();
    /// \}

    ///
    /// \group chart set menu slots(绘图操作)
    /// \{
    ///
    //新图例
    void onActionNewChartTriggered();
    //趋势线图
    void onActionAddLineChartTriggered();
    //bar图
    void onActionAddBarChartTriggered();
    //Histogram图
    void onActionAddHistogramChartTriggered();
    //Scatter图
    void onActionAddScatterChartTriggered();
    //Box图
    void onActionAddBoxChartTriggered();
    //IntervalChartTriggered
    void onActionAddIntervalChartTriggered();
    //开始矩形选框工具
    void onActionStartRectSelectTriggered(bool b);
    //开始圆形选框工具
    void onActionStartEllipseSelectTriggered(bool b);
    //开始多边形选框工具
    void onActionStartPolygonSelectTriggered(bool b);
    //清除所有选区
    void onActionClearAllSelectiedRegionTriggered(bool b);
    //选区单选模式
    void onActionSingleSelectionTriggered(bool b);
    //选区多选模式
    void onActionAdditionalSelectionTriggered(bool b);
    //选区减选模式
    void onActionSubtractionSelectionTriggered(bool b);
    //选区交集模式
    void onActionIntersectionSelectionTriggered(bool b);
    //选区移动
    void onActionSelectionRegionMove(bool b);
    //选区范围内的数据移动
    void onActionSelectionRegionDataMove(bool on);


    //开启当前绘图的十字光标
    void onActionEnableChartPickerTriggered(bool check);
    //开启当前绘图的拖动
    void onActionEnableChartPannerTriggered(bool check);
    //开启当前绘图的区间缩放
    void onActionEnableChartZoomTriggered(bool check);
    //当前绘图的缩放还原
    void onActionSetChartZoomToBaseTriggered(bool check);
    //当前绘图放大
    void onActionChartZoomInTriggered(bool check);
    //当前绘图缩小
    void onActionChartZoomOutTriggered(bool check);
    //缩放到最佳视图
    void actionZoomInBestView(bool check);
    //当前绘图重置
    void onActionChartZoomResetTriggered(bool check);
    //拾取y值
    void onActionYDataPickerTriggered(bool on);
    //拾取xy值
    void onActionXYDataPickerTriggered(bool on);
    //网格
    void onActionShowGridTriggered(bool on);
    //水平网格
    void onActionShowHGridTriggered(bool on);
    //垂直网格
    void onActionShowVGridTriggered(bool on);
    //显示密集水平网格
    void onActionShowCrowdedHGridTriggered(bool on);
    //显示密集垂直网格
    void onActionShowCrowdedVGridTriggered(bool on);
    //显示图例
    void onActionShowLegendTriggered(bool on);
    //显示图例选择器
    void onActionShowLegendPanelTriggered(bool on);

    /// \}

    ///
    /// \group chartDataManager menu slots(数据操作)
    /// \{
    ///
    // 剔除选定范围内数据
    void onActionInRangDataRemoveTriggered();
    // 把曲线转换为数据
    void onActionPickCurveToDataTriggered();
    /// \}



    ///
    /// \group about menu slots(关于菜单)
    /// \{
    //about action (关于)
    void onActionAboutTriggered();
    /// \}




    ///
    /// \group edit menu(编辑菜单)
    /// \{
    //Rescind（回退）
    void onActionUndoTriggered();
    //Redo （重做）
    void onActionRedoTriggered();
    /// \}




    ///
    /// \group window menu(窗口菜单)
    /// \{
    //视图 - 恢复默认布局
    void onActionSetDefalutDockPosTriggered();
    //标签模式
    void onActionTabModeTriggered(bool on);
    //窗口模式
    void onActionWindowModeTriggered(bool on);
    //窗口模式 - 层叠布置
    void onActionWindowCascadeTriggered(bool on);
    //窗口模式 - 均匀布置
    void onActionWindowTileTriggered(bool on);
    //显示隐藏DataFeatureDock窗口
    void onActionDataFeatureDockTriggered(bool on);
    //显示隐藏SubWindowListDock窗口
    void onActionSubWindowListDockTriggered(bool on);
    //显示隐藏ValueManagerDock窗口
    void onActionValueManagerDockTriggered(bool on);
    //显示隐藏LayerOutDock窗口
    void onActionLayerOutDockTriggered(bool on);
    //显示隐藏ValueViewerDock窗口
    void onActionValueViewerDockTriggered(bool on);
    //显示隐藏FigureViewer窗口
    void onActionFigureViewerTriggered(bool on);
    //显示隐藏message窗口
    void onActionMessageInfoDockTriggered(bool on);
    //显示隐藏figure set 窗口
    void onActionFigureSetDockTriggered(bool on);
    /// \}

    ///
    /// \group Tool menu(工具菜单)
    /// \{
    // 项目属性设置
    void onActionProjectSettingTriggered();
    /// \}

    ///
    /// 皮肤切换
    ///
    void onActionSkinChanged(QAction* act);

    ///
    //图层管理窗口改变了条目的可见性
    void onLayoutWidgetItemVisibleChanged(SAChart2D* chart,QwtPlotItem* item,bool on);
    //图层管理窗口改变了条目的颜色
    void onLayoutWidgetItemColorChanged(SAChart2D* chart,QwtPlotItem* item,QColor clr);
    //图层管理窗口删除了条目
    void onLayoutWidgetItemRemoved(SAChart2D* chart,QwtPlotItem* item);
public:
    //设置皮肤
    void setSkin(const QString& name);
    //获取ui接口
    SAUIInterface* uiInterface();
    //获取当前ui选择的区域选择模式
    SAAbstractRegionSelectEditor::SelectionMode getCurrentChartRegionSelectionMode() const;

    // 子窗口的创建
    template<typename INNERWIDGET>
    SAMdiSubWindow* createMdiSubWindow(SA::SubWndType type,const QString& title);
    // 子窗口的创建
    template<typename INNERWIDGET,typename Arg0>
    SAMdiSubWindow* createMdiSubWindow(SA::SubWndType type,const QString& title,Arg0 arg0);
    // 子窗口的创建
    SAMdiSubWindow* createMdiSubWindow(SA::SubWndType type,QWidget* w,const QString& title);



    //把一个XYSeries转换为value
    void makeValueFromXYSeries(const QString& name, SA::PickDataMode pickMode, const QVector<QPointF>& xy);

//-------dock-图层表格相关---------------------
    //图层表格model
	SAPlotLayerModel* getPlotLayerModel() const;

//------openfilemanager
    //OpenFileManager* getValueManager() const;
    SAValueManagerModel *getValueManagerModel() const;
    //
    SADrawDelegate* getDrawDelegate() const;
protected:
//    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
//    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
//    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
private:
    //打开文件
    bool openFile(const QString& fullPath);
    //打开项目文件夹
    bool openProject(const QString& projectPath);
    //配置信息保存
    void saveSetting();
    //配置信息加载
    void loadSetting();
    //保存最近打开的文件内容信息
    void saveRecentPath();
    //保存最近打开的文件内容信息
    void loadRecentPath();
    //刷新最近打开菜单
    void updateRecentPathMenu();

    //释放chart的附加editor
    void releaseChart2DEditor(SAChart2D* chart);

    //设置项目信息
    bool setProjectInfomation();
    //变量管理器的移除控制触发的槽
    Q_SLOT void onDataRemoved(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
    //============================================================
    void updateChartGridActionState(SAChart2D *chart);
    void updateSelectActionState(SAChart2D *chart);
    void updateChartEditorActionState(SAChart2D *chart);
    //子窗口序列化
    void __saveSubWindowToFolder(const QString& folderPath);
    void __loadSubWindowFromFolder(const QString& folderPath);
private:
#ifdef SA_USE_RIBBON_UI
    QScopedPointer<MainWindowPrivate> ui;
#else
    Ui::MainWindow *ui;
#endif
    SAUI* m_uiInterface;
    progressStateWidget* ui_status_progress;///< 状态栏上的进度信息
    SAInformationStatusWidget* ui_status_info;///< 状态栏上的特殊信息
    QMenu* m_menuTreeProjectItem;///< 项目树的弹出菜单
    SAPluginManager* m_pluginManager;///< 插件管理器
    int m_nProjectCount;//记录项目总数

    SAMdiSubWindowManager m_mdiManager;

    QScopedPointer<SADrawDelegate> m_drawDelegate;///< 绘图代理
    QActionGroup* m_chartRegionSelectionShapeActionGroup;///<选区选择形状的action group
    QActionGroup* m_chartRegionSelectionModeActionGroup;///<选区选择模式的action group
    int m_lastForceType;///< 记录最后的焦点信息
    unsigned int m_nUserChartCount;
    QStringList m_recentOpenFiles;///< 记录最近打开的文件
    QStringList m_recentOpenProjectFolders; ///< 记录最近打开的项目目录
};

///
/// \brief 子窗口的创建
/// \param type 窗口类型
/// \param title 窗口标题
/// \return MdiSubWindow的指针,若没创建成功返回nullptr
///
template<typename INNERWIDGET>
SAMdiSubWindow *MainWindow::createMdiSubWindow(SA::SubWndType type, const QString &title)
{
    SAMdiSubWindow* pSubw = m_mdiManager.newMdiSubWnd<SAMdiSubWindow,INNERWIDGET>();
    if(nullptr == pSubw)
        return pSubw;
    pSubw->setType(type);
    pSubw->setWindowTitle(title);
    pSubw->setWindowIcon(getIconByWndType(type));
    connect(pSubw,&SAMdiSubWindow::closedWindow
            ,this,&MainWindow::onSubWindowClosed);
    emit subWindowHaveCreated(pSubw);
    return pSubw;
}

template<typename INNERWIDGET,typename Arg0>
SAMdiSubWindow *MainWindow::createMdiSubWindow(SA::SubWndType type, const QString &title, Arg0 arg0)
{
    SAMdiSubWindow* pSubw = m_mdiManager.newMdiSubWnd<SAMdiSubWindow,INNERWIDGET>(arg0);
    if(nullptr == pSubw)
        return pSubw;
    pSubw->setType(type);
    pSubw->setWindowTitle(title);
    pSubw->setWindowIcon(getIconByWndType(type));
    connect(pSubw,&SAMdiSubWindow::closedWindow
            ,this,&MainWindow::onSubWindowClosed);
    emit subWindowHaveCreated(pSubw);
    return pSubw;
}



///
/// \brief 根据子窗口类型获取后缀名
/// \param type 子窗口类型
/// \return 后缀名
///
QString get_sub_window_type_suffix(SA::SubWndType type);
//保存子窗口到文件
bool save_sub_window(SAMdiSubWindow* w, const QString &folderPath, QString *errString);
//重文件加载子窗口
QMdiSubWindow* load_sub_window(SAUIInterface *ui, const QString& filePath, QString *errString);
//把没能和SubWindowList对应的文件删除
void remove_figure_file_not_in_sub_window_list(const QString &folderPath,const QList<QMdiSubWindow*>& subWindows);
#endif // MAINWINDOW_H
