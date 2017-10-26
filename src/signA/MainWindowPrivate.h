#ifndef MAINWINDOWPRIVATE_H
#define MAINWINDOWPRIVATE_H

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
class MainWindow;
class MainWindowPrivate
{
public:
    MainWindow* Parent;

    QAction *actionOpen;
        QAction *actionWindowCascade;
        QAction *actionWindowTile;
        QAction *actionWindowMode;
        QAction *actionTabMode;
        QAction *actionSave;
        QAction *actionQuite;
        QAction *actionDataFeatureDock;
        QAction *actionNewChart;
        QAction *actionNewTrend;
        QAction *actionClearProject;
        QAction *actionInRangDataRemove;
        QAction *actionOutRangDataRemove;
        QAction *actionExportReport;
        QAction *actionSubWindowListDock;
        QAction *actionEnableChartPicker;
        QAction *actionEnableChartPanner;
        QAction *actionEnableChartZoom;
        QAction *actionYDataPicker;
        QAction *actionShowGrid;
        QAction *actionShowHGrid;
        QAction *actionShowVGrid;
        QAction *actionShowCrowdedHGrid;
        QAction *actionShowCrowdedVGrid;
        QAction *actionShowLegend;
        QAction *actionLegendPanel;
        QAction *actionChartSet;
        QAction *actionChartZoomReset;
        QAction *actionPickCurveToData;
        QAction *actionAbout;
        QAction *actionRescind;
        QAction *actionRedo;
        QAction *actionValueManagerDock;
        QAction *actionViewValueInCurrentTab;
        QAction *actionViewValueInNewTab;
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
        QAction *actionZoomBase;
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
        QWidget *centralWidget;


        SARibbonBar *menuBar;
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

        SARibbonMenu *menuFile;
        SARibbonMenu *menuLineChart;
        //!Chart Operate
        SARibbonCategory* operateRibbonCategory;
        SARibbonPannel* operateCategorySelectEditorPannel;
        //!View Category Page
        SARibbonCategory* viewRibbonCategory;
        SARibbonPannel* viewCategoryWindowPannel;
        SARibbonToolButton* ribbonButtonFigureViewer;
        SARibbonToolButton* ribbonButtonDataFeatureDock;
        SARibbonToolButton* ribbonButtonLayerOutDock;
        SARibbonToolButton* ribbonButtonValueViewerDock;
        SARibbonToolButton* ribbonButtonValueManagerDock;
        SARibbonToolButton* ribbonButtonAllDock;
        SARibbonToolButton* ribbonButtonSetDefalutDockPos;
        SARibbonPannel* windowModeCategoryWindowPannel;
        SARibbonToolButton* ribbonButtonWindowMode;
        SARibbonToolButton* ribbonButtonTabMode;
        SARibbonToolButton* ribbonButtonWindowCascade;
        SARibbonToolButton* ribbonButtonWindowTile;


        SARibbonMenu *menuWindowsViewSet;

        //

        SARibbonMenu *menuExport;
        SARibbonMenu *menuImport;

        SARibbonMenu *menuAnalysis;
        SARibbonMenu *menuData;
        SARibbonMenu *menuChartDataManager;
        SARibbonMenu *menuDataManager;
        SARibbonMenu *menuChartSet;
        SARibbonMenu *menuGrid;
        SARibbonMenu *menuLegend;
        SARibbonMenu *menuZoomSet;
        SARibbonMenu *menuDataPickMenu;
        SARibbonMenu *menuRegionSelect;
        SARibbonMenu *menuHelp;
        SARibbonMenu *menuEdit;
        SARibbonMenu *menuTool;

        QStatusBar *statusBar;
        QDockWidget *dockWidget_DataFeature;
        QWidget *dockWidgetContents_2;
        QVBoxLayout *verticalLayout_2;
        SADataFeatureWidget *dataFeatureWidget;
        QToolBar *toolBar_chart;
        QToolBar *toolBar_chartSet;
        QToolBar *toolBar_plot;
        QDockWidget *dockWidget_windowList;
        QWidget *dockWidgetContents_4;
        QVBoxLayout *verticalLayout_6;
        QListView *listView_window;
        QDockWidget *dockWidget_valueManage;
        QWidget *dockWidgetContents_3;
        QVBoxLayout *verticalLayout_3;
        SAValueManagerTreeView *treeView_valueManager;
        QDockWidget *dockWidget_plotLayer;
        QWidget *dockWidgetContents_5;
        QVBoxLayout *verticalLayout_5;
        QTableView *tableView_layer;
        QDockWidget *dockWidget_chartDataViewer;
        QWidget *dockWidgetContents_chartDataViewer;
        QVBoxLayout *verticalLayout_7;
        QSplitter *splitter_chartDataViewer;
        QTreeView *treeView_curPlotItem;
        QTableView *tableView_curSelItemDatas;
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
        QToolBar *toolBarChartTools;

    MainWindowPrivate(MainWindow* p);
    void init();
    void retranslateUi(MainWindow *mainWinowPtr);
private:
    void setupUi(MainWindow *mainWinowPtr);

};

#endif // MAINWINDOWPRIVATE_H
