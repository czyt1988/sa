#ifndef MAINWINDOWPRIVATE_H
#define MAINWINDOWPRIVATE_H
#define RIBBON_CONTEXT_CATEGORY_ID_CHART_SET (10)
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <SADataFeatureWidget.h>
#include <SAMessageWidget.h>
#include <SATabValueViewerWidget.h>
#include "SAFiugreSetWidget.h"
#include "SAValueManagerTreeView.h"
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include "SARibbonMenu.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonContextCategory.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonGallery.h"
#include "SARibbonApplicationButton.h"
#include "SAChartDatasViewWidget.h"
#include "SAFigureLayoutWidget.h"
class MainWindow;
class MainWindowPrivate
{
public:
    MainWindow* Parent;

    QAction *actionOpen;
    QAction *actionClearRecentOpenFileHistroy;///< 清除最近打开文件记录
    QAction *actionClearRecentOpenProjectorHistroy;///< 清除最近打开项目记录
    QAction *actionWindowCascade;
    QAction *actionWindowTile;
    QAction *actionWindowMode;
    QAction *actionTabMode;
    QAction *actionSave;
    QAction *actionQuit;
    QAction *actionDataFeatureDock;
    QAction *actionNewChart;
    QAction *actionNewTrend;
    QAction *actionClearProject;
    QAction *actionInRangDataRemove;
    QAction *actionOutRangDataRemove;
    QAction *actionSubWindowListDock;
    QAction *actionEnableChartCrossCursor;
    QAction *actionEnableChartPanner;
    QAction *actionEnableChartZoom;
    QAction *actionYDataPicker;
    QAction *actionShowGrid;
    QAction *actionShowHGrid;
    QAction *actionShowVGrid;
    QAction *actionShowCrowdedHGrid;
    QAction *actionShowCrowdedVGrid;
    QAction *actionShowLegend;
    QAction *actionShowLegendPanel;
    QAction *actionChartZoomReset;
    QAction *actionPickCurveToData;
    QAction *actionAbout;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionValueManagerDock;
    QAction *actionMessageInfoDock;
    QAction *actionFigureSetDock;
    QAction *actionViewValueInCurrentTab;
    QAction *actionViewValueInNewTab;
    QAction *actionViewValueAppendInCurrentTab;
    QAction *actionLayerOutDock;
    QAction *actionRenameValue;
    QAction *actionSetDefalutDockPos;
    QAction *actionValueViewerDock;
    QAction *actionFigureViewer;
    QAction *actionXYDataPicker;
    QAction *actionDeleteValue;
    QAction *actionOpenProject;
    QAction *actionSaveAs;
    QAction *actionProjectSetting;
    QAction *actionZoomInBestView;
    QAction *actionSetZoomBase;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionStartRectSelect;
    QAction *actionStartEllipseSelect;
    QAction *actionStartPolygonSelect;
    QAction *actionClearAllSelectiedRegion;
    QAction *actionSingleSelection;
    QAction *actionAdditionalSelection;
    QAction *actionIntersectionSelection;
    QAction *actionSubtractionSelection;
    QAction *actionDrawScatterChart;
    QAction *actionDrawHistogramChart;
    QAction *actionDrawBarChart;
    QAction *actionDrawBoxChart;
    QAction *actionDrawIntervalChart;
    QAction *actionSelectionRegionMove;///<
    QAction *actionSelectionRegionDataMove;///<

    QAction *actionSelectSkin;///<
    QActionGroup* actionGroupSkins;
    QActionGroup* actionGroupSelection;
    QActionGroup* actionChartEditor;///< 图像编辑器的互斥



    SARibbonBar *menuBar;
    SARibbonButtonGroupWidget* ribbonRightTopBar;
    SARibbonApplicationButton* ribbonApplicationButton;
    //!Main Category Page

    SARibbonCategory* mainRibbonCategory;
    SARibbonPannel* mainCategoryFilePannel;
    SARibbonToolButton* ribbonButtonFileOpen;
    SARibbonToolButton* ribbonButtonOpen;
    SARibbonToolButton* ribbonButtonSave;
    SARibbonToolButton* ribbonButtonClearProject;

    SARibbonPannel* mainCategoryChartPannel;
    SARibbonToolButton* ribbonButtonNewChart;
    SARibbonToolButton* ribbonButtonLineChart;
    SARibbonToolButton* ribbonButtonBarChart;
    SARibbonToolButton* ribbonButtonHistogramChart;
    SARibbonToolButton* ribbonButtonBoxChart;
    SARibbonToolButton* ribbonButtonScatterChart;
    SARibbonMenu *menuFile;
    SARibbonMenu *menuRecentOpenFile;
    SARibbonMenu *menuRecentOpenProject;
    SARibbonMenu *menuLineChart;
    SARibbonMenu *menuBarChart;
    SARibbonMenu *menuHistogramChart;
    SARibbonMenu *menuBoxChart;
    SARibbonMenu *menuScatterChart;
    SARibbonMenu *menuSkinList;
    //! operate Category Page
    SARibbonCategory* operateRibbonCategory;
    SARibbonMenu *menuSelection;
    SARibbonPannel* operateCategoryChartEditorPannel;
    SARibbonToolButton* ribbonButtonStartSelection;

