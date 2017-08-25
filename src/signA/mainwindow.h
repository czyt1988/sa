#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
namespace Ui {
class MainWindow;
}


class QProgressBar;



class SAInformationStatusWidget;
class ValueDataViewer;
class SAPlotLayerModel;
class QwtPlotItemTreeModel;
class QwtPlotItemDataModel;
class DataFeatureTreeModel;
class SATabValueViewerWidget;
class SAValueManagerModel;
class SAFigureWindow;
class SAChart2D;
class QwtPlotItem;

class SAAbstractDataImportInterface;

///
/// \brief The MainWindow class
/// \todo 更改project model
///
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    friend class SAUI;
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
    static QColor getRandColor();
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
    //从subwindow指针中查找是否含有SAFigureWindow
    static SAFigureWindow* getFigureWidgetFromMdiSubWindow(QMdiSubWindow* sub);
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
    /// \}

    ///
    /// \group 插件控制
    /// \{
    void addDataImportPluginAction(QAction *action);
    //添加导出数据插件的响应槽
    QAction* addDataImportPluginMenu(QMenu *menu);
    //把菜单添加到分析功能的菜单中
    QAction *addAnalysisPluginMenu(QMenu *menu);
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
	void cleaningProject();
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
    /// \group chartDataManager menu slots(数据操作)
    /// \{
    ///
    // 剔除选定范围内数据
    void onActionInRangDataRemoveTriggered();
    // 把曲线转换为数据
    void onActionPickCurveToDataTriggered();
    /// \}

    ///
    /// \group dataManager menu slots(变量操作)
    /// \{
    ///
    //变量管理treeView-菜单-当前选择的数据在当前的tab里浏览
    void onActionViewValueInCurrentTabTriggered();
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
    //趋势图
    void onActionNewTrendTriggered();
    /// \}


    ///
    /// \group about menu slots(关于菜单)
    /// \{
    //about action (关于)
    void onActionAboutTriggered();
    /// \}

    ///
    /// \group layout table slots(图层表格相关)
    /// \{
    //图层表格鼠标的点击
    void onTableViewLayerPressed(const QModelIndex &index);
    /// \}

    ///
    /// \group TreeView CurPlotItem slots(曲线条目树形窗口)
    /// \{
    void onTreeViewCurPlotItemClicked(const QModelIndex &index);
    /// \}

    ///
    /// \group edit menu(编辑菜单)
    /// \{
    //Rescind（回退）
    void onActionRescindTriggered();
    //Redo （重做）
    void onActionRedoTriggered();
    /// \}




    ///
    /// \group window menu(窗口菜单)
    /// \{
    //视图 - 恢复默认布局
    void onActionSetDefalutDockPosTriggered();
    /// \}

    ///
    /// \group Tool menu(工具菜单)
    /// \{
    // 项目属性设置
    void onActionProjectSettingTriggered();
    /// \}


public slots:
    void onChartDataChanged(QWidget* widget,const QwtPlotItem* pC);
public:

    ///
    /// \brief 子窗口的创建
    /// \param type 窗口类型
    /// \param title 窗口标题
    /// \return MdiSubWindow的指针,若没创建成功返回nullptr
    ///
    template<typename INNERWIDGET>
    SAMdiSubWindow* createMdiSubWindow(SA::SubWndType type,const QString& title)
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
    SAMdiSubWindow* createMdiSubWindow(SA::SubWndType type,const QString& title,Arg0 arg0)
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
    //MdiSubWindow* createMdiSubWnd_ThermalDiagram(const QString &title);




    void pickCurData(const QString& name, QwtPlotCurve* cur, SA::PickDataMode pickMode, const QRectF& rang);

    //-------dock-图层表格相关---------------------
    //图层表格model
	SAPlotLayerModel* getPlotLayerModel() const;
    //-------dock-数据预览相关
    QwtPlotItemTreeModel* getDataViewerPlotItemTreeModel() const;
    QwtPlotItemDataModel* getDataViewerPlotItemDataModel() const;

    //------openfilemanager
    //OpenFileManager* getValueManager() const;
    SAValueManagerModel *getValueManagerModel() const;
    //
    SADrawDelegate* getDrawDelegate() const;
protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
private:
    //配置信息加载
    void saveSetting();
    void loadSetting();
    void saveWindowState(QSettings& setting);
    void loadWindowState(const QSettings& setting);

    //图层表格的选择样式生成
    QString getPlotLayerNewItemSelectedQSS(const QColor& rgb);
    //设置项目信息
    bool setProjectInfomation();
    //变量管理器的移除控制触发的槽
    Q_SLOT void onDataRemoved(const QList<SAAbstractDatas*>& dataBeDeletedPtr);
    //============================================================

private:
    Ui::MainWindow *ui;
    SAUI* uiInterface;
    progressStateWidget* ui_status_progress;///< 状态栏上的进度信息
    SAInformationStatusWidget* ui_status_info;///< 状态栏上的特殊信息
    QMenu* m_menuTreeProjectItem;///< 项目树的弹出菜单
    QMenu* m_menuValueManagerView;///< 变量管理弹出菜单
    QMdiSubWindow* m_lastActiveWnd;///< 记录最后激活的窗口

    SAPluginManager* m_pluginManager;///< 插件管理器
    int m_nProjectCount;//记录项目总数
    int m_nUserChartCount;///< 用户图表标志

    SAMdiSubWindowManager m_mdiManager;

    std::unique_ptr<SADrawDelegate> m_drawDelegate;///< 绘图代理
    SAFigureWindow* m_lastShowFigureWindow;///<记录最后一次显示的figure window
};


#endif // MAINWINDOW_H