    SARibbonToolButton* ribbonButtonSingleSelection;
    SARibbonToolButton* ribbonButtonAdditionalSelection;
    SARibbonToolButton* ribbonButtonSubtractionSelection;
    SARibbonToolButton* ribbonButtonIntersectionSelection;
    SARibbonButtonGroupWidget* ribbonButtonGroupSelectionMode;

    SARibbonToolButton* ribbonButtonSelectionRegionMove;

    SARibbonPannel* operateCategoryDataViewPannel;
    SARibbonToolButton* ribbonButtonChartCrossCursor;
    SARibbonToolButton* ribbonButtonChartPanner;
    SARibbonToolButton* ribbonButtonChartZoom;
    SARibbonToolButton* ribbonButtonXYDataPicker;
    SARibbonToolButton* ribbonButtonYDataPicker;
    SARibbonMenu *menuZoomSet;


    SARibbonToolButton* ribbonButtonInRangDataRemove;
    SARibbonToolButton* ribbonButtonPickCurveToData;
    SARibbonToolButton* ribbonButtonSelectionRegionDataMove;

    SARibbonMenu *menuDataRemove;
    //legend pannel
    SARibbonPannel* chartLegendCategoryWindowPannel;
    SARibbonToolButton* ribbonButtonShowLegend;
    SARibbonToolButton* ribbonButtonShowLegendPanel;

    SARibbonPannel* chartGridCategoryWindowPannel;
    SARibbonToolButton* ribbonButtonShowGrid;
    SARibbonToolButton* ribbonButtonShowHGrid;
    SARibbonToolButton* ribbonButtonShowVGrid;
    SARibbonToolButton* ribbonButtonShowCrowdedGrid;
    SARibbonMenu *menuShowCrowdedGrid;
    //! Analysis
    SARibbonCategory* analysisRibbonCategory;
    SARibbonPannel* analysisCategoryFunctionPannel;
    SARibbonToolButton* ribbonButtonAllFuntion;
    SARibbonGallery* ribbonGalleryFuntions;
    //!View Category Page
    SARibbonCategory* viewRibbonCategory;
    SARibbonPannel* viewCategoryWindowPannel;
    SARibbonToolButton* ribbonButtonFigureViewer;
    SARibbonToolButton* ribbonButtonDataFeatureDock;
    SARibbonToolButton* ribbonButtonLayerOutDock;
    SARibbonToolButton* ribbonButtonValueViewerDock;
    SARibbonToolButton* ribbonButtonValueManagerDock;
    SARibbonToolButton* ribbonButtonFigureSetDock;
    SARibbonToolButton* ribbonButtonAllDock;
    SARibbonToolButton* ribbonButtonSetDefalutDockPos;
    SARibbonPannel* windowModeCategoryWindowPannel;
    SARibbonToolButton* ribbonButtonWindowMode;
    SARibbonToolButton* ribbonButtonTabMode;
    SARibbonToolButton* ribbonButtonWindowCascade;
    SARibbonToolButton* ribbonButtonWindowTile;





    //===========================================================
    SARibbonMenu *menuWindowsViewSet;

    //

    SARibbonMenu *menuExport;
    SARibbonMenu *menuImport;

    SARibbonMenu *menuAnalysis;
    SARibbonMenu *menuDataManager;
    SARibbonMenu *menuChartSet;
    SARibbonMenu *menuGrid;

    SARibbonMenu *menuRegionSelect;
    SARibbonMenu *menuHelp;
    SARibbonMenu *menuEdit;
    SARibbonMenu *menuTool;

    QStatusBar *statusBar;
    QDockWidget *dockWidget_DataFeature;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_2;
    SADataFeatureWidget *dataFeatureWidget;
    QDockWidget *dockWidget_windowList;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_6;
    QListView *listView_window;
    QDockWidget *dockWidget_valueManage;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_3;
    SAValueManagerTreeView *treeView_valueManager;


    //dock chartDataViewer
    QDockWidget *dockWidget_chartDataViewer;
    SAChartDatasViewWidget* chartDatasViewWidget;
    //dock plotLayer
    QDockWidget *dockWidget_plotLayer;
    SAFigureLayoutWidget* figureLayoutWidget;




    QDockWidget *dockWidget_main;
    QWidget *dockWidgetContents_7;
    QVBoxLayout *verticalLayout_4;
    QMdiArea *mdiArea;
    QDockWidget *dockWidget_valueViewer;
    QWidget *dockWidgetContents_6;
    QVBoxLayout *verticalLayout_8;
    SATabValueViewerWidget *tabWidget_valueViewer;
    QDockWidget *dockWidget_message;
    QWidget *dockWidgetContents_8;
    QVBoxLayout *verticalLayout_9;
    SAMessageWidget *saMessageWidget;
    QDockWidget *dockWidget_plotSet;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    SAFiugreSetWidget *figureSetWidget;
        //QToolBar *toolBarChartTools;

    MainWindowPrivate(MainWindow* p);
    void init();
    void retranslateUi(MainWindow *mainWinowPtr);
private:
    void setupUi(MainWindow *mainWinowPtr);

};

#endif // MAINWINDOWPRIVATE_H
